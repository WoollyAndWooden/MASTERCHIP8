# Pong Training Documentation

Documentation for the Reinforcement Learning agent trained to play Pong.

## Environment

*   **Game**: Pong (1 Player).
*   **State**: 64x32 pixel grid (flattened or CNN input).
*   **Actions**: Up, Down, None.
*   **Reward Function**:
    *   +1 for hitting the ball.
    *   -1 for missing the ball.

## Flowcharts

### Reward Calculation
*(Placeholder: Insert Flowchart of Reward Logic here)*
1.  Read Memory Address `0x...` (Score).
2.  Compare with previous score.
3.  If increased, return +1.
