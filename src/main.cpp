#include <bits/stdc++.h>
#include "command.h"
#include "keyholder.h"

using namespace std;

#define pii pair<int, int>

struct Move{
    int x, y, turn;
};

struct EvaluatedMove {
    Move _move;
    int evaluation;

};
/// > Variables
// Game Settings
map<string, bool> game_booleans = {
    {"bot2Bot", true},
    {"showMoveInfo", false},
};
map<string, int> game_integers = {
    {"botTurn", -1},
    {"playerTurn", 1},
    {"maxDepth", 5}
};
map<string, int> game_floats = {
    {"betweenMoveDelaySecond", 1.0f},
    {"invalidMoveDelaySecond", 0.5f}
};

bool isStopped = false;

pii playerMove;
int currTurn = 0;

/// > Declearation
// Game Helper
void PreGame();
void StartGame();
void DisplayPos(int board[3][3]);
void PlayerInput(int board[3][3]);

// Stuffs
void CustomWait(int seconds);
void GameCommand();
void CommandHandler(vector<string> & args);

// Game Mechanics
int Minimax(int board[3][3], int depth, bool isMax);
int CheckTurn(int board[3][3]);
int CheckingWinner(int board[3][3]);
int WinOrNah(int board[3][3], pii pos);
bool IsMoveLeft(int board[3][3]);
Move BestMove(int board[3][3], int turn);


// Center Position of center columns and rows
const pii checkPos[5] = {{0, 1},{1, 0},{1, 1},{1, 2},{2, 1}};

int main() {
    // Random
    srand(time(NULL));

    // Game Command (pre-game)
    PreGame();

    GameCommand();

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


        if (game_integers[key_holder::botTurn] == currTurn || game_booleans[key_holder::bot2Bot] == true) {
            next_move = BestMove(board, currTurn);
            board[next_move.x][next_move.y] = next_move.turn;

            CustomWait(game_floats[key_holder::betweenMoveDelaySecond]);
        }

        if (currTurn == game_integers[key_holder::playerTurn] && game_booleans[key_holder::bot2Bot] == false) {
            PlayerInput(board);
            board[playerMove.first][playerMove.second] = game_integers[key_holder::playerTurn];

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

    if (!game_booleans[key_holder::bot2Bot])
        cin.ignore();

    GameCommand();
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

    if (game_booleans[key_holder::showMoveInfo]) {
    cout << "\nBest move value : " << bestVal;
    cout << "\nNumber of best moves : " << bestMoves.size() << "\nAnd chose the " << random_index + 1<< "th move";
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

void PlayerInput(int board[3][3]) {
    cout << "\n> Make your Move : ";
    cin >> playerMove.first >> playerMove.second;

    // Value range: (1 - 3) (1 - 3)
    // Decrement values by 1
    playerMove.first--;
    playerMove.second--;

    if ((playerMove.first < 0 || playerMove.first > 2) || (playerMove.second < 0 || playerMove.second > 2) ||
        board[playerMove.first][playerMove.second] != 0) {

        cout << "Your move wasn't valid!";
        CustomWait(game_floats[key_holder::betweenMoveDelaySecond]);
        system("cls");
        DisplayPos(board);

        PlayerInput(board);
    }
}

void GameCommand() {
    string inp, arg;
    vector<string> args;

    cout << '\n';
    cout << " > Command : ";

    getline(cin, inp);

    if (inp.size() != 0) {
         // Spliting the string into words
        stringstream str_stream(inp);
        while (str_stream >> arg) {
            args.push_back(arg);
        } 

        CommandHandler(args);
    }

    if (!isStopped)
        StartGame();
}

void CommandHandler(vector<string> & args) {

    switch(mett_com::commandValues[args[0]]) {
        case mett_com::close : {
            isStopped = true;
            cout << " **> Game Stopped";
            break;
        }
        case mett_com::set : {
            mett_com::Set(args, game_booleans);
            cout << game_booleans[key_holder::bot2Bot];

            break;
        }
        case mett_com::swturn : {
            mett_com::SwitchTurn(args, game_integers[key_holder::playerTurn], game_integers[key_holder::botTurn]);
        }
        default: {
            break;
        }
    } 
}


void CustomWait(int seconds) {
    std::this_thread::sleep_for(std::chrono::seconds(seconds));
}

void PreGame() {
    cout << " ====< GAME SETUPS >====\n";
    cout << " > Current Bot2Bot mode : " << (game_booleans[key_holder::bot2Bot] ? "true" : "false") << '\n';
    cout << " > Player Move : " << ((game_integers[key_holder::playerTurn] == 1) ? "X" : "O") << '\n';
    cout << " > Bot Move : " << ((game_integers[key_holder::botTurn] == 1) ? "X" : "O") << '\n';

    cout << "\n -?- Move Format : <row:integer[1 - 3]> <column:integer[1 - 3]>"; 

    cout << "\n\n -?- Leave command blank to start the Game!\n";
}