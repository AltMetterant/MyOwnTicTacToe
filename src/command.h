#ifndef command
#define command

#include <vector>
#include <map>
#include <string>
#include "keyholder.h"

using namespace std;

#define el '\n'

namespace mett_com {
    enum CommandValue {
        close = -1,
        help = 0,
        setrule = 1,
        ch_board = 2,
        swturn = 3,
    };
    static string cmdLogPrefix = " -i- ";

    static map<string, CommandValue> commandValues = {
        {"close", close},
        {"setrule", setrule},
        {"change_board", ch_board},
        {"cb", ch_board},
        {"switch_turn", swturn},
        {"swturn", swturn},
    };

    static void Close(vector<string> & args) {
        return;
    }
    static void SetRule(vector<string> & args, map<string, bool> & boolList) {
        int cmdSize = args.size();
        if (cmdSize == 1) {
            cout << cmdLogPrefix << "Invalid setrule command." << el;
            return;
        }
        string targetedName = key_holder::boolNames[args[1]];

        if (cmdSize == 2) {
            // Return a list of game rules (booleans)
            if (args[1] == "list") {
                // Remove the invalid targetedName from the boolNames list because
                // c++ will add the new key if the program try to get a map value out of a key
                key_holder::boolNames.erase(args[1]);


                string last_rule = "", curr_rule = "";

                // Command Log
                cout << "==== GameRule list =====" << el;
                cout << cmdLogPrefix << "{game rule} : [{name1},{name2},...]" << el;

                for (auto a : key_holder::boolNames) {
                    // Renew last_rule and curr_rule for checking
                    last_rule = curr_rule;
                    
                    curr_rule = a.second;

                    if (last_rule != curr_rule) {
                        cout << el << "    " << curr_rule << " : " << a.first << " ";
                    } 
                    else {
                        cout << a.second << " ";
                    }
                }
                
                cout << el << "================" << el;
                return;
            }

            if (targetedName.empty()) {
                cout << cmdLogPrefix << "Invalid Rule." << el;
                
                // Remove the invalid targetedName from the boolNames list because
                // c++ will add the new key if the program try to get a map value out of a key
                key_holder::boolNames.erase(args[1]);

                return;
            }
            else {
                // Give info about the targeted gamerule
                cout << '\n' << cmdLogPrefix << targetedName 
                << " is currently " << (boolList[targetedName] ? "true" : "false") << el;

                return;
            }

        } 

        string setVal = args[2];


        if (setVal == "true") {
            boolList[targetedName] = true;
            cout << '\n' << cmdLogPrefix << targetedName << " is now set to true" << el;
        }
        if (setVal == "false") {
            boolList[targetedName] = false;

            cout << '\n' << cmdLogPrefix << targetedName << " is now set to false" << el;
        }
    }
    static void SwitchTurn(vector<string> & args, int & first, int & second) {
        int temp = first;
        first = second;
        second = temp;
    }

    static vector<int> ChangeBoard(vector<string> & args) {
        vector<int> res(9, 0);
        string board = args[1];

        // Reset Board
        if (board == "reset" || board == "rs")
            return res;

        // Set new board
        // -!- If the length of the new preset-board string is less than 9, then the rest
        // of the unset value will be set to O (blank cell)
        for (int i = 0; i < board.size(); i++) {
            if (tolower(board[i]) == 'x') {
                res[i] = 1;
                continue;
            }
            if (tolower(board[i]) == 'o') {
                res[i] = -1;
                continue;
            }
        }

        return res;
    }

    static void Help(vector<string> & args) {
        if (args.size() == 1) {
            cout << " == Command Lists ==\n";
            cout << "   > close" << '\n';
            cout << "   > setrule <game_bool> <value>" << '\n';
            cout << "   > change_board [<board string>]" << '\n';
            cout << "   > swturn" << '\n';
            cout << "   <!> For more details, do help <command>" << '\n';
            cout << " ========= \n";

            return;
        }
        
        string targetedCmd = args[1];

        switch (commandValues[targetedCmd]) {
            case close : {
                cout << cmdLogPrefix << "Close the game." << el;
                cout << cmdLogPrefix << "other syntax : close" << el;

                break;
            }

            default :
                break;
        }
    }

}

#endif