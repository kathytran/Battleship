#include <cstdlib>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include "bot.h"
#include "screen.h"

using std::endl;
using std::ostream;
using std::vector;
using std::cout;

int ROWS, COLS;
bool up, down, left, right, vert_down, vert_up, horiz_right, horiz_left;
int previous_r, previous_c, vert_down_r, vert_up_r, horiz_right_c, horiz_left_c;
vector < vector <char> > my_board;

/* Initialization procedure, called when the game starts:
   init (rows, cols, num, screen, log)
   Arguments:
    rows, cols = the boards size
    num        = the number of ships
    screen     = a screen to update your knowledge about the game
    log        = a cout-like output stream
*/


void init(int rows, int cols, int num, Screen &screen, ostream &log) {
  ROWS = rows;
  COLS = cols;
  my_board = vector < vector<char> >(ROWS, vector<char>(COLS, '-'));    // 2D vector to store hits and misses
  up = false;
  down = false;
  left = false;
  right = false;
  vert_down = false;
  vert_up = false;
  horiz_right = false;
  horiz_left = false;
  log << "Start." << endl;
}

/* The procedure handling each turn of the game:
   next_turn(sml, lrg, num, gun, screen, log)
   Arguments:
    sml, lrg = the sizes of the smallest and the largest ships that are currently alive
    num      = the number of ships that are currently alive
    gun      = a gun.
               Call gun.shoot(row, col) to shoot:
                  Can be shot only once per turn.
                  Returns MISS, HIT, HIT_N_SUNK, ALREADY_HIT, or ALREADY_SHOT.
    screen   = a screen to update your knowledge about the game
    log      = a cout-like output stream
*/

void next_turn(int sml, int lrg, int num, Gun &gun, Screen &screen, ostream &log) {
  result res;

  // if a ship was hit in the previous turn, the spot below the hit is shot at
  if (down == true) {
    if ((previous_r+1) < ROWS && (previous_r+1) >= 0 && my_board[previous_r+1][previous_c] == '-') {   // ensures that shots aren't outside of the board
      res = gun.shoot(previous_r+1, previous_c);        // if next target hasn't been shot, it is shot at
      log << "Smallest: " << sml << " Largest: " << lrg << ". ";
      log << "Shoot at " << previous_r+1 << " " << previous_c << endl;

     if (res == MISS) {
       screen.mark(previous_r+1, previous_c, 'x', BLUE);
       my_board[previous_r+1][previous_c] = 'x';
       up = true;
       down = false;
       return;
      }
     else if (res == HIT) {
       screen.mark(previous_r+1, previous_c, '@', GREEN);
       my_board[previous_r+1][previous_c] = '@';
       // marks surrounding squares as misses
       if ((previous_c+1) >= 0 && (previous_c+1) < COLS) {
        my_board[previous_r+1][previous_c+1] = 'x';
        my_board[previous_r][previous_c+1] = 'x';
      }
       if ((previous_c-1) >= 0 && (previous_c-1) < COLS) {
        my_board[previous_r+1][previous_c-1] = 'x';
        my_board[previous_r][previous_c-1] = 'x';
      }
       vert_down_r = previous_r + 1;
       vert_down = true;     // continues to attempt to hit the ship down if there is a hit
       down = false;
       return;
     }
     else if (res == HIT_N_SUNK) {
       screen.mark(previous_r+1, previous_c, 'S', RED);
       my_board[previous_r+1][previous_c] = 'S';
       // marks surrounding squares as misses
       if ((previous_c+1) >= 0 && (previous_c+1) < COLS) {
        my_board[previous_r+1][previous_c+1] = 'x';
        my_board[previous_r][previous_c+1] = 'x';
      }
       if ((previous_c-1) >= 0 && (previous_c-1) < COLS) {
        my_board[previous_r+1][previous_c-1] = 'x';
        my_board[previous_r][previous_c-1] = 'x';
      }
      if ((previous_r-1) >= 0 && (previous_r-1) < ROWS)
       my_board[previous_r-1][previous_c] = 'x';
      if ((previous_r+2) >= 0 && (previous_r+2) < ROWS)
       my_board[previous_r+2][previous_c] = 'x';

       down = false;
       return;
      }
   }
   else {
     up = true;
     down = false;
  }
 }

  // if a ship was hit in the previous turn, the spot above the hit is shot at
  if (up == true) {
     if ((previous_r-1) >= 0 && (previous_r-1) < ROWS && my_board[previous_r-1][previous_c]  == '-') {      // ensures that shots aren't outside of the board
       log << "Smallest: " << sml << " Largest: " << lrg << ". ";     // if next target hasn't been shot, it is shot at
       log << "Shoot at " << previous_r-1 << " " << previous_c << endl;
       res = gun.shoot(previous_r-1, previous_c);

      if (res == MISS) {
        screen.mark(previous_r-1, previous_c, 'x', BLUE);
        my_board[previous_r-1][previous_c] = 'x';
        right = true;
        up = false;
        return;
       }
      else if (res == HIT) {
        screen.mark(previous_r-1, previous_c, '@', GREEN);
        my_board[previous_r-1][previous_c] = '@';
        // marks surrounding squares as misses
        if ((previous_c+1) >= 0 && (previous_c+1) < COLS) {
         my_board[previous_r-1][previous_c+1] = 'x';
         my_board[previous_r][previous_c+1] = 'x';
       }
        if ((previous_c-1) >= 0 && (previous_c-1) < COLS) {
         my_board[previous_r-1][previous_c-1] = 'x';
         my_board[previous_r][previous_c-1] = 'x';
       }
        vert_up_r = previous_r - 1;
        vert_up = true;     // continues to attempt to hit the ship up if there is a hit
        up = false;
        return;
      }
      else if (res == HIT_N_SUNK) {
        screen.mark(previous_r-1, previous_c, 'S', RED);
        my_board[previous_r-1][previous_c] = 'S';
        // marks surrounding squares as misses
        if ((previous_c+1) >= 0 && (previous_c+1) < COLS) {
         my_board[previous_r-1][previous_c+1] = 'x';
         my_board[previous_r][previous_c+1] = 'x';
       }
        if ((previous_c-1) >= 0 && (previous_c-1) < COLS) {
         my_board[previous_r-1][previous_c-1] = 'x';
         my_board[previous_r][previous_c-1] = 'x';
       }
       if ((previous_r-2) >= 0 && (previous_r-2) < ROWS)
        my_board[previous_r-2][previous_c] = 'x';

        up = false;
        return;
       }
    }
    else {
    right = true;
    up = false;
    }
  }

  // if a ship was hit in the previous turn, the spot to the right of the hit is shot at
  if (right == true) {
     if ((previous_c+1) < COLS && (previous_c+1) >= 0 && my_board[previous_r][previous_c+1] == '-') {   // ensures that shots aren't outside of the board
       log << "Smallest: " << sml << " Largest: " << lrg << ". ";    // if next target hasn't been shot, it is shot at
       log << "Shoot at " << previous_r << " " << previous_c+1 << endl;
       res = gun.shoot(previous_r, previous_c+1);

      if (res == MISS) {
        screen.mark(previous_r, previous_c+1, 'x', BLUE);
        my_board[previous_r][previous_c+1] = 'x';
        left = true;
        right = false;
        return;
       }
      else if (res == HIT) {
        screen.mark(previous_r, previous_c+1, '@', GREEN);
        my_board[previous_r][previous_c+1] = '@';
        // marks surrounding squares as misses
        if ((previous_r+1) >= 0 && (previous_r+1) < ROWS) {
         my_board[previous_r+1][previous_c+1] = 'x';
       }
        if ((previous_r-1) >= 0 && (previous_r-1) < ROWS) {
         my_board[previous_r-1][previous_c+1] = 'x';
       }
        horiz_right_c = previous_c + 1;
        horiz_right = true;     // continues to attempt to hit the ship to the right if there is a hit
        right = false;
        return;
      }
      else if (res == HIT_N_SUNK) {
        screen.mark(previous_r, previous_c+1, 'S', RED);
        my_board[previous_r][previous_c+1] = 'S';
        // marks surrounding squares as misses
        if ((previous_r+1) >= 0 && (previous_r+1) < ROWS) {
         my_board[previous_r+1][previous_c+1] = 'x';
       }
        if ((previous_r-1) >= 0 && (previous_r-1) < ROWS) {
         my_board[previous_r-1][previous_c+1] = 'x';
       }
       if ((previous_c+2) >= 0 && (previous_c+2) < COLS)
        my_board[previous_r][previous_c+2] = 'x';
       if ((previous_c-1) >= 0 && (previous_c-1) < COLS)
        my_board[previous_r][previous_c-1] = 'x';

        right = false;
        return;
       }
    }
    else {
    left = true;
    right = false;
    }
  }

  // if a ship was hit in the previous turn, the spot to the left of the hit is shot at
  if (left == true) {
     if ((previous_c-1) >= 0 && (previous_c-1) < COLS && my_board[previous_r][previous_c-1] == '-') {  // ensures that shots aren't outside of the board
       log << "Smallest: " << sml << " Largest: " << lrg << ". ";    // if next target hasn't been shot, it is shot at
       log << "Shoot at " << previous_r << " " << previous_c-1 << endl;
       res = gun.shoot(previous_r, previous_c-1);

      if (res == MISS) {
        screen.mark(previous_r, previous_c-1, 'x', BLUE);
        my_board[previous_r][previous_c-1] = 'x';
       }
      else if (res == HIT) {
        screen.mark(previous_r, previous_c-1, '@', GREEN);
        my_board[previous_r][previous_c-1] = '@';
        // marks surrounding squares as misses
        if ((previous_r+1) >= 0 && (previous_r+1) < ROWS) {
         my_board[previous_r+1][previous_c-1] = 'x';
       }
        if ((previous_r-1) >= 0 && (previous_r-1) < ROWS) {
         my_board[previous_r-1][previous_c-1] = 'x';
       }
        horiz_left_c = previous_c - 1;
        horiz_left = true;    // continues to attempt to hit the ship to the left if there is a hit
        left = false;
        return;
      }
      else if (res == HIT_N_SUNK) {
        screen.mark(previous_r, previous_c-1, 'S', RED);
        my_board[previous_r][previous_c-1] = 'S';
        // marks surrounding squares as misses
        if ((previous_r+1) >= 0 && (previous_r+1) < ROWS) {
         my_board[previous_r+1][previous_c-1] = 'x';
       }
        if ((previous_r-1) >= 0 && (previous_r-1) < ROWS) {
         my_board[previous_r-1][previous_c-1] = 'x';
       }
        if ((previous_c-2) >= 0 && (previous_c-2) < COLS)
         my_board[previous_r][previous_c-2] = 'x';
    }
      left = false;
      return;
    }
    else {
      left = false;
    }
  }

 // continually shoots down vertically until there is a miss or HIT_N_SUNK
 if (vert_down == true) {
   if ((vert_down_r+1) >= 0 && (vert_down_r+1) < ROWS && my_board[vert_down_r+1][previous_c]  == '-') {
     log << "Smallest: " << sml << " Largest: " << lrg << ". ";     // if next target hasn't been shot, it is shot at
     log << "Shoot at " << vert_down_r+1 << " " << previous_c << endl;
     res = gun.shoot(vert_down_r+1, previous_c);

    if (res == MISS) {
      screen.mark(vert_down_r+1, previous_c, 'x', BLUE);
      my_board[vert_down_r+1][previous_c] = 'x';
      up = true;         // attempts to continually hit the ship up vertically if there is a miss when shooting down
      vert_down = false;
      return;
     }
    else if (res == HIT) {
      screen.mark(vert_down_r+1, previous_c, '@', GREEN);
      my_board[vert_down_r+1][previous_c] = '@';
      // marks surrounding squares as misses
      if ((previous_c+1) >= 0 && (previous_c+1) < COLS) {
       my_board[vert_down_r+1][previous_c+1] = 'x';
     }
      if ((previous_c-1) >= 0 && (previous_c-1) < COLS) {
       my_board[vert_down_r+1][previous_c-1] = 'x';
     }
      vert_down_r = vert_down_r + 1;
      vert_down = true;
      return;
    }
    else if (res == HIT_N_SUNK) {         // When HIT_N_SUNK an entire ship, the surrounding squares are marked as MISS in the vector
      screen.mark(vert_down_r+1, previous_c, 'S', RED);
      my_board[vert_down_r+1][previous_c] = 'S';
      // marks surrounding squares as misses
      if ((previous_c+1) >= 0 && (previous_c+1) < COLS) {
       my_board[vert_down_r+1][previous_c+1] = 'x';
     }
      if ((previous_c-1) >= 0 && (previous_c-1) < COLS) {
       my_board[vert_down_r+1][previous_c-1] = 'x';
     }
     if ((vert_down_r+2) >= 0 && (vert_down_r+2) < ROWS)
      my_board[vert_down_r+2][previous_c] = 'x';

      vert_down = false;
      return;
     }
   }
   else {
     up = true;
     vert_down = false;
     return;
   }
 }

 // continually shoots to the up vertically until there is a miss or HIT_N_SUNK
 if (vert_up == true) {
   if ((vert_up_r-1) >= 0 && (vert_up_r-1) < ROWS && my_board[vert_up_r-1][previous_c]  == '-') {
     log << "Smallest: " << sml << " Largest: " << lrg << ". ";     // if next target hasn't been shot, it is shot at
     log << "Shoot at " << vert_up_r-1 << " " << previous_c << endl;
     res = gun.shoot(vert_up_r-1, previous_c);

    if (res == MISS) {
      screen.mark(vert_up_r-1, previous_c, 'x', BLUE);
      my_board[vert_up_r-1][previous_c] = 'x';
      vert_up = false;
      return;
     }
    else if (res == HIT) {
      screen.mark(vert_up_r-1, previous_c, '@', GREEN);
      my_board[vert_up_r-1][previous_c] = '@';
      // marks surrounding squares as misses
      if ((previous_c+1) >= 0 && (previous_c+1) < COLS) {
       my_board[vert_up_r-1][previous_c+1] = 'x';
     }
      if ((previous_c-1) >= 0 && (previous_c-1) < COLS) {
       my_board[vert_up_r-1][previous_c-1] = 'x';
     }
      vert_up_r = vert_up_r - 1;
      vert_up = true;
      return;
    }
    else if (res == HIT_N_SUNK) {           // When HIT_N_SUNK an entire ship, the surrounding squares are marked as MISS in the vector
      screen.mark(vert_up_r-1, previous_c, 'S', RED);
      my_board[vert_up_r-1][previous_c] = 'S';
      // marks surrounding squares as misses
      if ((previous_c+1) >= 0 && (previous_c+1) < COLS) {
       my_board[vert_up_r-1][previous_c+1] = 'x';
     }
      if ((previous_c-1) >= 0 && (previous_c-1) < COLS) {
       my_board[vert_up_r-1][previous_c-1] = 'x';
     }
     if ((vert_up_r-2) >= 0 && (vert_up_r-2) < ROWS)
      my_board[vert_up_r-2][previous_c] = 'x';

      vert_up = false;
      return;
     }
   }
   vert_up = false;
   return;
 }

 // continually shoots to the right horizontally until there is a miss or HIT_N_SUNK
if (horiz_right == true) {
  if ((horiz_right_c+1) >= 0 && (horiz_right_c+1) < COLS && my_board[previous_r][horiz_right_c+1]  == '-') {
    log << "Smallest: " << sml << " Largest: " << lrg << ". ";     // if next target hasn't been shot, it is shot at
    log << "Shoot at " << previous_r << " " << horiz_right_c+1 << endl;
    res = gun.shoot(previous_r, horiz_right_c+1);

   if (res == MISS) {
     screen.mark(previous_r, horiz_right_c+1, 'x', BLUE);
     my_board[previous_r][horiz_right_c+1] = 'x';
     left = true;       // attempts to continually hit the ship to the left horizontally if there is a miss
     horiz_right = false;
     return;
    }
   else if (res == HIT) {
     screen.mark(previous_r, horiz_right_c+1, '@', GREEN);
     my_board[previous_r][horiz_right_c+1] = '@';
     // marks surrounding squares as misses
     if ((previous_r+1) >= 0 && (previous_r+1) < ROWS) {
      my_board[previous_r+1][horiz_right_c+1] = 'x';
    }
     if ((previous_r-1) >= 0 && (previous_r-1) < ROWS) {
      my_board[previous_r-1][horiz_right_c+1] = 'x';
    }

     horiz_right_c = horiz_right_c + 1;
     horiz_right = true;
     return;
   }
   else if (res == HIT_N_SUNK) {          // When HIT_N_SUNK an entire ship, the surrounding squares are marked as MISS in my vector
     screen.mark(previous_r, horiz_right_c+1, 'S', RED);
     my_board[previous_r][horiz_right_c+1] = 'S';
     // marks surrounding squares as misses
     if ((previous_r+1) >= 0 && (previous_r+1) < ROWS) {
      my_board[previous_r+1][horiz_right_c+1] = 'x';
    }
     if ((previous_r-1) >= 0 && (previous_r-1) < ROWS) {
      my_board[previous_r-1][horiz_right_c+1] = 'x';
    }
    if ((horiz_right_c+2) >= 0 && (horiz_right_c+2) < COLS)
     my_board[previous_r][horiz_right_c+2] = 'x';

     horiz_right = false;
     return;
    }
  }
  horiz_right = false;
  return;
}

// continually shoots up vertically until there is a miss or HIT_N_SUNK
if (horiz_left == true) {
  if ((horiz_left_c-1) >= 0 && (horiz_left_c-1) < COLS && my_board[previous_r][horiz_left_c-1]  == '-') {
    log << "Smallest: " << sml << " Largest: " << lrg << ". ";     // if next target hasn't been shot, it is shot at
    log << "Shoot at " << previous_c << " " << horiz_left_c-1 << endl;
    res = gun.shoot(previous_r, horiz_left_c-1);

   if (res == MISS) {
     screen.mark(previous_r, horiz_left_c-1, 'x', BLUE);
     my_board[previous_r][horiz_left_c-1] = 'x';
     horiz_left = false;
    }
   else if (res == HIT) {
     screen.mark(previous_r, horiz_left_c-1, '@', GREEN);
     my_board[previous_r][horiz_left_c-1] = '@';
     // marks surrounding squares as misses
     if ((previous_r+1) >= 0 && (previous_r+1) < ROWS) {
      my_board[previous_r+1][horiz_left_c-1] = 'x';
    }
     if ((previous_r-1) >= 0 && (previous_r-1) < ROWS) {
      my_board[previous_r-1][horiz_left_c-1] = 'x';
    }
     horiz_left_c = horiz_left_c - 1;
     horiz_left = true;
     return;
   }
   else if (res == HIT_N_SUNK) {        // When HIT_N_SUNK an entire ship, the surrounding squares are marked as MISS in my vector
     screen.mark(previous_r, horiz_left_c-1, 'S', RED);
     my_board[previous_r][horiz_left_c-1] = 'S';
     // marks surrounding squares as misses
     if ((previous_r+1) >= 0 && (previous_r+1) < ROWS) {
      my_board[previous_r+1][horiz_left_c-1] = 'x';
    }
     if ((previous_r-1) >= 0 && (previous_r-1) < ROWS) {
      my_board[previous_r-1][horiz_left_c-1] = 'x';
    }
    if ((horiz_left_c-2) >= 0 && (previous_c-2) < COLS)
     my_board[previous_r][horiz_left_c-2] = 'x';

     horiz_left = false;
     return;
    }
  }
  horiz_left = false;
  return;
}

  // shoots randomly at an unexplored spot for the first round or if there was a miss the previous turn
  int r, c;
  srand(time(NULL));
  if (up == false && down == false && left == false && right == false && vert_down == false && vert_up == false && horiz_right == false &&  horiz_left == false) {
    do {         // ensures that an unexplored spot is shot at
      r = rand() % ROWS;
      c = rand() % COLS;
    } while (r < 0 || r > (ROWS-1) || c > (COLS-1) || c < 0 || my_board[r][c] != '-');

    log << "Smallest: " << sml << " Largest: " << lrg << ". ";
    log << "Shoot at " << r << " " << c << endl;
    res = gun.shoot(r,c);

    if (res == MISS) {
      screen.mark(r, c, 'x', BLUE);
      my_board[r][c] = 'x';
     }
    else if (res == HIT) {
      screen.mark(r, c, '@', GREEN);
      previous_r = r;
      previous_c = c;
      down = true;    // if there was a hit, the spot below the hit is shot at
      my_board[r][c] = '@';
    }

    else if (res == HIT_N_SUNK) {
      screen.mark(r, c, 'S', RED);
      my_board[r][c] = 'S';
      // squares surrounding HIT_N_SUNK are marked as a miss
      if ((r-1) >= 0 && (r-1) < ROWS)
        my_board[r-1][c] = 'x';
      if ((r+1) >= 0 && (r+1) < ROWS)
        my_board[r+1][c] = 'x';
      if ((c-1) >= 0 && (c-1) < COLS)
        my_board[r][c-1] = 'x';
      if ((c+1) >= 0 && (c+1) < COLS)
        my_board[r][c+1] = 'x';
     }
   return;
 }
}
