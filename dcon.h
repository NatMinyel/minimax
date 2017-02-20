//
// Created by nathnel on 6/4/17.
//

#ifndef MINIMAX_DCON_H
#define MINIMAX_DCON_H

#include "users.h"
#include <cstdlib>
#include <pqxx/pqxx>
using namespace pqxx;

void sendToDatabase(User& user) {
    try
    {
        // Create connection
        connection c("dbname=db");
        work insert(c);
        // Insert to database (with the exceptions of nullable values)
        insert.exec("INSERT INTO users(username, password)"
                            "VALUES(" + insert.quote(user.username)
                    + ", " + insert.quote(user.password) + ");"
        );
        // Save changes
        insert.commit();
        c.disconnect();
    } catch (const exception &e) {
        cerr << e.what() << endl;
        return;
    }
}

User createUser() {
    try {
        User user1;
        // Create connection for checking username availability
        connection c("dbname=db");
        work w(c);

        string username;

        enterUsername:
        cout << "Enter a username (CASE-SENSITIVE): ";
        cin >> username;
        // if b is a username already taken ask again
        result r = w.exec("SELECT * from users WHERE username=" + w.quote(username) + ";");
        // If taken
        if (!r.empty()) {
            cout << "Enter another username. This one is already taken." << endl;
            goto enterUsername;
        }
        // Disconnect
        w.commit();
        c.disconnect();

        string pass;
        cout << "Enter a password: ";
        cin >> pass;

        pass = encrypt_password(pass, "1"); // Encrypt using "1" as seed

        // Load initial values into user1
        user1.username = username;
        user1.password = pass;
        user1.successRate = 0.0;
        user1.gamesPlayed = 0;
        user1.gamesLost = 0;
        user1.gamesWon = 0;

        // Send to database
        sendToDatabase(user1);
        // Return created user
        return user1;
    } catch (const exception &e) {
        cerr << e.what() << endl;
    }
}


void signIn(User& user1) {
    try {
        // Create connection with the database
        connection c("dbname=db");
        work w(c);

        string uName;

        enterUsername:
        // Prompt for user name
        cout << "Enter your username (CASE-SENSITIVE): ";
        cin >> uName;
        result r = w.exec("SELECT * from users WHERE username=" + w.quote(uName) + ";");
        // If username does not exist
        if (r.empty()) {
            cout << "This username appears to be invalid. Create Account or try Again." << endl;
            goto enterUsername;
        } else {
            string pass;

            // Password input tries
            int passAuth = 0;
            enterPass:
            cout << "Enter your password: ";
            cin >> pass;

            // Encrypt password input
            pass = encrypt_password(pass, "1");

            passAuth++;
            // Check if the username and password are on the same
            // row in the database
            string realPass = r[0][2].c_str();

            // If wrong password is entered
            if (pass != realPass) {
                if (passAuth > 3) {
                    // If wrong password is entered three times in a row, exit
                    cout << "You've entered an incorrect password three times in a row.\n"
                            "Program is now exiting" << endl;
                    exit(1);
                }
                // Else prompt again
                cout << "This password is incorrect. Try Again." << endl;
                goto enterPass;
            }

            // Load user data in to the structure
            user1.username = r[0][1].c_str();
            user1.password = r[0][2].c_str();
            user1.gamesWon = atoi(r[0][3].c_str()); // Cast from string to int
            user1.gamesLost = atoi(r[0][4].c_str());
            user1.successRate = atof(r[0][5].c_str()); // Cast string to float
            user1.gamesPlayed = atoi(r[0][6].c_str());

            // Solved boards array in the form {a1, a2, a3, ...} string
            string solved = r[0][7].c_str();
            vector<int>solvedBoards;
            //  Iterate over all chars in the string
            int size = solved.size();
            // Intermediate strings to store numbers up to the comma
            string intermediate = "";

            for (int i=1; i<size; i++) {
                intermediate += solved[i];
                // If there's a comma or a close-brace, signal the end of a number
                if (solved[i] == ',' || solved[i] == '}') {
                    // Cast into an int
                    int temp = stoi(intermediate);
                    // Send to the vector
                    solvedBoards.push_back(temp);
                    // Reset intermediate string
                    intermediate = "";
                }
            }

            // Load into user1
            user1.boardsolved = solvedBoards;
        }
        // Finish working and close connection
        w.commit();
        c.disconnect();
    } catch (const exception &e) {
        cerr << e.what() << endl;
        return;
    }
}

void update(User& user) {
    try {
        // Create connection
        connection c("dbname=db");
        work update(c);

        // Create a string to send the array of solved levels
        // in the form {a1, a2, a3}
        string solved = "{";
        int s = user.boardsolved.size();
        for (int  i = 0; i < s; i++) {
            solved += to_string(user.boardsolved[i]);
            if (i != s-1)
                solved += ',';
        }
        solved += '}';

        // Update database with new information
        update.exec("UPDATE users SET gameswon=" +
                    to_string(user.gamesWon) + ", gamesplayed=" +
                    to_string(user.gamesPlayed) + ", gameslost=" + to_string(user.gamesLost)
                    + ", successrate=" + to_string(user.successRate)+ ", solvedboards=" +
                    update.quote(solved) + " WHERE username = "+
                    update.quote(user.username) + ";");

        // Save into database and close connection
        update.commit();
        c.disconnect();
    }
    catch (const exception &e)
    {
        cerr << e.what() << endl;
        return;
    }
}

#endif //MINIMAX_DCON_H
