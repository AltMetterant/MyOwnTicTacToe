#include <bits/stdc++.h>

using namespace std;

#define pii pair<int, int>

/// Declearation
void StartGame();
void Minimax(int gameState[3][3]);
string CheckTurn(int gameState[3][3]);
int CheckingTerminalState(int gameState[3][3]);
int WinOrNah(int gameState[3][3], pii pos);

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
    int gameState[3][3];
    gameState[2][0] = 1;
    gameState[1][2] = -1;

    memset(gameState, 0, sizeof gameState);

    Minimax(gameState);

}

void Minimax(int gameState[3][3]) {
    int currRes = CheckingTerminalState(gameState);
    if (currRes != 69) {
        if (currRes == 0) {
            cout << "Tie!";
        }
        if (currRes == 1) {
            cout << "Cross Wins!";
        }
        if (currRes == -1) {
            cout << "Nought Wins!";
        }
        return;
    }

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (CheckTurn(gameState) == "CROSS") {
                if (gameState[i][j] != 0)
                    continue;

                gameState[i][j] = 1;
                Minimax(gameState);
                gameState[i][j] = 0;
            }
            else {
                if (gameState[i][j] != 0)
                    continue;

                gameState[i][j] = -1;
                Minimax(gameState);
                gameState[i][j] = 0;
            }
        }
    }
}

string CheckTurn(int gameState[3][3]) {
    int temp = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            temp += (gameState[i][j] == 1);
            temp -= (gameState[i][j] == -1);
        }
    }

    return (temp == 1 ) ? "NOUGHT" : "CROSS";
}

int CheckingTerminalState(int gameState[3][3]) {
    for (pii x : checkPos) {
        if (gameState[x.first][x.second] == 0)
            continue;

        int currentState = WinOrNah(gameState, x);

        // If there's a winner, return the result
        if (currentState != 0)
            return currentState;
    }

    // Checking if there's any legal move (69 -> There's still at least 1 move)
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (gameState[i][j] == 0)
                return 69;
        }
    }

    // 0 -> Tie
    return 0;
}
int WinOrNah(int gameState[3][3], pii pos) {
    int temp = gameState[pos.first][pos.second];

    // If the position is in the center of the board
    if (pos.first == pos.second) {
        // First Diagonal Line (\)
        if (gameState[0][0] == temp && gameState[2][2] == temp)
            return temp;
        // Second Diagonal Line (/)
    if (gameState[0][2] == temp && gameState[2][0] == temp)
            return temp;
    }
    else {
        // If the position is on the center of the top and bottom row
        if (pos.first != 1) {
            if (gameState[pos.first][0] == temp && gameState[pos.first][2] == temp)
                return temp;
        }
        // If the position is on the center of the right and left column
        if (pos.second != 1) {
            if (gameState[0][pos.second] == temp && gameState[2][pos.second] == temp)
                return temp;
        }
    }

    // No one is the winner yet
    return 0;
}
