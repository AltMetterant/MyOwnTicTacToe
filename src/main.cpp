#include <bits/stdc++.h>

using namespace std;

#define pii pair<int, int>

struct Move{
    int x, y, turn;
};

struct EvaluatedMove {
    Move _move;
    int evaluation;
};

/// Variables
int maxDepth = 5;

pii playerMove;
bool bot2Bot = true;
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
void PlayerInput(int board[3][3]);
void GameCommand(string arg);

void CustomWait(int seconds);

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
    system("cls");

    int board[3][3] ={{0, 0, 0},
                      {0, 0, 0},
                      {0, 0, 0}};

    memset(board, 0, sizeof board);

    // Game Init
    currTurn = 1;
    bool isStarted = false;
    string gameResultMsg;

    // Pre-display
    DisplayPos(board);

    Move next_move = {0, 0, 0};

    while (!(next_move.x == -1 && next_move.y == -1)) {
        // Checking if the game is over
        int evaluate = CheckingWinner(board);
        if (evaluate == 0) {
            if (!IsMoveLeft(board)) {
                gameResultMsg = " --== TIE! ==--";
                break;
            }
        }
        else {
            gameResultMsg = (evaluate == 1) ? " --== CROSS WINS! ==--" : " --== NOUGHT WINS ==--";

            break;
        }


        if (botTurn == currTurn || bot2Bot == true) {
            next_move = BestMove(board, currTurn);
            board[next_move.x][next_move.y] = next_move.turn;

            CustomWait(1);
        }

        if (currTurn == playerTurn && bot2Bot == false) {
            PlayerInput(board);
            board[playerMove.first][playerMove.second] = playerTurn;

        }
        // Switch Turn
        currTurn *= -1;

        system("cls");
        DisplayPos(board);
    }

    // Game Ended
    system("cls");
    cout << "\n ==== GAME ENDED ====\n";
    cout << gameResultMsg;

    DisplayPos(board);

    if (!bot2Bot)
        cin.ignore();
    string inp;

    cout << "\n> Command : ";

    getline(cin, inp);

    GameCommand(inp);
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
    Move chosen_move = best_move;

    vector<EvaluatedMove> moves;
    vector<Move> bestMoves;

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
                if (moveVal >= bestVal) {
                    bestVal = moveVal;
                    moves.push_back({{i, j, turn}, moveVal});

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
                if (moveVal <= bestVal) {
                    bestVal = moveVal;
                    moves.push_back({{i, j, turn}, moveVal});
                }
            }
        }
    }
    // Get Best Moves
    for (EvaluatedMove _move : moves) {
        if (_move.evaluation == bestVal) {
            bestMoves.push_back({_move._move});
        }
    }

    // Get a Random move from Best Moves
    int random_index = rand() % bestMoves.size();
    best_move = bestMoves[random_index];

    //cout << "Number of best moves : " << bestMoves.size() << " and chose the " << random_index << " move";

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

void PlayerInput(int board[3][3]) {
    cout << "\n> Make your Move : ";
    cin >> playerMove.first >> playerMove.second;
    // Value range: (1 - 3) (1 - 3)

    playerMove.first--;
    playerMove.second--;

    if ((playerMove.first < 0 || playerMove.first > 2) || (playerMove.second < 0 || playerMove.second > 2) ||
        board[playerMove.first][playerMove.second] != 0) {

        cout << "Your move wasn't valid!";
        CustomWait(0.5f);
        system("cls");
        DisplayPos(board);

        PlayerInput(board);
    }
}

void GameCommand(string arg) {
    if (arg == "close")
        return;
    if (arg == "rv b2b") {
        bot2Bot = !bot2Bot;
    }

    StartGame();
}

void CustomWait(int seconds) {
    std::this_thread::sleep_for(std::chrono::seconds(seconds));
}
