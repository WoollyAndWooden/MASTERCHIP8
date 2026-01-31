import numpy as np
import tensorflow as tf
from tensorflow import keras
from tensorflow.keras import layers
import gymnasium as gym
from .env import PongEnv
import os

GAMMA = 0.99
EPSILON_START = 1.0
EPSILON_MIN = 0.1
EPSILON_DECAY_STEPS = 100000
BATCH_SIZE = 32
MAX_STEPS_PER_EPISODE = 10000 

BASE_DIR = os.path.dirname(os.path.abspath(__file__))
MODEL_PATH = os.path.join(BASE_DIR, "pong_dqn.keras")

def create_q_model():
    inputs = layers.Input(shape=(32, 64, 1))
    
    layer1 = layers.Conv2D(32, 8, strides=4, activation="relu")(inputs)
    layer2 = layers.Conv2D(64, 4, strides=2, activation="relu")(layer1)
    
    layer4 = layers.Flatten()(layer2)
    
    layer5 = layers.Dense(512, activation="relu")(layer4)
    
    action = layers.Dense(3, activation="linear")(layer5)
    
    return keras.Model(inputs=inputs, outputs=action)

def load_or_create_model(path):
    if os.path.exists(path):
        print(f"Loading model from {path}")
        try:
            return keras.models.load_model(path)
        except Exception as e:
            print(f"Error loading model: {e}. Creating new model.")
            return create_q_model()
    else:
        print(f"Creating new model for {path}")
        return create_q_model()

def train(rom_path, display_enabled=False):
    render_mode = "human" if display_enabled else None
    env = PongEnv(rom_path, render_mode=render_mode)
    
    model = load_or_create_model(MODEL_PATH)
    target_model = create_q_model()
    
    try:
        target_model.set_weights(model.get_weights())
    except ValueError:
        print("Model architecture mismatch. Creating new model.")
        model = create_q_model()
        target_model.set_weights(model.get_weights())
    
    optimizer = keras.optimizers.Adam(learning_rate=0.00025, clipnorm=1.0)
    loss_function = keras.losses.Huber()
    
    action_history = []
    state_history = []
    state_next_history = []
    rewards_history = []
    done_history = []
    episode_reward_history = []
    
    running_reward = 0
    episode_count = 0
    frame_count = 0
    
    epsilon = EPSILON_START
    max_memory_length = 100000
    
    try:
        while True: 
            state, _ = env.reset()
            episode_reward = 0
            
            for step in range(MAX_STEPS_PER_EPISODE):
                frame_count += 1
                
                if epsilon > np.random.rand(1)[0]:
                    action = np.random.choice(3)
                else:
                    state_tensor = tf.convert_to_tensor(state)
                    state_tensor = tf.expand_dims(state_tensor, 0)
                    action_probs = model(state_tensor, training=False)
                    action = tf.argmax(action_probs[0]).numpy()
                
                epsilon -= (EPSILON_START - EPSILON_MIN) / EPSILON_DECAY_STEPS
                epsilon = max(epsilon, EPSILON_MIN)
                
                state_next, reward, done, truncated, _ = env.step(action)
                
                episode_reward += reward
                
                action_history.append(action)
                state_history.append(state)
                state_next_history.append(state_next)
                rewards_history.append(reward)
                done_history.append(done)
                
                state = state_next
                
                if frame_count % 4 == 0 and len(done_history) > BATCH_SIZE:
                    indices = np.random.choice(range(len(done_history)), size=BATCH_SIZE)
                    
                    state_sample = np.array([state_history[i] for i in indices])
                    state_next_sample = np.array([state_next_history[i] for i in indices])
                    rewards_sample = [rewards_history[i] for i in indices]
                    action_sample = [action_history[i] for i in indices]
                    done_sample = tf.convert_to_tensor([float(done_history[i]) for i in indices])
                    
                    future_rewards = target_model.predict(state_next_sample, verbose=0)
                    updated_q_values = rewards_sample + GAMMA * tf.reduce_max(future_rewards, axis=1)
                    updated_q_values = updated_q_values * (1 - done_sample) - done_sample
                    
                    masks = tf.one_hot(action_sample, 3)
                    
                    with tf.GradientTape() as tape:
                        q_values = model(state_sample)
                        q_action = tf.reduce_sum(tf.multiply(q_values, masks), axis=1)
                        loss = loss_function(updated_q_values, q_action)
                        
                    grads = tape.gradient(loss, model.trainable_variables)
                    optimizer.apply_gradients(zip(grads, model.trainable_variables))
                    
                if frame_count % 10000 == 0:
                    target_model.set_weights(model.get_weights())
                    print(f"Frame {frame_count}, Running Reward: {running_reward:.2f}, Epsilon: {epsilon:.2f}")
                    
                if len(rewards_history) > max_memory_length:
                    del rewards_history[:1]
                    del state_history[:1]
                    del state_next_history[:1]
                    del action_history[:1]
                    del done_history[:1]
                    
                if done:
                    break
            
            episode_reward_history.append(episode_reward)
            if len(episode_reward_history) > 100:
                del episode_reward_history[:1]
            running_reward = np.mean(episode_reward_history)
            
            episode_count += 1
            if episode_count % 10 == 0:
                print(f"Episode {episode_count} finished. Reward: {episode_reward}. Running Reward: {running_reward:.2f}")
                
            if episode_count % 50 == 0:
                print("Saving model...")
                model.save(MODEL_PATH)
                
    except KeyboardInterrupt:
        print("\nTraining interrupted. Saving model...")
        model.save(MODEL_PATH)
        print("Model saved.")
