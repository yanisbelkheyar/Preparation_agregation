#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>

// Should really be a #define, but forbidden by agreg.
// Just using a const variable causes warnings from clangs, as using a variable for an array size makes that array
// variable-sized (even if the variable is const, contrary to C++), and there are lots of places that a variable-sized
// array is illegal.
enum {
    NUM_ROWS = 6,
    NUM_COLS = 7
};

typedef enum {
  P1,
  P2,
  Empty
} cell_t;
const char char_p1 = 'X';
const char char_p2 = 'O';
const char char_empty = '_';
const char cell_to_char[3] = {char_p1, char_p2, char_empty};

typedef enum {
    Draw,
    P1_Win,
    P2_Win,
    Ongoing
} game_state_t;

typedef enum {
    Player1 = 0,
    Player2 = 1
} player_t;
const char player_to_char[2] = {char_p1, char_p2};
const cell_t player_to_cell[2] = {P1, P2};
const game_state_t player_to_win[2] = {P1_Win, P2_Win};

unsigned compute_num_in_dir(cell_t state[NUM_COLS][NUM_ROWS], int row, int col, int row_change, int col_change) {
    assert(state[col][row] != Empty);

    unsigned num_aligned = 0;
    for (unsigned i = 1; i < 4; ++i) {
        const int new_row = row + i*row_change;
        if (new_row < 0 || new_row > NUM_ROWS) {break;}
        const int new_col = col + i*col_change;
        if (new_col < 0 || new_col > NUM_COLS) {break;}
        if (state[col][row] != state[new_col][new_row]) {break;}
        ++num_aligned;
    }
    return num_aligned;
}

bool check_alignement_along_dir(cell_t state[NUM_COLS][NUM_ROWS], int row, int col, int row_change, int col_change) {
    assert(state[col][row] != Empty);

    const unsigned num_aligned = 1 + compute_num_in_dir(state, row, col, row_change, col_change) + compute_num_in_dir(state, row, col, -row_change, -col_change);

    return num_aligned >= 4;
}

bool is_winning_move(cell_t state[NUM_COLS][NUM_ROWS], int row, int col) {
    assert(state[col][row] != Empty);

    return check_alignement_along_dir(state, row, col, 1, 0)
        || check_alignement_along_dir(state, row, col, 0, 1)
        || check_alignement_along_dir(state, row, col, 1, 1)
        || check_alignement_along_dir(state, row, col, 1, -1);
}

void print_state(cell_t state[NUM_COLS][NUM_ROWS]) {
    printf("\n\t1234567\n\n");
    assert(NUM_COLS == 7);
    for (unsigned i = 0; i < NUM_ROWS; ++i) {
        // I count from the bottom to the top, so I must reverse for the printing
        unsigned row = NUM_ROWS - i - 1;
        printf("\t");
        for (unsigned col = 0; col < NUM_COLS; ++col) {
            printf("%c", cell_to_char[state[col][row]]);
        }
        printf("\n");
    }
    printf("\n");
}

bool is_valid_col(cell_t state[NUM_COLS][NUM_ROWS], int col) {
    return (col >= 0) && (col < NUM_COLS) && (state[col][NUM_ROWS - 1] == Empty);
}

// Reads an input from the user with scanf, if it is a valid column stores it in result and return true, otherwise just return false
bool read_valid_col(cell_t state[NUM_COLS][NUM_ROWS], int *result) {
    int col;
    int matched = scanf("%d", &col);
    // handle the case where the user did not enter a number at all.
    // unfortunately, scanf leaves stdin exactly as is in that case, and so we would loop forever without that ugly getchar loop.
    if (matched != 1) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF)
            ;
        return false;
    }
    col -= 1; // Columns are numbered starting from 1 in the UI
    if (!is_valid_col(state, col)) {
        return false;
    }
    *result = col;
    return true;
}

game_state_t play_round(cell_t state[NUM_COLS][NUM_ROWS], player_t player, const char *player_name) {
    int col;
    do {
        print_state(state);
        printf("%s (%c) : quel est le numero de la colonne\ndans laquelle vous souhaitez jouer un jeton ?", player_name, player_to_char[player]);
    } while (!read_valid_col(state, &col));
    assert(is_valid_col(state, col));
    
    int row;
    for(unsigned i = 0; ; ++i) {
        if (state[col][i] == Empty) {
            state[col][i] = player_to_cell[player];
            row = i;
            break;
        }
    }
    assert(row < NUM_ROWS);

    if (is_winning_move(state, row, col)) {
        return player_to_win[player];
    }
    return Ongoing;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Expected 2 arguments for the names of the two players\n");
        return 1;
    }

    const char *p1_name = argv[1];
    const char *p2_name = argv[2];
    printf("*** Bienvenue pour une nouvelle partie de PUISSANCE 4 : %s vs %s ! ***", p1_name, p2_name);

    unsigned num_tokens_played = 0;
    game_state_t game_state = Ongoing;
    player_t next_player = Player1;

    cell_t state[NUM_COLS][NUM_ROWS];
    for (unsigned col = 0; col < NUM_COLS; ++col) {
        for (unsigned row = 0; row < NUM_ROWS; ++row) {
            state[col][row] = Empty;
        }
    }

    while (game_state == Ongoing) {
        const char * player_name = next_player == Player1 ? p1_name : p2_name;
        game_state = play_round(state, next_player, player_name);
        ++num_tokens_played;
        if ((game_state == Ongoing) && (num_tokens_played == NUM_COLS * NUM_ROWS)) {
            game_state = Draw;
        }
        next_player ^= 1;
    }

    switch (game_state) {
        case P1_Win:
            printf("\n\t VICTOIRE de %s\n\n", p1_name);
            break;
        case P2_Win:
            printf("\n\t VICTOIRE de %s\n\n", p2_name);
            break;
        case Draw:
            printf("\n\t Match nul\n\n");
            break;
        case Ongoing:
            assert(false);
    }

    return 0;
}