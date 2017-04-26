# Battleship

This program plays a single-player version of the game Battleship.

Strategies I used:

- A global 2D vector is created to store the spots that are hit, miss, hit and sunk, and impossible to shoot at by marking them with the hit character.
- The game starts by shooting at a random unexplored spot.
- If the random unexplored spot is a HIT_N_SUNK, the surrounding squares are marked as misses in the vector because they are impossible to be where the other ships are located.
- If there was a miss, a random unexplored possible spot is shot at (i.e. not the surrounding squares of the entire HIT_N_SUNK ship).
- When a ship is hit, that location is marked as a hit in the vector and is remembered by using 2 global int variables (previous_r and previous_c) and the coordinates of that hit is saved.
- When a ship is hit, the squares surrounding that hit is shot at the next turns.
- If a ship is hit and the square below it is also a hit, it continually shoots downward until there is a miss or HIT_N_SUNK. If there is a miss, it continually shoots upward until there is a HIT_N_SUNK.
- If a ship is hit and the square to the right of it is also a hit, it continually shoots to the right until there is a miss or HIT_N_SUNK. If there is a miss, it continually shoots to the left until there is a HIT_N_SUNK.
- When an entire ship is HIT_N_SUNK, it is marked in the vector that the surrounding squares are misses because they are impossible to be where the other ships are located.
