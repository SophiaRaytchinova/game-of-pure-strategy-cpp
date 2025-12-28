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
#include <vector> 
#include <string> 
using std::cin;
using std::cout;
using std::endl;

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

void mySwap(std::string& a, std::string& b) {
    std::string temp = a;
    a = b;
    b = temp;
}

void shuffleDeck(std::vector<std::string>& deck) {
    for (int i = deck.size() - 1; i > 0; i--) { //Fisher-Yates shuffle algorithm
        int randomIndex = rand() % (i + 1);
        mySwap(deck[i], deck[randomIndex]);
    }
}

void printHand(const std::vector<std::string>& hand) {
    for (int i = 0; i< hand.size(); i++) {
        cout << hand[i] << " ";
    }
    cout << endl;
}

int main() {

    return 0;
}