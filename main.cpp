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
bool userExists(const std::string& usernames, const std::string& passwords) {
    std::ifstream file("user.txt"); //opens file
    if (!file.is_open()) { //checks if file is open
        cout << "Error opening file!" << endl;
        return false;
    }
    std::string username, password;
    while (file >> username >> password) { //reads the file line by line
        if (username == usernames && password == passwords) {
            file.close();
            return true; // user found
        }
        else return false; //user not found
    }
}

int main() {

    return 0;
}