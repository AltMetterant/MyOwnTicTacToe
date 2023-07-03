#ifndef keyholder
#define keyholder

#include <vector>
#include <map>
#include <string>

using namespace std;

namespace key_holder {
    // Game booleans
    static string bot2Bot = "bot2Bot";
    static string showMoveInfo = "showMoveInfo";
    static string resetBoard = "resetBoard";

    // Game integers
    static string betweenMoveDelaySecond = "betweenMoveDelaySecond";
    static string maxDepth = "maxDepth";
    static string botTurn = "botTurn";
    static string playerTurn = "playerTurn";
    
    // Game floats
    static string invalidMoveDelaySecond = "invalidMoveDelaySecond";

    // For Commands
    static map<string, string> boolNames = {
        {"b2b", bot2Bot},
        {"bot2bot", bot2Bot},
        {"bot2Bot", bot2Bot},
        {"showMoveInfo", showMoveInfo},
        {"smi", showMoveInfo},
        {"resetBoard", resetBoard},
        {"resetboard", resetBoard},
        {"rsb", resetBoard},
    };

}

#endif