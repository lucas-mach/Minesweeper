#pragma once
#include "board.h"

//Open Welcome Window
void StartGame();

//Reads file from boardconfig.txt and takes in rows, cols, minecount
void ReadFile(int& row, int& col, int& mineCount);

