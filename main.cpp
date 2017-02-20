#include "dcon.h"
#include "play_game.h"


void mainMenu();
void dashboard(User&, int=0);
string isSolved(User*, int);
int boardMenu(User*);
void viewStats(User&);


int main() {
    system("clear");
    mainMenu();
}


void mainMenu() {
    cout << "\t\t*******************************" << endl;
    cout << "\t\t*        Tic-tac-toe          *" << endl;
    cout << "\t\t*******************************" << endl;
    cout << endl << endl;
    char response;
    mainMenu:
    // *****************************
    //          MAIN MENU
    // *****************************
    cout << "\tMenu" << endl;
    cout << "1. Sign In " << endl;
    cout << "2. Sign Up " << endl;
    cout << "You can press esc to quit." << endl << endl;
    cout << "Enter your choice. ";

    response = getch();
    if (response != '1' and response != '2' and response != 27) {
        system("clear");
        cout << "*******Enter a valid response*******\n";
        goto mainMenu;
    } else if (response == '1') {
        system("clear");
        User user1; // Create some user to pass by reference
        signIn(user1); // Sign user in
        dashboard(user1, 2); // go to dashboard is signed in
    } else if (response == '2') {
        system("clear");
        User user1;
        user1 = createUser();
        dashboard(user1, 1);
    } else exit(1);
}

void dashboard(User& user, int forMsg) { // Default: no message
    system("clear");
    if (forMsg == 1) {
        // Message for user creation
        cout << "****************************" << endl;
        cout << "User successfully created." << endl;
        cout << "****************************"<<endl;
    } else if (forMsg == 2) {
        // Message for user log in
        cout << "*****************************" << endl;
        cout << "Welcome " << user.username << "! " <<endl;
        cout << "*****************************" << endl;
    }

    dashboard:
    cout << "What do you want to do? " << endl;
    cout << "1. Play a game "<<endl;
    cout << "2. View Statistics "<<endl;
    cout << "3. Logout" <<endl;
    cout<<"Press esc to exit. "<<endl;
    char in;
    dash:
    in = getch();

    if ((in < '1' or in > '3') and in != 27) {
        cout << "Enter a valid response." << endl;
        goto dash;
    } else if (in == 27) {
        exit(1);
    } else if (in == '1') {
        system("clear");

        boardMenu(&user); // Start playing games

        prompt:
        cout<<endl<<"Press esc to quit. Press d to go back to the dashboard."<<endl;
        cout<<"Press a to play again."<<endl;
        char resp = getch();

        if (resp != 27 and resp != 'd' and resp != 'a') {
            cout<<"Enter a vaild response."<<endl;
            goto prompt;
        } else if (resp == 27) exit(1); // Exit
        else if (resp == 'd') dashboard(user); // Go back to user dashboard
        else {
            system("clear");
            boardMenu(&user);
            goto prompt;
        } // Go back to playing another game
    } else if (in == '3') {
        system("clear");
        // Display message and return to main menu
        cout<<"You have been successfully logged out."<<endl;
        mainMenu();
    } else {
        viewStats(user); // View current user statistics
        system("clear");
        goto dashboard;
    }
}

string isSolved(User* user, int level) {
    /*
     * This function accepts a user and level
     * and checks if that level was already solved
     * by the user
     * */
    int numOfBoards = user->boardsolved.size();
    for (int i = 0; i < numOfBoards; i++) {
        if (user->boardsolved[i] == level) {
            return "\033[33m*Solved* \033[0m"; // Change color and return solved
        }
    }
    return ""; // Return empty string
}

int boardMenu(User* user) {
    /*
     * Displays board options, level options
     * as a menu accepts the input and
     * sets game level and size.
     * Starts a game and updates user stats after.
     */
    boardMenu:
    cout << "********Play Game********" << endl;
    boards:
    // *********************************
    //            BOARD MENU
    // *********************************
    cout << "----Select your board size----" << endl;
    cout << "1. 3x3" << endl;
    cout << "2. 4x4" << endl;
    cout << "3. 5x5" << endl;
    cout << "4. 6x6" << endl;
    cout << "5. 7x7" << endl;
    cout << "Press b to go back. "<<endl;
    cout << "Enter esc to leave game. " << endl;
    char in;
    in = getch();
    if ((in < '1' or in > '5') and in != 27 and in != 'b') {
        system("clear");
        cout << "Enter a valid response." << endl;
        goto boardMenu;
    } else if (in == 27) {
        exit(0); // Exit
    }  else if (in == 'b') {
        dashboard(*user); // Go back to dashboard
    } else {
        system("clear");
        int size = in - 48;

        // *******************************
        //         LEVEL MENU
        // *******************************
        boardSubMenu:
        cout << "----Select your level----" << endl;
        string msg = isSolved(user, (size - 1)*3 + 1);
        cout << "1. Easy " << msg << endl;
        msg = isSolved(user, (size - 1)*3+ 2);
        cout << "2. Medium " << msg << endl;
        msg = isSolved(user, (size - 1)*3 + 3);
        cout << "3. Hard " << msg << endl;
        cout << "Enter esc to leave game. " << endl;
        cout << "Enter 'b' to go back to previous menu. " << endl;

        char input;
        input = getch();
        if ((input < '1' or input > '3') and input != 27 and input != 'b') {
            system("clear");
            cout << "Enter a valid response." << input << endl;
            goto boardSubMenu;
        } else if (input == 27) {
            exit(0);
        } else if (input == 'b') {
            system("clear");
            goto boards;
        } else {

            int level = (size - 1)*3 + (input - 48);

            // Create a board using the properties size and level
            Board board;
            board.size = size + 2;
            board.level = level;

            int depth = 2 + size + (input - 49);

            if (input == '1') depth = 1;
            else if (input == '2') depth -= 2;

            // Play the game
            int win = playGame(size + 2, depth);

            user->gamesPlayed++;

            // If user won update stats
            if (win == 1) {
                user->gamesWon++;
                // If the board is already solved
                int len = user->boardsolved.size();
                bool found = 0;
                for (int i = 0; i< len; i++) {
                    if (board.level == user->boardsolved[i]) {
                        found = 1;
                        break;
                    }
                }

                // If it's not already solved
                if (!found)
                    user->boardsolved.push_back(board.level);

            } else if (win == -1)
                user->gamesLost++;

            // Calculate success rate
            user->successRate = (float) user->gamesWon / (float) user->gamesPlayed;
            update(*user);
        }
    }
}


void viewStats(User& user) {
    system("clear");
    if (user.gamesPlayed == 0) {
        cout<<"***********************************"<<endl;
        cout<<"You have not played any games yet."<<endl;
        cout<<"Come back when you've played some."<<endl;
        cout<<"***********************************"<<endl;
        cout<<"Press any key to continue..."<<endl;
        getch();
        return;
    }
    else {
        system("clear");
        cout<<"\t\tStatistics for user "<<user.username<<endl<<endl;
        cout<<"Games Won: "<<user.gamesWon<<endl;
        cout<<"Games Lost: "<<user.gamesLost<<endl;
        cout<<"Games Played: "<<user.gamesPlayed<<endl;
        cout<<"Success Rate: "<<user.successRate*100<<"%"<<endl;
        cout<<"\nPress any key to continue...";
        getch();
        return;
    }
}