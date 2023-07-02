#ifndef FILENAME_H
#define FILENAME_H

#include <vector>
#include <map>
#include <string>

using namespace std;

namespace key_holder {
    // Game booleans
    static string bot2Bot = "bot2Bot";
    static string botTurn = "botTurn";
    static string playerTurn = "playerTurn";
    static string maxDepth = "maxDepth";
    static string showMoveInfo = "showMoveInfo";

    // Game integers
    static string betweenMoveDelaySecond = "betweenMoveDelaySecond";
    
    // Game floats
    static string invalidMoveDelaySecond = "invalidMoveDelaySecond";

    // For Commands
    static map<string, string> bool_names = {
        {"b2b", bot2Bot},
        {"bot2bot", bot2Bot},
        {"bot2Bot", bot2Bot},
        {"showMoveInfo", showMoveInfo},
        {"smi", showMoveInfo}
    };

}

#endif