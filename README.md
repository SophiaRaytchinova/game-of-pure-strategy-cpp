# Pure Strategy - Console Implementation

- A console-based implementation of the card game "Pure Strategy", developed as a final project for the **"Introduction to Programming"** course at **FMI** (Sofia University, Winter 2025–2026).

## The Game

- **Pure Strategy** is a tactical game of prediction. Two players compete locally for reward cards revealed from a central deck.
  * **The Decks**: Both players start with an identical hand of 13 cards (A, 2-10, J, Q, K).
  * **The Goal**: Outplay your opponent by choosing a higher card to win the current reward.
  * **Point System**:
    *Ace*: 1 point/ *Jack*: 11 points/ *Queen*: 12 points/ *King*: 13 points
    Numbers *2-10* are worth their face value.
  * **Tie Breaker**: If both players play the same card, the reward card stays on the table and is added to the prize for the next round.

## Project Overview

- This project focuses on clean C++ logic using standard libraries. It features a custom-built user authentication system and file-based data persistence.

## Key Features

  * **User Profiles**: Secure registration and login for two distinct players.
  * **Persistent Statistics**: Saves total games played and win rates to individual '.txt' files.
  * **Opponent Tracking**: Keeps a detailed record of wins and losses specifically against each opponent you've faced.
  * **Input Validation**: Trims spaces and handles invalid card entries to prevent crashes.
  * **Randomization**: Implements the Fisher-Yates shuffle algorithm for the reward deck.

## How to Play

  ### 1. Account Setup
  
  - When you first run the program, you will interact with the *Main Menu*.
  * **Register**: Use option '1' to create 2 new accounts. Your username and password cannot contain spaces.
  * **Login**: Both players must log in separately using options '2' (Player 1) and '3' (Player 2). The system will prevent you from logging into the same account twice in one session.
  
  ### 2. The Lobby
  
  * Once both players are authenticated, select option '4' to enter the Game Lobby. Here you can:
  * **View Stats**: See your total games played, overall win rate, and detailed records against specific opponents you have played before.
  * **Start Game**: Select option '1' to start the game.
  
  ### 3. Playing a Round
  
  * **Rounds**: The game consists of 13 rounds (one for each card in the deck).
  * **The reward**: A reward card is revealed from the shuffled deck (e.g., a "10" or "K").
  * **Choosing a card**: You must type the name of the card you want to play (e.g., 'A', '10', 'J'). The program is case-sensitive and will trim any accidental spaces you type.
    *Player 1* chooses a card from their hand (hidden from Player 2).
    *Player 2* chooses a card (hidden from Player 1).
  * **Winning the round**: The player who plays the higher value wins the reward. If it's a tie, the reward stays on the table for the next round.
  
  ### 4. Game Over & Persistence
  
  * **Victory**: The player with the highest total points from their collected rewards wins. If points are tied, the player who won the most physical cards is the winner. If they have an equal number of physical cards it is a complete tie and no one wins.
  * **Automatic saving**: Your statistics are updated and saved to your personal '.txt' file immediately after the game ends.
