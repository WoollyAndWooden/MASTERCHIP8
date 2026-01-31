import numpy as np
import tensorflow as tf
from tensorflow import keras
from tensorflow.keras import layers
import gymnasium as gym
from .env import TicTacToeEnv
import os

GAMMA = 0.99
EPSILON_START = 1.0
EPSILON_MIN = 0.1
EPSILON_DECAY_STEPS = 5000
BATCH_SIZE = 32
MAX_STEPS_PER_EPISODE = 100

BASE_DIR = os.path.dirname(os.path.abspath(__file__))
MODEL_P1_PATH = os.path.join(BASE_DIR, "tictactoe_p1.keras")
MODEL_P2_PATH = os.path.join(BASE_DIR, "tictactoe_p2.keras")

def create_q_model():
    inputs = layers.Input(shape=(22, 22, 1))
    
    layer1 = layers.Conv2D(32, 3, strides=1, activation="relu")(inputs)
    layer2 = layers.Conv2D(64, 3, strides=1, activation="relu")(layer1)
    
    layer3 = layers.Flatten()(layer2)
    
    layer4 = layers.Dense(128, activation="relu")(layer3)
    
    action = layers.Dense(9, activation="linear")(layer4)
    
    return keras.Model(inputs=inputs, outputs=action)

def load_or_create_model(path):
    if os.path.exists(path):
        print(f"Loading model from {path}")
        return keras.models.load_model(path)
    else:
        print(f"Creating new model for {path}")
        return create_q_model()

def train(rom_path, display_enabled=False):
    render_mode = "human" if display_enabled else None
    env = TicTacToeEnv(rom_path, render_mode=render_mode)
    
    model_p1 = load_or_create_model(MODEL_P1_PATH)
    target_p1 = create_q_model()
    target_p1.set_weights(model_p1.get_weights())
    
    model_p2 = load_or_create_model(MODEL_P2_PATH)
    target_p2 = create_q_model()
    target_p2.set_weights(model_p2.get_weights())
    
    optimizer_p1 = keras.optimizers.Adam(learning_rate=0.001)
    optimizer_p2 = keras.optimizers.Adam(learning_rate=0.001)
    loss_function = keras.losses.Huber()
    
    memory_p1 = []
    memory_p2 = []
    
    epsilon = EPSILON_START
    episode_count = 0
    
    try:
        while True:
            state, _ = env.reset()
            done = False
            turn = 1 
            
            last_state_p1 = None
            last_action_p1 = None
            last_state_p2 = None
            last_action_p2 = None
            
            step_count = 0
            
            while not done and step_count < 50: 
                step_count += 1
                
                if display_enabled:
                    env.render()
                
                valid_moves = list(range(9))
                
                current_model = model_p1 if turn == 1 else model_p2
                
                if np.random.rand() < epsilon:
                    action = np.random.choice(valid_moves)
                else:
                    state_tensor = tf.convert_to_tensor(state)
                    state_tensor = tf.expand_dims(state_tensor, 0)
                    q_values = current_model(state_tensor, training=False)
                    action = np.argmax(q_values.numpy()[0])

                next_state, reward, done, _, info = env.step(action)
                
                valid_move = info.get("valid", False)
                
                if turn == 1:
                    if last_state_p2 is not None:
                        r_p2 = -reward if reward > 1 else 0 
                        d_p2 = done
                        memory_p2.append((last_state_p2, last_action_p2, r_p2, state, d_p2))
                    
                    memory_p1.append((state, action, reward, next_state, done))
                    
                    last_state_p1 = state
                    last_action_p1 = action
                    
                    if done:
                        if last_state_p2 is not None:
                             memory_p2.append((last_state_p2, last_action_p2, -reward, next_state, done))

                else: 
                    if last_state_p1 is not None:
                        r_p1 = -reward if reward > 1 else 0
                        d_p1 = done
                        memory_p1.append((last_state_p1, last_action_p1, r_p1, state, d_p1))
                        
                    memory_p2.append((state, action, reward, next_state, done))
                    
                    last_state_p2 = state
                    last_action_p2 = action
                    
                    if done:
                        if last_state_p1 is not None:
                            memory_p1.append((last_state_p1, last_action_p1, -reward, next_state, done))

                state = next_state
                
                if valid_move:
                    turn *= -1
                
            if len(memory_p1) > BATCH_SIZE:
                indices = np.random.choice(len(memory_p1), BATCH_SIZE)
                batch = [memory_p1[i] for i in indices]
                train_batch(model_p1, target_p1, batch, optimizer_p1, loss_function)
                
            if len(memory_p2) > BATCH_SIZE:
                indices = np.random.choice(len(memory_p2), BATCH_SIZE)
                batch = [memory_p2[i] for i in indices]
                train_batch(model_p2, target_p2, batch, optimizer_p2, loss_function)

            epsilon -= (EPSILON_START - EPSILON_MIN) / EPSILON_DECAY_STEPS
            epsilon = max(epsilon, EPSILON_MIN)
            
            episode_count += 1
            if episode_count % 10 == 0:
                print(f"Episode {episode_count} complete. Epsilon: {epsilon:.2f}")
                
            if episode_count % 50 == 0:
                print("Saving models...")
                model_p1.save(MODEL_P1_PATH)
                model_p2.save(MODEL_P2_PATH)
                target_p1.set_weights(model_p1.get_weights())
                target_p2.set_weights(model_p2.get_weights())
                
    except KeyboardInterrupt:
        print("\nTraining interrupted. Saving models...")
        model_p1.save(MODEL_P1_PATH)
        model_p2.save(MODEL_P2_PATH)
        print("Models saved.")

def train_batch(model, target_model, batch, optimizer, loss_fn):
    states = np.array([x[0] for x in batch])
    actions = np.array([x[1] for x in batch])
    rewards = np.array([x[2] for x in batch])
    next_states = np.array([x[3] for x in batch])
    dones = tf.convert_to_tensor([float(x[4]) for x in batch])

    future_rewards = target_model.predict(next_states, verbose=0)
    updated_q_values = rewards + GAMMA * tf.reduce_max(future_rewards, axis=1)
    updated_q_values = updated_q_values * (1 - dones) - dones * 0 
    
    masks = tf.one_hot(actions, 9)

    with tf.GradientTape() as tape:
        q_values = model(states)
        q_action = tf.reduce_sum(tf.multiply(q_values, masks), axis=1)
        loss = loss_fn(updated_q_values, q_action)

    grads = tape.gradient(loss, model.trainable_variables)
    optimizer.apply_gradients(zip(grads, model.trainable_variables))
