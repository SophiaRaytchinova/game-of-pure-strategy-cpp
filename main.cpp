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

const int MAX_LEN = 1024;
const int HAND_SIZE = 5;
const int MAX_CARD_LENGTH = 3; //max length of card string (e.g., "10" + '\0')
const int DECK_SIZE = 52;
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

// --Functions for cards--
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

//cardPoints function returns the points of a given card as an integer
int cardPoints(const char* card) {
    if (areStrEqual(card, "A")) return ACE_POINTS;
    if (areStrEqual(card, "J")) return JACK_POINTS;
    if (areStrEqual(card, "Q")) return QUEEN_POINTS;
    if (areStrEqual(card, "K")) return KING_POINTS;

    int value = 0;
    for (int i = 0; card[i] != '\0'; i++) {
        value = value * 10 + (card[i] - '0');
    }
    return value;
}

//shuffleDeck function shuffles the deck of cards
void shuffleDeck(char deck[DECK_SIZE][MAX_CARD_LENGTH]) {
    for (size_t i = DECK_SIZE - 1; i > 0; i--) { //Fisher-Yates shuffle algorithm
        int randomIndex = rand() % (i + 1);
        swapCards(deck[i], deck[randomIndex]);
    }
}

//printHand function prints the cards you have in that moment
void printHand(char hand[HAND_SIZE][MAX_CARD_LENGTH]) {
    for (size_t i = 0; i < HAND_SIZE; i++) {
        cout << hand[i] << " ";
    }
    cout << endl;
}

// --Functions for profiles--

//userExists function checks if the user trying to log exists in the files or not
bool userExists(const char* logUsername) {
    if (isStrEmpty(logUsername)) return false;

    std::ifstream user;
    user.open("users.txt"); //opens file
    if (!user.is_open()) return false; //checks if the file is open

    char username[MAX_LEN], char password[MAX_LEN];

    while (user >> username >> password) { //reads the file with users and passwords line by line
        if (areStrEqual(username, password)) {
            user.close();
            return true; // user found
        }
    }

    user.close();
    return false; //user not found
}

//validRegisterData function checks if the username and password provided during registration are valid
bool validData(const char newUsername[MAX_LEN], const char newPassword[MAX_LEN]){
    if (isStrEmpty(newUsername) || isStrEmpty(newPassword)) {
        cout << "Username and password cannot be empty!" << endl;
        return false;
    }
    if (containsSpace(newUsername) || containsSpace(newPassword)) {
        cout << "Username and password cannot contain spaces!" << endl;
        return false;
    }
    return true;
}

//registerUser function registers a new user by adding their username and password to the users.txt file and creates a profile file for them
bool registerUser(const char newUsername[MAX_LEN], const char newPassword[MAX_LEN]) {
    if (userExists(newUsername)) {
        cout << "User already exists!" << endl;
        return false;
    }

    if (!validData(newUsername, newPassword)) {
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
bool createProfileFile(const char* username) {
    char fileName[MAX_LEN];
    int i = 0;

    while (username[i] != '\0') {
        fileName[i] = username[i];
        i++;
    }
    fileName[i++] = '.';
    fileName[i++] = 't';
    fileName[i++] = 'x';
    fileName[i++] = 't';
    fileName[i] = '\0';

    std::ofstream profile;
    profile.open(fileName);

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

bool loginUser(const char logUsername[MAX_LEN], const char logPassword[MAX_LEN]) {
    if (!validData(logUsername, logPassword)) {
        return false;
    }

    std::ifstream userFile;
    userFile.open("users.txt");

    if (!userFile.is_open()) {
        cout << "Error opening users.txt file!" << endl;
        return false;
    }

    char fileUsername[MAX_LEN], filePassword[MAX_LEN];
    while (userFile >> fileUsername >> filePassword) {
        if (areStrEqual(fileUsername, logUsername) && areStrEqual(filePassword, logPassword)) {
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


