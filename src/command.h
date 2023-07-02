#include <bits/stdc++.h>
#include "keyholder.h"

using namespace std;

namespace mett_com {
    enum CommandValue {
        close = -1,
        set = 1,
        disable = 2,
        swturn = 3,
    };
    static map<string, CommandValue> commandValues = {
        {"close", close},
        {"set", set},
        {"switch_turn", swturn},
        {"swturn", swturn},
    };

    static void Close(vector<string> & args) {
        return;
    }
    static void Set(vector<string> & args, map<string, bool> & bool_List) {
        string targeted_name = key_holder::bool_names[args[1]];

        cout << "\n ===========" << targeted_name;

        if (args[2] == "true") {
            bool_List[targeted_name] = true;
            cout << "\n **> " << targeted_name << " is now set to true";
        }
        if (args[2] == "false") {
            bool_List[targeted_name] = false;

            cout << "\n **> " << targeted_name << " is now set to false";
        }
    }
    static void SwitchTurn(vector<string> & args, int & first, int & second) {
        int temp = first;
        first = second;
        second = temp;
    }

}
