//
// Created by nathnael on 5/27/2017.
//

#ifndef MINIMAX_PLAY_GAME_H
#define MINIMAX_PLAY_GAME_H



#include <iostream>
#include <cstdlib>
#include <chrono>
#include <ctime>

using namespace std;

// Machine player always 1
#define MACHINE_PLAYER 1

// Board Size
#define DIM_MAX 7

#define MAX 1000
#define MIN -1000

// Board
int board[DIM_MAX][DIM_MAX];
// Dimensions
int d;

// Max depth
int max_depth;

// Machine Player best move
int best_row;
int best_col;

// For gridding system
char alphabets[26];

void init();
void draw();
int checkForWinner(int b[DIM_MAX][DIM_MAX]);
void readPlayerInput(int &r, int &c);
int minimax(int hypothetical_board[DIM_MAX][DIM_MAX], int player, bool myMove, int depth, int alpha, int beta);


int playGame(int board_size, int level) {
    int current_player = -1;
    int winner, row = -1, col = -1, score;
    int total_moves = 0;
    d = board_size;
    max_depth = level;
    for (int index=0; index<26; index++) {
        alphabets[index] = index + 'a';
    }
    init();
    while (true) {
        system("clear");
        draw();
        // check for winner
        winner = checkForWinner(board);
        if (winner != 0) {
            if (winner == -1) {
                cout << "You have won the game. " << endl << endl;
                return 1;
            }
            else {
                cout << "You have lost the game. " << endl << endl;
                return -1;
            }
        }
        if (total_moves == (d * d)) {
            cout << "This game has ended in a tie. " << endl << endl;
            break;
        }
        if (current_player == MACHINE_PLAYER) {
//            Counting time elapsed for minimax
//            auto start = chrono::high_resolution_clock::now();
            score = minimax(board, current_player, true, 0, MIN, MAX);
//            auto finish = chrono::high_resolution_clock::now();
//            chrono::duration<double>elapsed = finish - start;
//            cout<<elapsed.count()<<endl;
//            getch();
            if (score != MIN) {
                row = best_row;
                col = best_col;
            }
        } else {
            readPlayerInput(row, col);
        }
        if ((row >= 0 && row < d) && (col >= 0 && col < d)) {
            // If the row and position is not taken
            if (board[row][col] == 0) {
                // set player position in board
                board[row][col] = current_player;
                // change turn
                current_player = -1 * current_player;
                // add 1 to total positions occupied
                total_moves++;
            }
        }
    }
    return 0;
}


int minimax(int hypothetical_board[DIM_MAX][DIM_MAX], int player, bool maximizingPlayer, int depth, int alpha, int beta) {
    /*
     * Minimax function which recursively calls itself to
     * retrieve the optimal position for the next move.
     * It returns 0 if there maximum recursion depth is exceeded
     * or of there are no possible moves left.
     * The arguments alpha and beta denote the best possible score
     * for the maximizer and minimizer along the path to the root.
     */

    int score;
    // Based on the depth setting return
    if (depth > max_depth) return 0;

    // Check if there's winner for every recursive call
    int winner = checkForWinner(hypothetical_board);
    if (winner != 0) return winner;

    int move_row = -1, move_col = -1; // Helper when board is full

    // Start from worst possible score for players
    if (maximizingPlayer) {
        score = MIN;
    } else {
        score = MAX;
    }

    // Randomize iteration start
    time_t now = time(0);
    srand(now);

    int b, c;
    rand()%2 == 0 ? b = 0: b = d;
    if (rand()%2 == 0) {
        b = 0;
    }
    else {
        b = d;
    }
    rand()%2 == 0 ? c = 0: c = d;

    // Randomized effects
    // For all possible moves
    int i;
    for (c == 0 ? i = 0 : i = c-1; c == 0 ? i < d : i >= 0; c == 0 ? i++ : i--) {
        int j;
        for (b == 0 ? j = 0 : j = b-1; b == 0 ? j < d : j >= 0; b == 0 ? j++ : j--) {
            // if we can move to that position
            if (hypothetical_board[i][j] == 0) {
                hypothetical_board[i][j] = player;
                // Recursively call minimax with updated alpha and beta values
                int thisScore = minimax(hypothetical_board, -1 * player, !maximizingPlayer, depth + 1, alpha, beta);
                if (maximizingPlayer) {
                    // if it's the computers move maximize the score
                    // (take the one that is the highest)
                    if (thisScore > score) {
                        score = thisScore;
                        move_row = i;
                        move_col = j;
                    } // If the score is better than alpha
                    if (score > alpha) {
                        // Update best possible move for max (alpha)
                        alpha = score;
                    }
                    // If alpha is greater than or equal to
                    // beta reset board and return beta
                    // to the next player (minimizer)
                    if (alpha >= beta) {
                        hypothetical_board[i][j] = 0;
                        return beta;
                    }
                } else {
                    // if it's the human players move minimize the
                    // score (take the one that is the lowest of the utilities)
                    if (thisScore < score) {
                        score = thisScore;
                        move_row = i;
                        move_col = j;
                    }
                    // If the score is better than beta (for the minimizer)
                    if (score < beta) {
                        beta = score;
                    }
                    // If alpha is greater than or equal to
                    // beta reset board and return alpha
                    // to the next player (maximizer)
                    if (beta <= alpha) {
                        hypothetical_board[i][j] = 0;
                        return alpha;
                    }
                }
                hypothetical_board[i][j] = 0; // Reset the board after the try
            }
        }
    }
    if (move_row == -1) return 0; // no valid moves left
    // Set best_row and best_col with
    // values determined to be the best at every depth
    best_row = move_row;
    best_col = move_col;
    return score;
}

void readPlayerInput(int& r, int& c) {
    // Prompt for move
    string location;
    input:
    cout << "Enter a move for player X (for example, c2): ";
    cin >> location;
    if (location.size() > 2 || location[0] > alphabets[d-1] || (location[1] - '0') > d-1) {
        cout<<"Enter a valid input."<<endl;
        goto input;
    }
    // Take the first letter and use it to set the row
    r = tolower(location[0]) - 'a';
    // Take the column letter and use it to set the column
    c = location[1] - '0';
}

void init() {
    /* Initializes the board.
     * Clears all of its contents each time
     * we play.
     * */
    for (int i = 0; i < DIM_MAX; i++) {
        for (int j = 0; j < DIM_MAX; j++) {
            board[i][j] = 0;
        }
    }
    // For machine player values which are very
    // small (close to infinity).
    // Starting points from which we need to improve.
    best_row = -1000;
    best_col = -1000;
}

void drawHL(){
    /*
     * Draws a horizontal line based on
     * the size if the board
     */
    cout << "   " ;
    for (int i = 0; i<d; i++) cout<<"--  ";
    cout <<endl;
}

void draw() {
    /*
     * Draws the globally declared board
     * at its current state. Replaces player
     * values with X and O and puts spaces
     * in available positions
     */
    cout << "    " ;
    for (int i = 0; i<d; i++) cout<<i << "   ";
    cout <<endl;
    drawHL();
    for (int j = 0; j < d; j++) {
        cout << alphabets[j] << " | ";
        for (int i = 0; i < d; i++) {
            if (board[j][i] == -1) cout << "X | ";
            else if (board[j][i] == 1) cout << "O | ";
            else cout << "  | ";
        }
        cout<<endl;
        drawHL();
    }
}


int checkForWinner(int b[DIM_MAX][DIM_MAX]) {
    /* This function accepts a board of
     * size DIM_MAX x DIM_MAX and returns the
     * winner -1 or 1 for human player and
     * machine player resepectively and
     * 0 if there is no winner
     * */
    int prev, winner;
    for (int i = 0; i < d; i++) {
        prev = b[i][0]; // Check the rows
        winner = prev;
        for (int j = 1; j < d; j++) {
            if (prev != b[i][j]) {
                winner = 0;
                break;
            }
        }
        if (winner != 0) return winner;
        // Check the column
        prev = b[0][i];
        winner = prev;
        for (int j = 1; j < d; j++) {
            if (prev != b[j][i]) {
                winner = 0;
                break;
            }
        }
        if (winner != 0) return winner;
    }

    // Check the diagonals
    // From left to right
    prev = b[0][0];
    winner = prev;
    for (int j = 1; j < d; j++) {
        if (prev != b[j][j]) {
            winner = 0;
            break;
        }
    }
    if (winner != 0) return winner;
    // From right to left
    prev = b[0][d - 1];
    winner = prev;
    for (int j = 1; j < d; j++) {
        if (prev != b[j][d - 1 - j]) {
            winner = 0;
            break;
        }
    }
    if (winner != 0) return winner;
    return 0;
}


#endif //MINIMAX_PLAY_GAME_H
