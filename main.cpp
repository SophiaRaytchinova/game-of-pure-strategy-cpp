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

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
using std::cin;
using std::cout;
using std::endl;

const int MAX_OPPONENTS = 50; //so we can track stats against max 50 different opponents
const int MAX_USER_PASS_LEN = 1024;
const int DECK_SIZE = 13; //number of different cards in the deck (A, 2, 3, 4, 5, 6, 7, 8, 9 , 10, J, Q, K)
const int HAND_SIZE = DECK_SIZE;
const int MAX_CARD_LENGTH = 4; //max length of card char[] (e.g., "10" + '\0')

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

bool createProfileFile(const char username [MAX_USER_PASS_LEN]);
void getProfileFileName(const char username[MAX_USER_PASS_LEN], char fileName[MAX_USER_PASS_LEN]);
bool loadFullProfile(const char username[], Stats &s, OpponentStats opponents[], int &opponentCount);
bool saveFullProfile(const char username[], const Stats &s, OpponentStats opponents[], int opponentCount);
void updateOpponent(OpponentStats opponents[], int &count, const char opponentName[], bool won);

void waitAndClearScreen() {
    cout << "\nPress ENTER to continue...";
    cin.ignore(MAX_USER_PASS_LEN, '\n');

    for (int i = 0; i < 30; i++) {
        cout << endl;
    }
}

// --FUNCTIONS FOR CHAR ARRAY--
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
void playerTurn(const char username[MAX_USER_PASS_LEN], char hand[HAND_SIZE][MAX_CARD_LENGTH], char rewards[DECK_SIZE][MAX_CARD_LENGTH], int rewardsCount, char chosenCard[MAX_CARD_LENGTH]) {
    cout << "\n" << username << "'s turn\n";
    cout << "Your hand: ";
    printHand(hand);

    cout << "Your reward cards so far: ";
    for (int i = 0; i < rewardsCount; i++) {
        cout << rewards[i] << " ";
    }
    cout << endl;

    while (true) {
        cout << "Choose a card: ";
        cin.getline(chosenCard, MAX_CARD_LENGTH);
        if (isValidCard(hand, chosenCard)) break;
        cout << "Invalid card. Try again. \n";
    }
}

void initializeHands (
    char hand1[HAND_SIZE][MAX_CARD_LENGTH], 
    char hand2[HAND_SIZE][MAX_CARD_LENGTH], 
    char deck[DECK_SIZE][MAX_CARD_LENGTH]
) {
    for (int i = 0; i< HAND_SIZE; i++) {
        strCpy(hand1[i], deck[i]);
        strCpy(hand2[i], deck[i]);
    }
}

void playRound(
    int roundIndex,
    const char username1[MAX_USER_PASS_LEN],
    const char username2[MAX_USER_PASS_LEN],
    char hand1[HAND_SIZE][MAX_CARD_LENGTH],
    char hand2[HAND_SIZE][MAX_CARD_LENGTH],
    char deck[DECK_SIZE][MAX_CARD_LENGTH],
    int &score1,
    int &score2,
    int &accRewards,
    char rewards1[DECK_SIZE][MAX_CARD_LENGTH],
    char rewards2[DECK_SIZE][MAX_CARD_LENGTH],
    int &rewardsCount1,
    int &rewardsCount2
) {
    cout << "\nReward card: " << deck[roundIndex] << endl;
    accRewards += cardPoints(deck[roundIndex]);

    char cardChosenByP1[MAX_CARD_LENGTH], cardChosenByP2[MAX_CARD_LENGTH];
    // ----- PLAYER 1 TURN -----
    playerTurn(username1, hand1, rewards1, rewardsCount1, cardChosenByP1);
    waitAndClearScreen();

    // ----- PLAYER 2 TURN -----
    playerTurn(username2, hand2, rewards2, rewardsCount2, cardChosenByP2);
    waitAndClearScreen();

    removeCard(hand1, cardChosenByP1);
    removeCard(hand2, cardChosenByP2);

    int pointsOfChosenCardByP1 = cardPoints(cardChosenByP1);
    int pointsOfChosenCardByP2 = cardPoints(cardChosenByP2);

    cout << username1 << " played: " << cardChosenByP1 << " (" << pointsOfChosenCardByP1 << " points)" << endl;
    cout << username2 << " played: " << cardChosenByP2 << " ("  << pointsOfChosenCardByP2 << " points)" << endl;

    if (pointsOfChosenCardByP1 > pointsOfChosenCardByP2) { 
        score1 += accRewards; 
        strCpy(rewards1[rewardsCount1++], deck[roundIndex]);
        accRewards = 0;
        cout << username1 << " wins the reward!"; 
    }
    else if (pointsOfChosenCardByP2 > pointsOfChosenCardByP1) { 
        score2 += accRewards; 
        strCpy(rewards2[rewardsCount2++], deck[roundIndex]);
        accRewards = 0;
        cout << username2 << " wins the reward!"; 
    }
    else cout << "Tie! Reward card remains.\n";

}

void updateStatsAfterGame(
    const char username1[MAX_USER_PASS_LEN],
    const char username2[MAX_USER_PASS_LEN],
    bool player1Won,
    bool player2Won
) {
    Stats s1 = {0, 0}, s2 = {0, 0};
    OpponentStats opp1[MAX_OPPONENTS], opp2[MAX_OPPONENTS];
    int oppCount1 = 0, oppCount2 = 0;

    loadFullProfile(username1, s1, opp1, oppCount1);
    loadFullProfile(username2, s2, opp2, oppCount2);

    s1.gamesPlayed++;
    s2.gamesPlayed++;

    if (player1Won) s1.gamesWon++;
    if (player2Won) s2.gamesWon++;

    updateOpponent(opp1, oppCount1, username2, player1Won);
    updateOpponent(opp2, oppCount2, username1, player2Won);

    saveFullProfile(username1, s1, opp1, oppCount1);
    saveFullProfile(username2, s2, opp2, oppCount2);
}

void playGame (const char username1[MAX_USER_PASS_LEN], const char username2[MAX_USER_PASS_LEN]) {
    char deck[DECK_SIZE][MAX_CARD_LENGTH] = { 
        "A", "2", "3", "4", "5", "6", "7", 
        "8", "9", "10", "J", "Q", "K"
    };

    shuffleDeck(deck);

    char hand1[HAND_SIZE][MAX_CARD_LENGTH];
    char hand2[HAND_SIZE][MAX_CARD_LENGTH];
    initializeHands(hand1, hand2, deck);
    
    int score1 = 0, score2 = 0, accumulatedRewardPoints = 0;
    char rewards1[DECK_SIZE][MAX_CARD_LENGTH];
    char rewards2[DECK_SIZE][MAX_CARD_LENGTH];
    int rewardsCount1 = 0, rewardsCount2 = 0;

    for (int j = 0; j < HAND_SIZE; j++) {
        playRound(
            j,
            username1, username2,
            hand1, hand2,
            deck,
            score1, score2,
            accumulatedRewardPoints,
            rewards1, rewards2,
            rewardsCount1, rewardsCount2
        );
    }

    cout << "\nFinal Scores: ";
    cout << username1 << ": " << score1 << endl;
    cout << username2 << ": " << score2 << endl;

    bool player1Won = score1 > score2;
    bool player2Won = score2 > score1;

    updateStatsAfterGame(username1, username2, player1Won, player2Won);
}

// --FUNCTIONS FOR PROFILES--
bool loadFullProfile(const char username[], Stats &s, OpponentStats opponents[], int &opponentCount) {
    char fileName[MAX_USER_PASS_LEN];
    getProfileFileName(username, fileName);

    std::ifstream file(fileName);
    if (!file.is_open()) return false;

    char line[MAX_USER_PASS_LEN];

    file.getline(line, MAX_USER_PASS_LEN); // Username
    file.getline(line, MAX_USER_PASS_LEN); // Total games played
    sscanf(line, "Total games played: %d", &s.gamesPlayed);

    file.getline(line, MAX_USER_PASS_LEN); // Total games won
    sscanf(line, "Total games won: %d", &s.gamesWon);

    file.getline(line, MAX_USER_PASS_LEN); // "Games against other players..."

    opponentCount = 0;
    while (file.getline(line, MAX_USER_PASS_LEN)) {
        sscanf(
            line,
            "%[^:]: %d games played (%d",
            opponents[opponentCount].name,
            &opponents[opponentCount].gamesPlayed,
            &opponents[opponentCount].gamesWon
        );
        opponentCount++;
    }

    file.close();
    return true;
}

void updateOpponent(OpponentStats opponents[], int &count, const char opponentName[], bool won) {
    for (int i = 0; i < count; i++) {
        if (areStrEqual(opponents[i].name, opponentName)) {
            opponents[i].gamesPlayed++;
            if (won) opponents[i].gamesWon++;
            return;
        }
    }

    strCpy(opponents[count].name, opponentName);
    opponents[count].gamesPlayed = 1;
    opponents[count].gamesWon = won ? 1 : 0;
    count++;
}

bool saveFullProfile(const char username[], const Stats &s, OpponentStats opponents[], int opponentCount) {
    char fileName[MAX_USER_PASS_LEN];
    getProfileFileName(username, fileName);

    std::ofstream file(fileName);
    if (!file.is_open()) return false;

    double winPercent = (s.gamesPlayed > 0) ? (s.gamesWon * 100.0 / s.gamesPlayed) : 0.0;

    file << "Username: " << username << endl;
    file << "Total games played: " << s.gamesPlayed << endl;
    file << "Total games won: " << s.gamesWon
         << " (" << winPercent << "%)" << endl;
    file << "Games against other players (wins/%):" << endl;

    for (int i = 0; i < opponentCount; i++) {
        double p = (opponents[i].gamesPlayed > 0)
            ? (opponents[i].gamesWon * 100.0 / opponents[i].gamesPlayed)
            : 0.0;

        file << opponents[i].name << ": "
             << opponents[i].gamesPlayed << " games played ("
             << opponents[i].gamesWon << "/" << p << "% wins)"
             << endl;
    }

    file.close();
    return true;
}

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
