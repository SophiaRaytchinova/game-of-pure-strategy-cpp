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
using std::cin;
using std::cout;
using std::endl;

const int MAX_OPPONENTS = 20;
const int MAX_USER_PASS_LEN = 1024;
const int HAND_SIZE = 5;
const int MAX_CARD_LENGTH = 4; //max length of card string (e.g., "10" + '\0')
const int DECK_SIZE = 13; //number of different cards in the deck
const int ACE_POINTS = 1;
const int JACK_POINTS = 11;
const int QUEEN_POINTS = 12;
const int KING_POINTS = 13;

struct Stats {
    int gamesPlayed;
    int gamesWon;
};

struct OpponentStats {
    char name[MAX_USER_PASS_LEN];
    int gamesPlayed;
    int gamesWon;
};

void waitAndClearScreen() {
    cout << "\nPress ENTER to continue...";
    cin.ignore(MAX_USER_PASS_LEN, '\n');

    for (int i = 0; i < 30; i++) {
        cout << endl;
    }
}

bool createProfileFile(const char username [MAX_USER_PASS_LEN]);
bool loadStats(const char username[MAX_USER_PASS_LEN], Stats &s);
bool saveStats(const char username[MAX_USER_PASS_LEN], const Stats &s);
void getProfileFileName(const char username[MAX_USER_PASS_LEN], char fileName[MAX_USER_PASS_LEN]);
//bool registerUser(const char newUsername[MAX_USER_PASS_LEN], const char newPassword[MAX_USER_PASS_LEN]);

void strCpy(char* dest, const char* src) {
    int i = 0;
    while (src[i] != '\0') { 
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0'; 
}

bool areStrEqual(const char *a, const char* b) {
    int i = 0;

    while (a[i] != '\0' && b[i]!='\0') {
        if (a[i] != b[i]) return false;
        i++;
    }
    return a[i]=='\0' && b[i]=='\0';
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

// --FUNCTIONS FOR CARDS--
void swapCards(char* a, char* b) {
    char temp[MAX_CARD_LENGTH];
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
int cardPoints(const char card[MAX_CARD_LENGTH]) {
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
        if (!isStrEmpty(hand[i])) cout << hand[i] << " ";
    }
    cout << endl;
}

bool removeCard(char hand[HAND_SIZE][MAX_CARD_LENGTH], const char card[]) {
    // find the card in hand and remove it
    for (int i = 0; i < HAND_SIZE; i++) {
        if (areStrEqual(hand[i], card)) {
            strCpy(hand[i], ""); // mark as used
            return true;
        }
    }
    return false; // card not found
}

bool isValidCard(char hand[HAND_SIZE][MAX_CARD_LENGTH], const char card[]) {
    for (int i = 0; i < HAND_SIZE; i++)
        if (areStrEqual(hand[i], card)) return true;
    return false;
}

// --FUNCTIONS FOR GAMEPLAY--
void playGame(const char username1[MAX_USER_PASS_LEN], const char username2[MAX_USER_PASS_LEN]) {
    char deck[DECK_SIZE][MAX_CARD_LENGTH] = { 
        "A", "2", "3", "4", "5", "6", "7", 
        "8", "9", "10", "J", "Q", "K"
    };
    shuffleDeck(deck);

    char hand1[HAND_SIZE][MAX_CARD_LENGTH];
    char hand2[HAND_SIZE][MAX_CARD_LENGTH];
    for (int i = 0; i < HAND_SIZE; i++) {
        strCpy(hand1[i], deck[i]);
        strCpy(hand2[i], deck[i + HAND_SIZE]);
    }

    int score1 = 0, score2 = 0, accumulatedRewardPoints = 0;

    for (int r = 0; r < HAND_SIZE; r++) {
        cout << "\nReward card: " << deck[r + 2 * HAND_SIZE] << endl;
        accumulatedRewardPoints += cardPoints(deck[r + 2 * HAND_SIZE]);
        char cardChosenByP1[MAX_CARD_LENGTH], cardChosenByP2[MAX_CARD_LENGTH];

    // ----- PLAYER 1 TURN -----
    cout << "\n" << username1 << "'s turn\n";
    cout << "Your hand: ";
    printHand(hand1);

    while (true) {
        cout << "Choose a card: ";
        cin.getline(cardChosenByP1, MAX_CARD_LENGTH);
        if (isValidCard(hand1, cardChosenByP1)) break;
        cout << "Invalid card. Try again.\n";
    }

    waitAndClearScreen();

    // ----- PLAYER 2 TURN -----
    cout << "\n" << username2 << "'s turn\n";
    cout << "Your hand: ";
    printHand(hand2);

    while (true) {
        cout << "Choose a card: ";
        cin.getline(cardChosenByP2, MAX_CARD_LENGTH);
        if (isValidCard(hand2, cardChosenByP2)) break;
        cout << "Invalid card. Try again.\n";
    }

        removeCard(hand1, cardChosenByP1);
        removeCard(hand2, cardChosenByP2);

        int pointsOfChosenCardByP1 = cardPoints(cardChosenByP1);
        int pointsOfChosenCardByP2 = cardPoints(cardChosenByP2);

        if (pointsOfChosenCardByP1 > pointsOfChosenCardByP2) { 
            score1 += accumulatedRewardPoints; 
            accumulatedRewardPoints = 0;
            cout << username1 << " wins the reward!\n"; 
        }
        else if (pointsOfChosenCardByP2 > pointsOfChosenCardByP1) { 
            score2 += accumulatedRewardPoints; 
            cout << username2 << " wins the reward!\n"; 
        }
        else cout << "Tie! Reward card remains.\n";

        cout << username1 << "'s hand: "; printHand(hand1);
        cout << username2 << "'s hand: "; printHand(hand2);
    }

    cout << "\nFinal Scores: ";
    cout << username1 << ": " << score1 << ", " << username2 << ": " << score2 << endl;
    
    if (score1 > score2) cout << username1 << " wins!\n";
    else if (score2 > score1) cout << username2 << " wins!\n";
    else cout << "Game is a tie!\n";

    bool player1Won = (score1 > score2);
    bool player2Won = (score2 > score1);

    Stats s1 = {0,0}, s2 = {0,0};
    loadStats(username1, s1);
    loadStats(username2, s2);

    s1.gamesPlayed++;
    s2.gamesPlayed++;

    if(player1Won) s1.gamesWon++;
    if(player2Won) s2.gamesWon++;

    saveStats(username1, s1);
    saveStats(username2, s2);

}

// --FUNCTIONS FOR PROFILES--
bool loadStats(const char username[MAX_USER_PASS_LEN], Stats &s) {
    char fileName[MAX_USER_PASS_LEN];
    getProfileFileName(username, fileName);

    std::ifstream file;
    file.open(fileName);
    if(!file.is_open()) return false;

    char temp[MAX_USER_PASS_LEN];
    file >> temp >> temp; // skip "Username: <name>"
    file >> temp >> temp >> s.gamesPlayed; // "Total games played: X"
    file >> temp >> temp >> temp >> s.gamesWon; // "Total games won: X"
    file.close();
    return true;
}

bool saveStats(const char username[MAX_USER_PASS_LEN], const Stats &s) {
    char fileName[MAX_USER_PASS_LEN];
    getProfileFileName(username, fileName);

    std::ofstream file;
    file.open(fileName);
    if(!file.is_open()) return false;

    file << "Username: " << username << endl;
    file << "Total games played: " << s.gamesPlayed << endl;
    file << "Total games won: " << s.gamesWon << endl;
    file << "Games against other players (wins/%): " << endl;
    file.close();
    return true;
}


//userExists function checks if the user trying to log exists in the files or not
bool userExists(const char logUsername[MAX_USER_PASS_LEN], const char logPassword[MAX_USER_PASS_LEN]) {
    if (isStrEmpty(logUsername)) return false;

    std::ifstream user;
    user.open("users.txt"); //opens file
    if (!user.is_open()) return false; //checks if the file is open

    char username[MAX_USER_PASS_LEN], password[MAX_USER_PASS_LEN];

    while (user >> username >> password) { //reads the file with users and passwords line by line
        if (areStrEqual(username, logUsername)) {
            if (areStrEqual(password, logPassword)) {   
            user.close();
            return true; // user found
            }
        }
    }

    user.close();
    return false; //user not found
}

//validRegisterData function checks if the username and password provided during registration are valid
bool validData(const char newUsername[MAX_USER_PASS_LEN], const char newPassword[MAX_USER_PASS_LEN]){
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
bool registerUser(const char newUsername[MAX_USER_PASS_LEN], const char newPassword[MAX_USER_PASS_LEN]) {
    if (userExists(newUsername, newPassword)) {
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

void getProfileFileName(const char username[MAX_USER_PASS_LEN], char fileName[MAX_USER_PASS_LEN]) {
    strCpy(fileName, username);
    int i = 0;

    while (fileName[i] != '\0') i++;
    fileName[i++] = '.';
    fileName[i++] = 't';
    fileName[i++] = 'x';
    fileName[i++] = 't';
    fileName[i] = '\0';
}

//createProfileFile function creates a profile file for the new user
bool createProfileFile(const char username[MAX_USER_PASS_LEN]) {
    char profileFile[MAX_USER_PASS_LEN];
    getProfileFileName(username, profileFile);

    std::ofstream profile;
    profile.open(profileFile);

    if (!profile.is_open()) {
        cout << "Error creating profile file for " << username << "!" << endl;
        return false;
    }

    profile << "Username: " << username << endl;
    profile << "Total games played: 0" << endl;
    profile << "Total games won: 0" << endl;
    profile << "Total Points: 0" << endl;

    profile.close();
    return true;
}

bool loginUser(const char logUsername[MAX_USER_PASS_LEN], const char logPassword[MAX_USER_PASS_LEN]) {
    if (!validData(logUsername, logPassword)) {
        return false;
    }

    std::ifstream userFile;
    userFile.open("users.txt");

    if (!userFile.is_open()) {
        //cout << "Error opening users.txt file! File might not exist." << endl;
        return false;
    }

    char fileUsername[MAX_USER_PASS_LEN], filePassword[MAX_USER_PASS_LEN];
    while (userFile >> fileUsername >> filePassword) {
        if (areStrEqual(fileUsername, logUsername) && areStrEqual(filePassword, logPassword)) {
            userFile.close();
            return true; // login successful
        }
    }

    userFile.close();
    return false; // login failed
}

void printMainMenu() {
    cout << "===== CARD GAME MENU =====" << endl;
    cout << "1. Register" << endl;
    cout << "2. Login" << endl;
    cout << "3. Exit" << endl;
    cout << "Enter your choice: ";
}

int main() {
    srand(time(0)); // seed random number generator

    int menuChoice;
    char username1[MAX_USER_PASS_LEN];
    char password1[MAX_USER_PASS_LEN];
    char username2[MAX_USER_PASS_LEN];
    char password2[MAX_USER_PASS_LEN];

    while (true) {
        printMainMenu();

        cin >> menuChoice;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(MAX_USER_PASS_LEN, '\n');
            cout << "Invalid choice. Try again." << endl;
            cout << endl;
            continue;
        }
        cin.ignore(); // clear newline left in buffer

        if (menuChoice == 1) {
            // Registration
            cout << "Enter new username: ";
            cin.getline(username1, MAX_USER_PASS_LEN);

            cout << "Enter new password: ";
            cin.getline(password1, MAX_USER_PASS_LEN);

            if (registerUser(username1, password1)) {
                cout << "Registration successful! You can now login." << endl;
            } else {
                cout << "Registration failed. Try again." << endl;
            }
        }
        else if (menuChoice == 2) {
            // Login player1
            cout << "Player 1 -> Enter username: ";
            cin.getline(username1, MAX_USER_PASS_LEN);

            cout << "Player 1 -> Enter password: ";
            cin.getline(password1, MAX_USER_PASS_LEN);

            if (!loginUser(username1, password1)) {
                cout << "Login failed for player1. Check username/password. " << endl;
                continue;
            } 
            cout << "Player 2 -> Enter username: ";
            cin.getline(username2, MAX_USER_PASS_LEN);

            cout << "Player 2 -> Enter password: ";
            cin.getline(password2, MAX_USER_PASS_LEN);

            if (!loginUser(username2, password2)) {
                cout << "Login failed for player2. Check username/password. " << endl;
                continue;
            } 

            cout << "Both players logged in successfully! Starting game! " << endl;
            
            waitAndClearScreen();
            playGame(username1, username2);
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
