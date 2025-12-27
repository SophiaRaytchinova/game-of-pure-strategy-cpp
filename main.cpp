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
#include <fstream>
#include <cstdlib>
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
    for (int i = 0; i < deck.size(); i++) {
        int randomIndex = rand() % deck.size();
        mySwap(deck[i], deck[randomIndex]);
    }
}


int main() {

    return 0;
}