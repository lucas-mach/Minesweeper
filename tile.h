#pragma once
#include <vector>
using namespace std;

//Struct so variables have easy access
struct Tile
{
	//Has a mine or not
	bool mine = false;

	//Has a flag or not
	bool flag = false;

	//Is revealed or not
	bool unrevealed = true;

	//number in the tile, could be up to 8
	int surroundingMines = 0;

	//stores tiles around it, ranges from 3-8 tiles
	vector<Tile*> adjacent_tiles;
};

