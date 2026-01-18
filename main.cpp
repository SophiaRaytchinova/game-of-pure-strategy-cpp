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
* <Main file containing the implementation of the "Pure strategy" card game logic, profile management and menu system>
*
*/

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
using std::cin;
using std::cout;
using std::endl;

const int SCREEN_HEIGHT = 30; //for size of console clear
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
bool containsSpace(const char* s);
bool isStrEmpty(const char* s);

// --HELPER FUNCTIONS--
void waitAndClearScreen() {
    cout << "\nPress ENTER to continue...";
    cin.ignore(MAX_USER_PASS_LEN, '\n');
    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        cout << endl;
    }
}

int readFirstNumberFromLine(const char line[]) {
    int i = 0;
    while (line[i] != '\0' && (line[i] < '0' || line[i] > '9')) {
        i++;
    }
    int number = 0;
    while (line[i] >= '0' && line[i] <= '9') {
        number = number * 10 + (line[i] - '0');
        i++;
    }
    return number;
}

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

// --FUNCTIONS FOR CHAR ARRAY--
void trimStr(char str[MAX_CARD_LENGTH]) {
    int start = 0;
    while (str[start] == ' ') start++;

    int end = 0;
    while (str[end] != '\0') end++;
    end--;

    if (end < start) {
        str[0] = '\0';
        return;
    }

    int j = 0;
    for (int i = start; i <= end; i++) {
        str[j++] = str[i];
    }
    str[j] = '\0';
}

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

void shuffleDeck(char deck[DECK_SIZE][MAX_CARD_LENGTH]) {
    for (size_t i = DECK_SIZE - 1; i > 0; i--) { //using the Fisher-Yates shuffle algorithm
        int randomIndex = rand() % (i + 1);
        swapCards(deck[i], deck[randomIndex]);
    }
}

void printHand(char hand[HAND_SIZE][MAX_CARD_LENGTH]) {
    for (size_t i = 0; i < HAND_SIZE; i++) {
        if (!isStrEmpty(hand[i])) cout << hand[i] << " ";
    }
    cout << endl;
}

bool removeCard(char hand[HAND_SIZE][MAX_CARD_LENGTH], const char card[]) {
    for (int i = 0; i < HAND_SIZE; i++) {
        if (areStrEqual(hand[i], card)) {
            strCpy(hand[i], ""); //mark as used card
            return true;
        }
    }
    return false;
}

bool isValidCard(char hand[HAND_SIZE][MAX_CARD_LENGTH], const char card[]) {
    for (int i = 0; i < HAND_SIZE; i++) {
        if (areStrEqual(hand[i], card)) return true;
    }
    return false;
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

// --FUNCTIONS FOR GAMEPLAY--
void playerTurn(
    const char username[MAX_USER_PASS_LEN], 
    char hand[HAND_SIZE][MAX_CARD_LENGTH], 
    char rewards[DECK_SIZE][MAX_CARD_LENGTH], 
    int rewardsCount, 
    char chosenCard[MAX_CARD_LENGTH]
) {
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
        if (cin.fail()) {
            cin.clear();
            cin.ignore(MAX_USER_PASS_LEN, '\n');
        }
        trimStr(chosenCard);

        if (isStrEmpty(chosenCard)) {
            cout << "You must enter a card! Try again. \n";
            continue;
        }

        if (isValidCard(hand, chosenCard)) break;
        cout << "Invalid card. Try again. \n";
    }
}

bool playRound(
    int roundIndex,
    const char username1[MAX_USER_PASS_LEN],
    const char username2[MAX_USER_PASS_LEN],
    char hand1[HAND_SIZE][MAX_CARD_LENGTH],
    char hand2[HAND_SIZE][MAX_CARD_LENGTH],
    char deck[DECK_SIZE][MAX_CARD_LENGTH],
    int &score1,
    int &score2,
    char rewards1[DECK_SIZE][MAX_CARD_LENGTH],
    char rewards2[DECK_SIZE][MAX_CARD_LENGTH],
    int &rewardsCount1,
    int &rewardsCount2,
    int &accumulatedRewardCount,
    char accumulatedRewards[DECK_SIZE][MAX_CARD_LENGTH]
) {
    char cardChosenByP1[MAX_CARD_LENGTH], cardChosenByP2[MAX_CARD_LENGTH];
    waitAndClearScreen();
    cout << "\nReward card for this round: " << deck[roundIndex]; //<< endl
    if (accumulatedRewardCount > 0) {
        cout << "\nCurrent reward cards on the table: ";
        for (int i = 0; i < accumulatedRewardCount; i++) {
            cout << accumulatedRewards[i] << " ";
            cout << endl;
        }
    }
    playerTurn(username1, hand1, rewards1, rewardsCount1, cardChosenByP1);
    removeCard(hand1, cardChosenByP1);
    waitAndClearScreen();

    cout << "\nReward card for this round: " << deck[roundIndex] << endl;
    if (accumulatedRewardCount > 0) {
        cout << "\nCurrent reward cards on the table: ";
        for (int i = 0; i < accumulatedRewardCount; i++) {
            cout << accumulatedRewards[i] << " ";
            cout << endl;
        }
    }
    playerTurn(username2, hand2, rewards2, rewardsCount2, cardChosenByP2);
    removeCard(hand2, cardChosenByP2);
    waitAndClearScreen();

    int pointsOfChosenCardByP1 = cardPoints(cardChosenByP1);
    int pointsOfChosenCardByP2 = cardPoints(cardChosenByP2);

    cout << username1 << " played: " << cardChosenByP1 << " (" << pointsOfChosenCardByP1 << " points)" << endl;
    cout << username2 << " played: " << cardChosenByP2 << " ("  << pointsOfChosenCardByP2 << " points)" << endl;

    strCpy(accumulatedRewards[accumulatedRewardCount++], deck[roundIndex]);

    if (pointsOfChosenCardByP1 > pointsOfChosenCardByP2) { 
        for (int i = 0; i < accumulatedRewardCount; i++) {
            strCpy(rewards1[rewardsCount1++], accumulatedRewards[i]);
            score1 += cardPoints(accumulatedRewards[i]);
        }
        accumulatedRewardCount = 0;
        cout << username1 << " wins all the reward cards this round!\n"; 
        return true;
    }
    else if (pointsOfChosenCardByP2 > pointsOfChosenCardByP1) { 
        for (int i = 0; i < accumulatedRewardCount; i++) {
            strCpy(rewards2[rewardsCount2++], accumulatedRewards[i]);
            score2 += cardPoints(accumulatedRewards[i]);
        }
        accumulatedRewardCount = 0;
        cout << username2 << " wins all the reward cards this round!\n"; 
        return true;
    }
    else {
        cout << "Tie! Reward card remains.\n";
        //if ( i == DECK_SIZE - 1 ) cout << "No cards left. Final tie cards are discarded.\n";
        return false;
    }
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
    char hand1[HAND_SIZE][MAX_CARD_LENGTH];
    char hand2[HAND_SIZE][MAX_CARD_LENGTH];

    initializeHands(hand1, hand2, deck);
    shuffleDeck(deck);

    int score1 = 0, score2 = 0;
    char rewards1[DECK_SIZE][MAX_CARD_LENGTH];
    char rewards2[DECK_SIZE][MAX_CARD_LENGTH];
    int rewardsCount1 = 0, rewardsCount2 = 0;
    int accumulatedRewardCount = 0;
    char accumulatedRewards[DECK_SIZE][MAX_CARD_LENGTH];

    int roundIndex = 0;
    while (roundIndex < HAND_SIZE) {
        bool roundHadWinner = playRound(
            roundIndex,
            username1, username2,
            hand1, hand2,
            deck,
            score1, score2,
            rewards1, rewards2,
            rewardsCount1, rewardsCount2,
            accumulatedRewardCount, accumulatedRewards
        );
            roundIndex++;
    }
    cout << "\nFinal Scores: " << endl;
    cout << username1 << ": " << score1 << endl;
    cout << username2 << ": " << score2 << endl;

    bool player1Won = score1 > score2;
    bool player2Won = score2 > score1;
    if (player1Won) {
        cout << username1 << " wins the game!" << endl;
    } 
    else if (player2Won) {
        cout << username2 << " wins the game!" << endl;
    }
    else {
        cout << "The game ends in a tie! Checking number of cards won..." << endl;
        if (rewardsCount1 > rewardsCount2) {
            cout << username1 << " wins the game by number of cards won!" << endl;
            player1Won = true;
        }
        else if (rewardsCount2 > rewardsCount1) {
            cout << username2 << " wins the game by number of cards won!" << endl;
            player2Won = true;
        }
        else {
            cout << "The game is a complete tie! No winner this time." << endl;
        }
    }

    updateStatsAfterGame(username1, username2, player1Won, player2Won);

    cout << "GAME OVER!" << endl;
    cout<< "Press ENTER to return to main menu...";
    cin.ignore(MAX_USER_PASS_LEN, '\n');
}

// --FUNCTIONS FOR PROFILES--
void getOpponentLine(const char line[], OpponentStats &opponent) {
    int i = 0, j = 0;

    while (line[i] != ':' && line[i] != '\0') {
        opponent.name[j++] = line[i++];
    }
    opponent.name[j] = '\0';
    i += 2;
    opponent.gamesPlayed = 0;
    while (line[i] >= '0' && line[i] <= '9') {
        opponent.gamesPlayed = opponent.gamesPlayed * 10 + (line[i] - '0');
        i++;
    }
    while (line[i] != '(' && line[i] != '\0') i++;
    i++;
    opponent.gamesWon = 0;
    while (line[i] >= '0' && line[i] <= '9') {
        opponent.gamesWon = opponent.gamesWon * 10 + (line[i] - '0');
        i++;
    }
}

bool loadFullProfile(const char username[], Stats &s, OpponentStats opponents[], int &opponentCount) {
    char fileName[MAX_USER_PASS_LEN];
    getProfileFileName(username, fileName);

    std::ifstream file(fileName);
    if (!file.is_open()) return false;

    char line[MAX_USER_PASS_LEN];

    file.getline(line, MAX_USER_PASS_LEN); //username
    file.getline(line, MAX_USER_PASS_LEN); //total games played
    s.gamesPlayed = readFirstNumberFromLine(line);

    file.getline(line, MAX_USER_PASS_LEN); //total games won
    s.gamesWon = readFirstNumberFromLine(line);

    file.getline(line, MAX_USER_PASS_LEN); //games against other players

    opponentCount = 0;
    while (file.getline(line, MAX_USER_PASS_LEN)) {
        getOpponentLine(line, opponents[opponentCount]);
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
    file << "Total games won: " << s.gamesWon << " (" << winPercent << "%)" << endl;
    file << "Games against other players (wins/%):" << endl;

    for (int i = 0; i < opponentCount; i++) {
        double p = (opponents[i].gamesPlayed > 0) ? (opponents[i].gamesWon * 100.0 / opponents[i].gamesPlayed) : 0.0;
        file << opponents[i].name << ": " << opponents[i].gamesPlayed << " games played (" << opponents[i].gamesWon << "/" << p << "% wins)" << endl;
    }

    file.close();
    return true;
}

bool userExists(const char logUsername[MAX_USER_PASS_LEN], const char logPassword[MAX_USER_PASS_LEN]) {
    if (isStrEmpty(logUsername)) return false;

    std::ifstream user;
    user.open("users.txt");
    if (!user.is_open()) return false;

    char username[MAX_USER_PASS_LEN], password[MAX_USER_PASS_LEN];

    while (user >> username >> password) {
        if (areStrEqual(username, logUsername)) {
            if (areStrEqual(password, logPassword)) {   
                user.close();
                return true;
            }
        }
    }

    user.close();
    return false;
}

bool registerUser(const char newUsername[MAX_USER_PASS_LEN], const char newPassword[MAX_USER_PASS_LEN]) {
    if (userExists(newUsername, newPassword)) {
        cout << "User already exists!" << endl;
        return false;
    }

    if (!validData(newUsername, newPassword)) {
        return false;
    }

    std::ofstream users;
    users.open("users.txt", std::ios::app);
    
    if (!users.is_open()) {
        cout << "Error opening user.txt file!" << endl;
        return false;
    }

    users << newUsername << " " << newPassword << endl;
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
        return false;
    }

    char fileUsername[MAX_USER_PASS_LEN], filePassword[MAX_USER_PASS_LEN];
    while (userFile >> fileUsername >> filePassword) {
        if (areStrEqual(fileUsername, logUsername) && areStrEqual(filePassword, logPassword)) {
            userFile.close();
            return true;
        }
    }

    userFile.close();
    return false;
}

// --FUNCTIONS FOR USING IN MAIN--
void printStartMenu() {
    cout << "------- PURE STRATEGY MENU -------" << endl;
    cout << "       1. Register new user" << endl;
    cout << "         2. Login user1" << endl;
    cout << "         3. Login user2" << endl;
    cout << "         4. Start game" << endl;
    cout << "          5. Exit game" << endl;
    cout << "----------------------------------" << endl;
    cout << "Enter your choice: ";
}

void showProfileStats(const char username[MAX_USER_PASS_LEN]) {
    Stats s;
    OpponentStats opponents[MAX_OPPONENTS];
    int opponentCount = 0;

    if (!loadFullProfile(username, s, opponents, opponentCount)) {
        cout << "Error loading profile for " << username << "!" << endl;
        return;
    }

    cout << "\n----- PROFILE: " << username << " -----" << endl;
    cout << "Total games played: " << s.gamesPlayed << endl;
    cout << "Total games won: " << s.gamesWon << endl;
    cout << "\n-- GAMES AGAINST OTHER PLAYERS --" << endl;
    if (s.gamesPlayed > 0) {
        cout << "Win rate: " << (s.gamesWon * 100.0 / s.gamesPlayed) << "%" << endl;
    }

    if (opponentCount > 0) {
        cout << "--RECORDS AGAINST OPPONENTS--" << endl;
        for (int i = 0; i < opponentCount; i++) {
            cout << opponents[i].name << ": " << opponents[i].gamesPlayed << " games played, " 
                 << opponents[i].gamesWon << " wins" << endl;
        }
    }
    cout << "-----------------------" << endl;
}

void loginMenu(char player1[MAX_USER_PASS_LEN], char player2[MAX_USER_PASS_LEN]) {
    int choice;
    while (true) {
        cout << "Logged in as: " << player1 << " and " << player2 << endl;
        cout << "------------- PURE STRATEGY LOBBY -------------" << endl;
        cout << "                1. Start game" << endl;
        cout << "         2. View player 1 statistics" << endl;
        cout << "         3. View player 2 statistics" << endl;
        cout << " 4. Log out from player 1 and return to main menu" << endl;
        cout << " 5. Log out from player 2 and return to main menu" << endl;
        cout << "                 6. Exit lobby" << endl;
        cout << "-----------------------------------------------" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(MAX_USER_PASS_LEN, '\n');
            continue;
        }

        cin.ignore(MAX_USER_PASS_LEN, '\n');
        if (choice == 1) {
            playGame(player1, player2);
        } 
        if (choice == 2) {
            showProfileStats(player1);
            waitAndClearScreen();
        }
        else if (choice == 3) {
            showProfileStats(player2);
            waitAndClearScreen();
        }
        else if (choice == 4) {
            strCpy(player1, "");
            cout << "Player1 logged out and returning to main menu..." << endl;
            break;
        }
        else if (choice == 5) {
            strCpy(player2, "");
            cout << "Player2 logged out and returning to main menu..." << endl;
            break;
        } 
        else if (choice == 6) {
            cout << "Exiting lobby. Goodbye!" << endl;
            break;
        }
        else {
            cout << "Invalid choice. Try again." << endl;
        }
    }
}

void runGame() {
    int menuChoice;
    char username1[MAX_USER_PASS_LEN] = "", password1[MAX_USER_PASS_LEN] = "";
    char username2[MAX_USER_PASS_LEN] = "", password2[MAX_USER_PASS_LEN] = "";

    while (true) {
        cout << "------------ SESSION --------------"  << endl;
        cout <<"Player1: " << (isStrEmpty(username1) ? "[Not logged in]" : username1) << endl;
        cout <<"Player2: " << (isStrEmpty(username2) ? "[Not logged in]" : username2) << endl;

        printStartMenu();
        if (!(cin >> menuChoice)) {
            cin.clear();
            cin.ignore(MAX_USER_PASS_LEN, '\n');
            cout << "Invalid choice. Try again." << endl;
            continue;
        }
        cin.ignore(MAX_USER_PASS_LEN, '\n');

        if (menuChoice == 1) {
            char regUsername[MAX_USER_PASS_LEN];
            char regPassword[MAX_USER_PASS_LEN];
            cout << "----------- REGISTER NEW ACCOUNT -----------" << endl;
            cout << "Enter new username: ";
            cin.getline(regUsername, MAX_USER_PASS_LEN);
            cout << "Enter new password: ";
            cin.getline(regPassword, MAX_USER_PASS_LEN);

            if (registerUser(regUsername, regPassword)) {
                cout << "Registration successful! You can now login." << endl;
            } else {
                cout << "Registration failed. Try again." << endl;
            }
        }
        else if (menuChoice == 2) {
            char tempUsername[MAX_USER_PASS_LEN];
            cout << "Player 1 -> Enter username: ";
            cin.getline(tempUsername, MAX_USER_PASS_LEN);

            if (areStrEqual(username2, tempUsername)) {
                cout << "Cannot log in the same profile as Player 2." << endl;
                continue;
            }

            cout << "Player 1 -> Enter password: ";
            cin.getline(password1, MAX_USER_PASS_LEN);

            if (!loginUser(tempUsername, password1)) {
                cout << "Login failed for player1. Check username/password. " << endl;
                continue;
            } 
            strCpy(username1, tempUsername);
            cout << "Player 1 logged in successfully!" << endl;
        }
        else if (menuChoice == 3) {
            char tempUsername[MAX_USER_PASS_LEN];
            cout << "Player 2 -> Enter username: ";
            cin.getline(tempUsername, MAX_USER_PASS_LEN);

            if (areStrEqual(username1, tempUsername)) {
                cout << "Cannot log in the same profile as Player 1." << endl;
                continue;
            }

            cout << "Player 2 -> Enter password: ";
            cin.getline(password2, MAX_USER_PASS_LEN);

            if (!loginUser(tempUsername, password2)) {
                cout << "Login failed for player2. Check username/password. " << endl;
                continue;
            } 
            strCpy(username2, tempUsername);
            cout << "Player 2 logged in successfully!" << endl;
        }  
        else if (menuChoice == 4) {
            cout << endl;
            if (isStrEmpty(username1) || isStrEmpty(username2)) {
                cout << "Error! Both players must be logged in to start the game!" << endl;
                cout << "Current player1: " << (isStrEmpty(username1) ? "[Not logged in]" : "OK") << endl;
                cout << "Current player2: " << (isStrEmpty(username2) ? "[Not logged in]" : "OK") << endl;
                //continue;
            }
            else {
                cout << "Entering game lobby..." << endl;
                cout << endl;
                loginMenu(username1, username2);
            }
        }
        else if (menuChoice == 5) {
            cout << "Exiting program. Goodbye!" << endl;
            break;
        }
        else {
            cout << "Invalid choice. Try again." << endl;
        }
        cout << endl;
    }
}

int main() {
    srand(time(0));
    runGame();
    return 0;
}
