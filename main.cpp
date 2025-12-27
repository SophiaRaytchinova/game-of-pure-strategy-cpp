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

int cardPoints (char card) {
    if (card >= '0' && card <= '9') {
    	return card;
    }
    else if (card == 'A') {
        return 1;
    }
    else if (card == 'J') {
        return 11;
    }
    else if (card == 'Q') {
        return 12;
    }
    else if (card == 'K') {
        return 13;
    }
}


int main() {

    return 0;
}