/*
*  
* Solution to course project #1
* Introduction to programming course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2025/2026
*
* @author Sophia Raytchinova
* @idnumber 5MI0600681
* @compiler Visual Studio
*
* <main.cpp>
*
*/

//for now comments are only for my understanding, will clean up later

#include <iostream>
#include <fstream> // for file operations
#include <cstdlib> // for rand() and srand()
#include <ctime>   // for time()
#include <vector>  // for dynamic arrays-hand, deck
#include <string> 

using std::cin;
using std::cout;
using std::endl;

const int HAND_SIZE = 5;
const int ACE_POINTS = 1;
const int JACK_POINTS = 11;
const int QUEEN_POINTS = 12;
const int KING_POINTS = 13;

bool createProfileFile(const std::string& username);
bool registerUser(const std::string& newUsername, const std::string& newPassword);




bool areStrEqual(const char *a, const char* b) {
    int i = 0;
    while (a[i] != '\0' && b[i]!='\0') {
        if (a[i] != b[i]) return false;
        else return true;
    }
    return a[i]==b[i];
}

bool containsSpace(const char* s) {
    for (int i = 0; s[i] != '\0'; i++) {
        if (s[i] == ' ') return true;
    }
    return false;
}

bool isStrEmpty(const char* s) {
    return s[0] == '\0';
}

void swapCards(char* a, char* b) {
    char temp[3];
    int i = 0;

    while (a[i] != '\0') {
        temp[i] = a[i];
        i++;
    }
    temp[i] = '\0';

    i = 0; 
    while (b[i] != '\0') {
        a[i] = b[i];
        i++;
    }
    a[i] = '\0';

    i = 0; 
    while (temp[i] != '\0') {
        b[i] = temp[i];
        i++;
    }
    b[i] = '\0';
}

// --Basic functions for cards--
//cardPoints function returns the points of a given card as an integer
int cardPoints(const char* card) {
    if (areStrEqual(card, "A")) return ACE_POINTS;
    if (areStrEqual(card, "J")) return JACK_POINTS;
    if (areStrEqual(card, "Q")) return QUEEN_POINTS;
    if (areStrEqual(card, "K")) return KING_POINTS;

    int value = 0;
    for (int i=0; card[i]!='\0'; i++) {
        value = value*10+(card[i]-'0');
    }
    return value;
}


//shuffleDeck function shuffles the deck of cards
//check if you can use sth else like an array instead of vector
void shuffleDeck(std::vector<std::string>& deck) {
    for (size_t i = deck.size() - 1; i > 0; i--) { //Fisher-Yates shuffle algorithm
        int randomIndex = rand() % (i + 1);
        mySwap(deck[i], deck[randomIndex]);
    }
}

//printHand function prints the cards you have in that moment
void printHand(const std::vector<std::string>& hand) {
    for (size_t i = 0; i < hand.size(); i++) {
        cout << hand[i] << " ";
    }
    cout << endl;
}

// --Functions for profiles--

//userExists function checks if the user trying to log exists in the files or not
bool userExists(const std::string& logUsername) {
    if (logUsername.empty()) {
        return false;
    }

    std::ifstream user;//("users.txt"); //opens file in read mode
    user.open("users.txt");

    if (!user.is_open()) { //checks if file is open
        return false;
    }

    std::string username, password;
    while (user >> username >> password) { //reads the file with users and passwords line by line
        if (username == logUsername) {
            user.close();
            return true; // user found
        }
    }

    user.close();
    return false; //user not found
}

//validRegisterData function checks if the username and password provided during registration are valid
bool validRegisterData(const std::string& newUsername, const std::string& newPassword){
    if (newUsername.empty() || newPassword.empty()) {
        cout << "Username and password cannot be empty!" << endl;
        return false;
    }
    if (newUsername.find(' ') != std::string::npos) {
        cout << "Username cannot contain spaces!" << endl;
        return false;
    }
    if (newPassword.find(' ') != std::string::npos) {
        cout << "Password cannot contain spaces!" << endl;
        return false;
    }
    return true;
}

//registerUser function registers a new user by adding their username and password to the users.txt file and creates a profile file for them
bool registerUser(const std::string& newUsername, const std::string& newPassword) {
    if (userExists(newUsername)) {
        cout << "User already exists!" << endl;
        return false;
    }

    if (!validRegisterData(newUsername, newPassword)) {
        return false;
    }

    //append new user to user.txt
    std::ofstream users;
    users.open("users.txt", std::ios::app); //opens file in append mode, so we can add content without removing its current content
    
    if (!users.is_open()) { //checks if file is open
        cout << "Error opening user.txt file!" << endl;
        return false;
    }

    users << newUsername << " " << newPassword << endl; //writes username and password in users.txt
    users.close();

    return createProfileFile(newUsername);
}

//createProfileFile function creates a profile file for the new user
bool createProfileFile(const std::string& username) {
    std::ofstream profile;
    profile.open(username + ".txt"); //creates a new file for user and their stats
    
    if (!profile.is_open()) {
        cout << "Error creating profile file for " << username << "!" << endl;
        return false;
    }

    profile << "Username: " << username << endl;
    profile << "Total games played: 0" << endl;
    profile << "Total games won: 0 (0%)" << endl;
    profile << "Games against other players (wins/%): " << endl;

    profile.close();
    return true;
}

//validLoginData function checks if the username and password provided during login are valid
bool validLoginData(const std::string& logUsername, const std::string& logPassword) {
    if (logUsername.empty() || logPassword.empty()) {
        cout << "Username and password cannot be empty!" << endl;
        return false;
    }
    if (logUsername.find(' ') != std::string::npos) {
        cout << "Not valid username, try wihthout spaces!" << endl;
        return false;
    }
    if (logPassword.find(' ') != std::string::npos) {
        cout << "Not valid password, try without spaces!" << endl;
        return false;
    }
    return true;
}

bool loginUser(const std::string& logUsername, const std::string& logPassword) {
    if (!validLoginData(logUsername, logPassword)) {
        return false;
    }

    std::ifstream userFile;//("users.txt");
    userFile.open("users.txt");

    if (!userFile.is_open()) {
        cout << "Error opening users.txt file!" << endl;
        return false;
    }

    std::string fileUsername, filePassword;
    while (userFile >> fileUsername >> filePassword) {
        if (fileUsername == logUsername && filePassword == logPassword) {
            userFile.close();
            return true; // login successful
        }
    }

    userFile.close();
    return false; // login failed
}


// go over generated main for testing and check

int main() {
    srand(time(0)); // seed random number generator

    int menuChoice;
    std::string username, password;

    while (true) {
        // Main menu
        cout << "===== CARD GAME MENU =====" << endl;
        cout << "1. Register" << endl;
        cout << "2. Login" << endl;
        cout << "3. Exit" << endl;
        cout << "Enter your menuChoice: ";

        cin >> menuChoice;
        cin.ignore(); // clear newline left in buffer

        if (menuChoice == 1) {
            // Registration
            cout << "Enter new username: ";
            std::getline(cin, username);

            cout << "Enter new password: ";
            std::getline(cin, password);

            if (registerUser(username, password)) {
                cout << "Registration successful! You can now login." << endl;
            } else {
                cout << "Registration failed. Try again." << endl;
            }
        }
        else if (menuChoice == 2) {
            // Login
            cout << "Enter username: ";
            std::getline(cin, username);

            cout << "Enter password: ";
            std::getline(cin, password);

            if (loginUser(username, password)) {
                cout << "Login successful! Welcome, " << username << "!" << endl;

                // ---- Simple card demo ----
                std::vector<std::string> deck = { "A","2","3","4","5","6","7","8","9","10","J","Q","K" };
                
                shuffleDeck(deck);

                std::vector<std::string> hand;
                for (int i = 0; i < HAND_SIZE; i++) {
                    hand.push_back(deck[i]); // draw 5 cards
                }

                cout << "Your hand: ";
                printHand(hand);

                int totalPoints = 0;
                for (const std::string& card : hand) {
                    totalPoints += cardPoints(card);
                }
                cout << "Card points: " << totalPoints << endl;

            } else {
                cout << "Login failed. Check your username/password." << endl;
            }
        }
        else if (menuChoice == 3) {
            cout << "Exiting program. Goodbye!" << endl;
            break;
        }
        else {
            cout << "Invalid choice. Try again." << endl;
        }

        cout << endl;
    }

    return 0;
}


