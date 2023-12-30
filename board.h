#pragma once
#include "tile.h"
#include <vector>

//#include <SFML/Audio.hpp>

#include <random>
#include <SFML\Graphics\>
#include <string>
#include <chrono>
#include <fstream>
#include <iostream>
using namespace std;

//A person who won the game
struct Person
{
	string name;	//their name
	int seconds;	//their time completion
	Person(string name, int seconds)
	{
		this->name = name;
		this->seconds = seconds;
	}
};



class Board
{
	//Class variables
	vector<vector<Tile>> minefield;		//2d vector of tiles
	vector<Person> Winners;		//vector of Winners 
	string userName;	//Name of user
	int rows;	//# of rows
	int cols;	//# of cols
	int mineCount;	//# of mines
	int WIDTH;	//cols * 32
	int HEIGHT;	//rows * 32 + 100
	bool debugMode = false;	//if debug mode is enabled or not
	bool gameLost = false;	//if game is lost
	bool gameWon = false;	//if game is won
	bool gamePause = false;	//if game is paused
	bool leaderboard_open = false;	//if leaderboard is open
	int clicks = 0;	//used for initial click, makes sure you can not click on a bomb initially
	int mines_minus_flags;	//Used for minecounter ui
	int seconds = 0;	//Keeps track of overall seconds
	int seconds2 = 0;	//Keeps track of seconds while in debug or leaderboard mode
	float lost_time = 0;	//Adds the seconds2, and to calculate the seconds in game mode, subtract seconds an seconds2
	
	int only_once = 0;	//temp variable so a function only runs once in one game
	int only_once_ = 0;	//temp variable so a function only runs once in one game
	int first_run = 0;	//For the clock, so the statement only runs as the game window is opened, if not then the clock will run every iteration and seconds would be "0"
	int first_run_ = 0;	//For the clock, so the statement only runs if clicked pause or leaderboard ui

	//Sprites
public:
	sf::Sprite tile_hidden;
	sf::Sprite face_happy;
	sf::Sprite mine;
	sf::Sprite tile_revealed;
	sf::Sprite flag;
	sf::Sprite num_1;
	sf::Sprite num_2;
	sf::Sprite num_3;
	sf::Sprite num_4;
	sf::Sprite num_5;
	sf::Sprite num_6;
	sf::Sprite num_7;
	sf::Sprite num_8;
	sf::Sprite debug;
	sf::Sprite pause;
	sf::Sprite play;
	sf::Sprite leaderboard;
	sf::Sprite face_lost;
	sf::Sprite face_win;
	sf::Sprite digits_counter;

public:
	
	Board(int &rows, int &cols, int &mineCount, string &userName, vector<Person> &Winners);		//Constructor
	void DrawGameWindow();	//Draw Game Window
	void AddMines();	//Add Mines to board
	void AdjacentTiles();	//Calculate "surrounding mines" for each tile
	void ClickTile(int& curCol, int& curRow, bool &leftClick);	//reveals the tile accordingly
	int CheckIfButtonPressed(int &x, int &y);	//Checks if a ui button is clicked
	void ResetBoard();	//Resets the board
	void RevealTiles(Tile& tile);	//If tiles with no surrounding mines are connected, reveal them all, recursive function
	void CheckIfWon();	//Check if Game Won
	float Timer(chrono::time_point<chrono::system_clock> &start);	//Keeps track of seconds
	void WriteData();	//Write data into leaderboard file
	void OpenLeaderboard();		//Open leaderboard window
};

void ReadLeaderBoardFile(vector<Person>& Winners);	//Not apart of board class, reads the data from leaderboard.txt