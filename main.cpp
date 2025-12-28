/*
*  
* Solution to course project #1
* Introduction to programming course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2025/2026
*
* @author Sophia Raytchinova
* @idnumber 5MI0600681
* @compiler VS
*
* <main.cpp>
*
*/

#include <iostream>
#include <fstream> // for file operations
#include <cstdlib> // for rand()
#include <vector>  // for dynamic arrays-hand, deck
#include <string> 
using std::cin;
using std::cout;
using std::endl;

// --Basic functions for cards--
//cardPoints function returns the points of a given card as an integer
int cardPoints(const std::string card) {
    if (card == "A") {
        return 1;
    }
    if (card == "J") {
        return 11;
    }
    if (card == "Q") {
        return 12;
    }
    if (card == "K") {
        return 13;
    }
    return std::stoi(card); // for cards 2-10, from string to integer->prints int
}
//implementation of swap function for strings
void mySwap(std::string& a, std::string& b) {
    std::string temp = a;
    a = b;
    b = temp;
}
//shuffleDeck function shuffles the deck of cards
void shuffleDeck(std::vector<std::string>& deck) {
    for (int i = deck.size() - 1; i > 0; i--) { //Fisher-Yates shuffle algorithm
        int randomIndex = rand() % (i + 1);
        mySwap(deck[i], deck[randomIndex]);
    }
}
//printHand function prints the cards you have in that moment
void printHand(const std::vector<std::string>& hand) {
    for (int i = 0; i< hand.size(); i++) {
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
    std::ifstream user("users.txt"); //opens file in read mode
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

bool registerUser(const std::string& newUsername, const std::string& newPassword) {
    if (newUsername.empty() || newPassword.empty()) {
        cout << "Username and password cannot be empty! Enter valid username and password. " << endl;
        return false;
    }
    if (userExists(newUsername)) {
        cout << "User already exists!" << endl;
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
    //append new user to user.txt
    std::ofstream users("users.txt", std::ios::app); //opens file in append mode, so we can add content without removing its current content
    if (!users.is_open()) { //checks if file is open
        cout << "Error opening user.txt file!" << endl;
        return false;
    }
    users << newUsername << " " << newPassword << endl; //writes username and password to file
    users.close();
    //createс a profile file for the new user
    std::ofstream profile(newUsername + ".txt", std::ios::app); //creates a new file for user statistics
    if (!profile.is_open()) {
        cout << "Error creating profile file!" << endl;
        return false;
    }
    profile << "Username: " << newUsername << endl;
    profile << "Total games played: 0" << endl;
    profile << "Total games won: 0 (0%)" << endl;
    profile << "Games against other players (wins/%): " << endl;
    profile.close();
    return true;
}

int main() {

    return 0;
}