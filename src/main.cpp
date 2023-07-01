#include <bits/stdc++.h>

using namespace std;

#define pii pair<int, int>

struct Move{
    int x, y, turn;
};

int maxDepth = 5;

pii playerMove;
bool bot2Bot = false;
int botTurn = -1, playerTurn = 1, currTurn = 0;

/// Declearation
void StartGame();
int Minimax(int board[3][3], int depth, bool isMax);
int CheckTurn(int board[3][3]);
int CheckingWinner(int board[3][3]);
int WinOrNah(int board[3][3], pii pos);
void DisplayPos(int board[3][3]);
bool IsMoveLeft(int board[3][3]);
Move BestMove(int board[3][3], int turn);
void PlayerInput();

// Center Position of center column and row
pii checkPos[5] = {{0, 1},{1, 0},{1, 1},{1, 2},{2, 1}};

int main() {

    StartGame();

    return 0;
}

// Game State
// [3 x 3] 2D array
// 0 : haven't ticked
// 1 : Cross
// -1 : Nought
void StartGame() {

    int board[3][3] ={{0, 0, 0},
                      {0, 0, 0},
                      {0, 0, 0}};

    memset(board, 0, sizeof board);

    // Game Init
    currTurn = 1;
    bool isStarted = false;

    // Pre-display
    DisplayPos(board);

    Move next_move = {0, 0, 0};

    while (!(next_move.x == -1 && next_move.y == -1)) {
        // Checking if the game is over
        if (CheckingWinner(board) == 0) {
            if (!IsMoveLeft(board))
                break;
        }
        else break;


        if (botTurn == currTurn || bot2Bot == true) {
            next_move = BestMove(board, currTurn);
            board[next_move.x][next_move.y] = next_move.turn;

            // Switch Turn
        }

        if (currTurn == playerTurn && bot2Bot == false) {
            PlayerInput();
            board[playerMove.first][playerMove.second] = playerTurn;

        }
        currTurn *= -1;

        DisplayPos(board);
    }

    // Game Ended
    cout << "\n\n==== GAME ENDED ====";
    DisplayPos(board);
}

int Minimax(int board[3][3], int depth, bool isMax) {
    int evaluate = CheckingWinner(board);

    if (evaluate == 1) {
        return 1;
    }
    if (evaluate == -1) {
        return -1;
    }

    if (!IsMoveLeft(board)) {
        return 0;
    }

    // Maximizer's Move
    if (isMax) {
        int best = INT_MIN;

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {

                if (board[i][j] != 0)
                    continue;

                board[i][j] = 1;

                best = max(best, Minimax(board, depth + 1, !isMax));

                //Undo options
                board[i][j] = 0;

            }
        }

        return best;
    }
    // Minimizer's Move
    else {
        int best = INT_MAX;

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {

                if (board[i][j] != 0)
                    continue;

                board[i][j] = -1;

                best = min(best, Minimax(board, depth + 1, !isMax));

                //Undo options
                board[i][j] = 0;

            }
        }

        return best;
    }
}

Move BestMove(int board[3][3], int turn) {

    Move best_move = {-1, -1, turn};

    int bestVal;

    if (turn == 1) {
        bestVal = INT_MIN;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {

                if (board[i][j] != 0)
                    continue;

                board[i][j] = 1;

                int moveVal = Minimax(board, 0, false);

                // Undo Move
                board[i][j] = 0;

                // Found best move
                if (moveVal > bestVal) {
                    bestVal = moveVal;
                    best_move = {i, j, turn};
                }

            }
        }
    }
    else {
        bestVal = INT_MAX;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {

                if (board[i][j] != 0)
                    continue;

                board[i][j] = -1;

                int moveVal = Minimax(board, 0, true);

                // Undo Move
                board[i][j] = 0;

                // Found best move
                if (moveVal < bestVal) {
                    bestVal = moveVal;
                    best_move = {i, j, turn};
                }
            }
        }
    }
    return best_move;
}

int CheckTurn(int board[3][3]) {
    int temp = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            temp += (board[i][j] == 1);
            temp -= (board[i][j] == -1);
        }
    }

    return (temp == 1 ) ? -1 : 1;
}

int CheckingWinner(int board[3][3]) {
    for (pii x : checkPos) {
        if (board[x.first][x.second] == 0)
            continue;

        int currentState = WinOrNah(board, x);

        // If there's a winner, return the result
        if (currentState != 0)
            return currentState;
    }

    // No one is Victorious Yet
    return 0;
}
int WinOrNah(int board[3][3], pii pos) {
    int temp = board[pos.first][pos.second];

    // If the position is in the center of the board
    if (pos.first == pos.second) {
        // First Diagonal Line (\)
        if (board[0][0] == temp && board[2][2] == temp)
            return temp;
        // Second Diagonal Line (/)
        if (board[0][2] == temp && board[2][0] == temp)
            return temp;
        // Center Collumn (|)
        if (board[0][1] == temp && board[2][1] == temp)
            return temp;
        // Center Row (--)
        if (board[1][0] == temp && board[1][2] == temp)
            return temp;
    }
    else {
        // If the position is on the center of the top and bottom row
        if (pos.first != 1) {
            if (board[pos.first][0] == temp && board[pos.first][2] == temp)
                return temp;
        }
        // If the position is on the center of the right and left column
        if (pos.second != 1) {
            if (board[0][pos.second] == temp && board[2][pos.second] == temp)
                return temp;
        }
    }

    // No one is the winner yet
    return 0;
}

void DisplayPos(int board[3][3]) {
    cout << "\n\n";

    for (int i = 0; i < 3; i++) {
        cout << "    ";
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == 0)
                cout << "-";
            else cout << ((board[i][j] == 1) ? "X" : "O");
            cout << " ";
        }
        cout << '\n';
    }
}

bool IsMoveLeft(int board[3][3]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == 0)
                return true;
        }
    }
    return false;
}

void PlayerInput() {
    cout << "\n> Make your Move : ";
    cin >> playerMove.first >> playerMove.second;
}
