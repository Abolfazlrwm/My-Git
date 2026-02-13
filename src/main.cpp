#include "../include/MyGit.hpp"
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cout << "MyGit v1.0 - Terminal Version Control System\n";
        cout << "Usage: mygit <command> [<args>]\n\n";
        cout << "Commands:\n  init, add, commit -m, log, checkout, status\n";
        return 1;
    }

    string cmd = argv[1];
    MyGit git;

    if (cmd == "init")
        git.init();
    else if (cmd == "add" && argc > 2)
        git.add(argv[2]);
    else if (cmd == "commit" && argc > 3 && string(argv[2]) == "-m")
        git.commit(argv[3]);
    else if (cmd == "log")
        git.log();
    else if (cmd == "checkout" && argc > 2)
        git.checkout(argv[2]);
    else if (cmd == "status")
        git.status();
    else
        cout << "Unknown command or missing arguments." << endl;

    return 0;
}