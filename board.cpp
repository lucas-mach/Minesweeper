#include "board.h"

//Constructor
Board::Board(int& rows, int& cols, int& mineCount, string &userName, vector<Person> &Winners)
{
	//Initialize class variables and creates a vector of vector<tile> to store tiles in
	this->rows = rows;
	this->cols = cols;
	this->mineCount = mineCount;
	this->WIDTH = cols * 32;
	this->HEIGHT = rows * 32 + 100;
	this->userName = userName;
	this->mines_minus_flags = mineCount;	//for mineCounter
	this->Winners = Winners;

	
	//Add default tiles into 2d vector, unrevealed
	for (int i = 0; i < cols; i++)
	{
		vector<Tile> temp;
		for (int j = 0; j < rows; j++)
		{
			Tile _tile;
			temp.push_back(_tile);
		}
		this->minefield.push_back(temp);
	}	
}


//Opens Game Window
void Board::DrawGameWindow()
{
	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Minesweeper");

	//Initialize the Sprites
	//Gray Square
	sf::Texture texture;
	texture.loadFromFile("images/tile_hidden.png");
	this->tile_hidden.setTexture(texture, true);

	//Happy face
	sf::Texture _texture;
	_texture.loadFromFile("images/face_happy.png");
	face_happy.setTexture(_texture, true);
	face_happy.setPosition(((cols / 2.0) * 32) - 32, 32 * (rows + 0.5f));

	//Mine
	sf::Texture __texture;
	__texture.loadFromFile("images/mine.png");
	mine.setTexture(__texture, true);

	//Revealed Tile
	sf::Texture ___texture;
	___texture.loadFromFile("images/tile_revealed.png");
	tile_revealed.setTexture(___texture, true);

	//Flag
	sf::Texture ____texture;
	____texture.loadFromFile("images/flag.png");
	flag.setTexture(____texture, true);

	//Numbers
	sf::Texture texture1;
	texture1.loadFromFile("images/number_1.png");
	num_1.setTexture(texture1, true);

	sf::Texture texture2;
	texture2.loadFromFile("images/number_2.png");
	num_2.setTexture(texture2, true);

	sf::Texture texture3;
	texture3.loadFromFile("images/number_3.png");
	num_3.setTexture(texture3, true);

	sf::Texture texture4;
	texture4.loadFromFile("images/number_4.png");
	num_4.setTexture(texture4, true);

	sf::Texture texture5;
	texture5.loadFromFile("images/number_5.png");
	num_5.setTexture(texture5, true);

	sf::Texture texture6;
	texture6.loadFromFile("images/number_6.png");
	num_6.setTexture(texture6, true);

	sf::Texture texture7;
	texture7.loadFromFile("images/number_7.png");
	num_7.setTexture(texture7, true);

	sf::Texture texture8;
	texture8.loadFromFile("images/number_8.png");
	num_8.setTexture(texture8, true);


	//Debug Button
	sf::Texture texture_debug;
	texture_debug.loadFromFile("images/debug.png");
	debug.setTexture(texture_debug, true);
	debug.setPosition(cols * 32 - 304, 32 * (rows + 0.5f));

	//Pause/Play Buttons
	sf::Texture texture_pause;
	texture_pause.loadFromFile("images/pause.png");
	pause.setTexture(texture_pause, true);
	pause.setPosition((cols * 32) - 240, 32 * (rows + 0.5f));

	sf::Texture texture_play;
	texture_play.loadFromFile("images/play.png");
	play.setTexture(texture_play, true);
	play.setPosition((cols * 32) - 240, 32 * (rows + 0.5f));

	//Leaderboard Button
	sf::Texture texture_leaderboard;
	texture_leaderboard.loadFromFile("images/leaderboard.png");
	leaderboard.setTexture(texture_leaderboard, true);
	leaderboard.setPosition((cols * 32) - 176, 32 * (rows + 0.5f));

	//Face Lose
	sf::Texture texture_unhappy;
	texture_unhappy.loadFromFile("images/face_lose.png");
	face_lost.setTexture(texture_unhappy, true);
	face_lost.setPosition(((cols / 2.0) * 32) - 32, 32 * (rows + 0.5f));

	//Face Win
	sf::Texture texture_win;
	texture_win.loadFromFile("images/face_win.png");
	face_win.setTexture(texture_win, true);
	face_win.setPosition(((cols / 2.0) * 32) - 32, 32 * (rows + 0.5f));

	//Digits
	sf::Texture texture_digits;
	texture_digits.loadFromFile("images/digits.png");
	digits_counter.setTexture(texture_digits, true);
	//Music :)
	/*sf::SoundBuffer buffer;
	buffer.loadFromFile("sound.wav");
	sf::Sound sound;
	sound.setBuffer(buffer);
	sound.play();*/

	/*sf::SoundBuffer buffer_;
	buffer_.loadFromFile("loser.wav");
	sf::Sound loser_sound;
	loser_sound.setBuffer(buffer_);*/


	//Add randomized mines and calculate surrounding mines for each Tile
	AddMines();
	AdjacentTiles();
	
	//initialize the start for start of game, and during pause and leaderboard times
	chrono::time_point<chrono::system_clock> start;
	chrono::time_point<chrono::system_clock> start1;

	//Open Game Window
	while (window.isOpen())
	{
		/*if (sound.getStatus() == sf::Music::Status::Stopped)
		{
			sound.play();
		}*/
		//Keeps track of time, including when game is paused
		if (clicks > 0 and !gameWon and !gameLost)
		{
			if (first_run == 0)
			{
				start = chrono::system_clock::now();
				first_run++;
			}
			if (!gamePause and !leaderboard_open)
			{

				seconds = round(Timer(start) - lost_time);
				if (seconds < 0)
				{
					seconds = 0;
				}
			}
			else
			{
				if (first_run_ == 0)
				{
					start1 = chrono::system_clock::now();
					first_run_++;
				}
				seconds2 = Timer(start1);
			}
		}
		
		
		
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)	//Close Window
				window.close();


			if (event.type == sf::Event::MouseButtonPressed)
			{

				bool leftClick;
				
				//Used for coordinates for UI buttons
				int x = WIDTH * sf::Mouse::getPosition(window).x / window.getSize().x;
				int y = HEIGHT * sf::Mouse::getPosition(window).y / window.getSize().y;
				
				//Used for clicking tiles
				int curRow = sf::Mouse::getPosition(window).y / ((window.getSize().y - (100.0 / HEIGHT * window.getSize().y)) / rows);
				int curCol = sf::Mouse::getPosition(window).x / (window.getSize().x / cols);
				
				if (clicks == 0 and curRow < rows and curCol < cols) {
					while (minefield[curCol][curRow].surroundingMines > 0 or minefield[curCol][curRow].mine)	//If first click is a mine or number, reset board until good
					{
						ResetBoard();
					}
					RevealTiles(minefield[curCol][curRow]);
					clicks++;

				}
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					leftClick = true;
					if (CheckIfButtonPressed(x, y) == 1 and !gamePause)	//If debug UI is clicked, enable debug mode
					{
						if (!debugMode)
						{
							debugMode = true;
						}
						else
						{
							debugMode = false;
						}
					}
					else if (CheckIfButtonPressed(x, y) == 2)	//If face UI is clicked, reset the board
					{
						ResetBoard();
					}
					else if (CheckIfButtonPressed(x, y) == 3 and !gameWon and !gameLost)	//If pause UI is clicked, pause game, including timer
					{
						if (!gamePause)
						{
							first_run_ = 0;
							gamePause = true;
						}
						else
						{
							lost_time += seconds2;
							gamePause = false;
						}
					}
					else if (CheckIfButtonPressed(x, y) == 4)	//If leaderboard UI is clicked, open leaderboard window
					{	
						leaderboard_open = true;
					}
				}
				else if (event.mouseButton.button == sf::Mouse::Right)
				{
					leftClick = false;
				}
				if (!gameLost and !gameWon and !gamePause and !leaderboard_open)
				{
					ClickTile(curCol, curRow, leftClick);
					CheckIfWon();	//Check is won after every click
				}
			}
		}

		window.clear(sf::Color::White);

		//Draw Board
		
		



		//Draw board regularly, no pause or leaderboard UI clicked
		if (!gamePause and !leaderboard_open)
		{
			for (unsigned int i = 0; i < minefield.size(); i++)
			{
				for (unsigned int j = 0; j < minefield[i].size(); j++)
				{
					if (minefield[i][j].unrevealed)
					{
						tile_hidden.setPosition(i * 32, j * 32);
						window.draw(tile_hidden);
						if (minefield[i][j].flag and !gameLost)
						{
							flag.setPosition(i * 32, j * 32);
							window.draw(flag);
						}
					}
					else
					{
						tile_revealed.setPosition(i * 32, j * 32);
						window.draw(tile_revealed);
						if (minefield[i][j].mine)
						{
							gameLost = true;
						}
						if (minefield[i][j].surroundingMines > 0 and !minefield[i][j].mine)
						{
							if (minefield[i][j].surroundingMines == 1)
							{
								num_1.setPosition(i * 32, j * 32);
								window.draw(num_1);
							}
							else if (minefield[i][j].surroundingMines == 2)
							{
								num_2.setPosition(i * 32, j * 32);
								window.draw(num_2);
							}
							else if (minefield[i][j].surroundingMines == 3)
							{
								num_3.setPosition(i * 32, j * 32);
								window.draw(num_3);
							}
							else if (minefield[i][j].surroundingMines == 4)
							{
								num_4.setPosition(i * 32, j * 32);
								window.draw(num_4);
							}
							else if (minefield[i][j].surroundingMines == 5)
							{
								num_5.setPosition(i * 32, j * 32);
								window.draw(num_5);
							}
							else if (minefield[i][j].surroundingMines == 6)
							{
								num_6.setPosition(i * 32, j * 32);
								window.draw(num_6);
							}
							else if (minefield[i][j].surroundingMines == 7)
							{
								num_7.setPosition(i * 32, j * 32);
								window.draw(num_7);
							}
							else if (minefield[i][j].surroundingMines == 8)
							{
								num_8.setPosition(i * 32, j * 32);
								window.draw(num_8);
							}
						}
					}
					if (gameWon)
					{
						for (int x = 0; x < minefield.size(); x++)
						{
							for (int y = 0; y < minefield[x].size(); y++)
							{
								if (minefield[x][y].mine)
								{
									flag.setPosition(x * 32, y * 32);
									window.draw(flag);
								}
							}
						}
					}
					if (gameLost)
					{
						for (int x = 0; x < minefield.size(); x++)
						{
							for (int y = 0; y < minefield[x].size(); y++)
							{
								if (minefield[x][y].mine)
								{
									mine.setPosition(x * 32, y * 32);
									window.draw(mine);
								}
							}
						}
					}
					//If debug UI clicked, draw mines onto board
					if (debugMode and !gameLost and !gameWon)
					{
						for (int i = 0; i < minefield.size(); i++)
						{
							for (int j = 0; j < minefield[i].size(); j++)
							{
								if (minefield[i][j].mine)
								{
									mine.setPosition(i * 32, j * 32);
									window.draw(mine);
								}
							}
						}
					}
				}
			}
		}
		else if (gamePause or leaderboard_open)	//if game is paused, or leaderboard is open, draw unrevealed tiles across board
		{
			for (int i = 0; i < minefield.size(); i++)
			{
				for (int j = 0; j < minefield[i].size(); j++)
				{
					tile_revealed.setPosition(i * 32, j * 32);
					window.draw(tile_revealed);					
				}
			}		
		}
	

		
		//Adds winners to vector in order
		//Write the data
		if (gameWon and only_once == 0)
		{

			only_once++;
			if (Winners.size() <= 5)
			{
				int times_run = Winners.size();	//Gets the Winners.size() before the winner is added,
				bool temp = false;
				Person person(userName, seconds);	//Create a object Person with name and time saved
				
				for (int i = 0; i < times_run; i++)
				{
					if (this->seconds < Winners[i].seconds)
					{
						if (Winners.size() < 5)
						{
							vector<Person>::iterator iter = Winners.begin() + i;
							Winners.insert(iter, person);
							temp = true;
							break;
						}
						else
						{
							vector<Person>::iterator iter = Winners.begin() + i;
							Winners.insert(iter, person);
							Winners.pop_back();
							temp = true;
							break;
						}
					}
				}
				if (!temp and times_run == 0)
				{
					Winners.push_back(person);					
				}
				else if (!temp and times_run > 0 and times_run < 5)
				{
					Winners.push_back(person);
				}
				else if (!temp and times_run == 5 and this->seconds < Winners[4].seconds)
				{
					Winners[4].name = userName;
					Winners[4].seconds = this->seconds;
				}
				WriteData();	//Write data into file after a winner is added to the list
			}			
		}

		//Draw UI Buttons
		if (!gameLost and !gameWon)
		{
			window.draw(face_happy);
		}
		else if (gameLost)
		{			
			window.draw(face_lost);
		}
		else if (gameWon)
		{
			window.draw(face_win);
		}	
		//Draw the Mine Counter
		int temp = mines_minus_flags;
		//If positive number
		if (mines_minus_flags >= 0)
		{
			for (int i = 2; i >= 0; i--)
			{
				digits_counter.setTextureRect(sf::IntRect((temp % 10) * 21, 0, 21, 32));
				digits_counter.setPosition(33 + 21 * i, 32 * (rows + 0.5f) + 16);
				window.draw(digits_counter);
				temp /= 10;
			}
		}
		//If negative number
		else
		{
			digits_counter.setPosition(12, 32 * (rows + 0.5f) + 16);
			digits_counter.setTextureRect(sf::IntRect(10 * 21, 0, 21, 32));
			window.draw(digits_counter);
			temp = mines_minus_flags * -1;
			for (int i = 2; i >= 0; i--)
			{
				digits_counter.setTextureRect(sf::IntRect((temp % 10) * 21, 0, 21, 32));
				digits_counter.setPosition(33 + 21 * i, 32 * (rows + 0.5f) + 16);
				window.draw(digits_counter);
				temp /= 10;
			}
		}
		
		//Draw Timer
		//If seconds under a minute
		if (seconds < 60)
		{
			digits_counter.setTextureRect(sf::IntRect(0 * 21, 0, 21, 32));
			digits_counter.setPosition((cols * 32) - 97, 32 * (rows + 0.5f) + 16);
			window.draw(digits_counter);
			digits_counter.setPosition((cols * 32) - 97 + 21, 32 * (rows + 0.5f) + 16);
			window.draw(digits_counter);
			int temp = seconds;
			for (int i = 1; i >= 0; i--)
			{
				digits_counter.setTextureRect(sf::IntRect((temp % 10) * 21, 0, 21, 32));
				digits_counter.setPosition((cols * 32) - 54 + (21 * i), 32 * (rows + 0.5f) + 16);
				window.draw(digits_counter);
				temp /= 10;
			}
		}
		//If seconds greater than a minute
		else
		{
			int minutes = seconds / 60;
			int temp = seconds % 60;
			for (int i = 1; i >= 0; i--)
			{
				digits_counter.setTextureRect(sf::IntRect((minutes % 10) * 21, 0, 21, 32));
				digits_counter.setPosition((cols * 32) - 97 + (21 * i), 32 * (rows + 0.5f) + 16);
				window.draw(digits_counter);
				minutes /= 10;
			}
			for (int i = 1; i >= 0; i--)
			{
				digits_counter.setTextureRect(sf::IntRect((temp % 10) * 21, 0, 21, 32));
				digits_counter.setPosition((cols * 32) - 54 + (21 * i), 32 * (rows + 0.5f) + 16);
				window.draw(digits_counter);
				temp /= 10;
			}

		}
		window.draw(debug);
		if (!gamePause)
		{
			window.draw(pause);
		}
		else
		{
			window.draw(play);
		}
		window.draw(leaderboard);
		window.display();
		if (leaderboard_open)
		{
			OpenLeaderboard();
		}
		if (gameWon and only_once_ == 0)
		{
			only_once_++;
			OpenLeaderboard();
		}
	}
}

//Add mines to the board and makes sure it is exactly 50
void Board::AddMines()
{
	random_device rd;
	mt19937 rng(rd());
	uniform_int_distribution<int> uni(0, rows - 1);
	uniform_int_distribution<int> _uni(0, cols - 1);
	
	for (int i = 0; i < mineCount; i++)
	{
		int random_row = uni(rng);
		int random_col = _uni(rng);
		
		while (minefield[random_col][random_row].mine)
		{
			int random_row = uni(rng);
			int random_col = _uni(rng);
			if (!minefield[random_col][random_row].mine)
			{
				minefield[random_col][random_row].mine = true;
				break;
			}
		}
		minefield[random_col][random_row].mine = true;
	}
}

void Board::AdjacentTiles()	//Calculates the "surrounding mines" in each tile using all posible cases in postion
{
	//cols = 25
	//rows = 16

	for (int i = 0; i < minefield.size(); i++)
	{
		for (int j = 0; j < minefield[i].size(); j++)
		{
			//Top left corner
			if (i == 0 and j == 0)
			{
				minefield[i][j].adjacent_tiles.push_back(&minefield[i + 1][j]);
				minefield[i][j].adjacent_tiles.push_back(&minefield[i + 1][j + 1]);
				minefield[i][j].adjacent_tiles.push_back(&minefield[i][j + 1]);
			}
			//Top right corner
			else if (i == cols - 1 and j == 0)
			{
				minefield[i][j].adjacent_tiles.push_back(&minefield[i - 1][j]);
				minefield[i][j].adjacent_tiles.push_back(&minefield[i - 1][j + 1]);
				minefield[i][j].adjacent_tiles.push_back(&minefield[i][j + 1]);
			}
			//Bottom Left corner
			else if (i == 0 and j == rows - 1)
			{
				minefield[i][j].adjacent_tiles.push_back(&minefield[i + 1][j]);
				minefield[i][j].adjacent_tiles.push_back(&minefield[i + 1][j - 1]);
				minefield[i][j].adjacent_tiles.push_back(&minefield[i][j - 1]);
			}
			//Bottom right corner
			else if (i == cols - 1 and j == rows - 1)
			{
				minefield[i][j].adjacent_tiles.push_back(&minefield[i - 1][j]);
				minefield[i][j].adjacent_tiles.push_back(&minefield[i - 1][j - 1]);
				minefield[i][j].adjacent_tiles.push_back(&minefield[i][j - 1]);
			}
			//Left most column
			else if (i == 0 and j != 0 and j != rows - 1)
			{
				minefield[i][j].adjacent_tiles.push_back(&minefield[i][j - 1]);
				minefield[i][j].adjacent_tiles.push_back(&minefield[i + 1][j - 1]);
				minefield[i][j].adjacent_tiles.push_back(&minefield[i + 1][j]);
				minefield[i][j].adjacent_tiles.push_back(&minefield[i + 1][j + 1]);
				minefield[i][j].adjacent_tiles.push_back(&minefield[i][j + 1]);
			}
			//Right most column
			else if (i == cols - 1 and j != 0 and j != rows - 1)
			{
				minefield[i][j].adjacent_tiles.push_back(&minefield[i][j - 1]);
				minefield[i][j].adjacent_tiles.push_back(&minefield[i - 1][j - 1]);
				minefield[i][j].adjacent_tiles.push_back(&minefield[i - 1][j]);
				minefield[i][j].adjacent_tiles.push_back(&minefield[i - 1][j + 1]);
				minefield[i][j].adjacent_tiles.push_back(&minefield[i][j + 1]);
			}
			//Top most row
			else if (j == 0 and i != 0 and i != cols - 1)
			{
				minefield[i][j].adjacent_tiles.push_back(&minefield[i - 1][j]);
				minefield[i][j].adjacent_tiles.push_back(&minefield[i - 1][j + 1]);
				minefield[i][j].adjacent_tiles.push_back(&minefield[i][j + 1]);
				minefield[i][j].adjacent_tiles.push_back(&minefield[i + 1][j + 1]);
				minefield[i][j].adjacent_tiles.push_back(&minefield[i + 1][j]);
			}
			//Bottom most row
			else if (j == rows - 1 and i != 0 and i != cols - 1)
			{
				minefield[i][j].adjacent_tiles.push_back(&minefield[i - 1][j]);
				minefield[i][j].adjacent_tiles.push_back(&minefield[i - 1][j - 1]);
				minefield[i][j].adjacent_tiles.push_back(&minefield[i][j - 1]);
				minefield[i][j].adjacent_tiles.push_back(&minefield[i + 1][j - 1]);
				minefield[i][j].adjacent_tiles.push_back(&minefield[i + 1][j]);
			}
			//Any tile in the middle, has 8 surrounding tiles
			else
			{
				minefield[i][j].adjacent_tiles.push_back(&minefield[i - 1][j - 1]);
				minefield[i][j].adjacent_tiles.push_back(&minefield[i][j - 1]);
				minefield[i][j].adjacent_tiles.push_back(&minefield[i + 1][j - 1]);
				minefield[i][j].adjacent_tiles.push_back(&minefield[i + 1][j]);
				minefield[i][j].adjacent_tiles.push_back(&minefield[i + 1][j + 1]);
				minefield[i][j].adjacent_tiles.push_back(&minefield[i][j + 1]);
				minefield[i][j].adjacent_tiles.push_back(&minefield[i - 1][j + 1]);
				minefield[i][j].adjacent_tiles.push_back(&minefield[i - 1][j]);
			}
		}
	}
	for (int i = 0; i < minefield.size(); i++)
	{
		for (int j = 0; j < minefield[i].size(); j++)
		{
			for (int k = 0; k < minefield[i][j].adjacent_tiles.size(); k++)
			{
				if (minefield[i][j].adjacent_tiles[k]->mine)
				{
					minefield[i][j].surroundingMines++;
				}
			}
		}
	}
}

void Board::RevealTiles(Tile &tile)	//Recursive function to reveal multiple tiles at once if surrounding mines = 0
{
	if (tile.surroundingMines > 0 or tile.mine)
	{		
		tile.unrevealed = false;
		return;
	}


	if (tile.unrevealed and tile.surroundingMines == 0)
	{
		tile.unrevealed = false;
		for (int i = 0; i < tile.adjacent_tiles.size(); i++)
		{
			RevealTiles(*tile.adjacent_tiles[i]);
		}
	}
	else
	{
		return;
	}
}

void Board::ClickTile(int &curCol, int &curRow, bool &leftClick)	//Click tile functions, adds flags, and calls Reveal tiles function if tile is unrevealed
{
	// if the clicked location is a tile
	if (curCol < cols and curRow < rows)
	{
		// if the current tile is unrevealed and is not a flag
		if (minefield[curCol][curRow].unrevealed and leftClick and !minefield[curCol][curRow].flag)
		{
			RevealTiles(minefield[curCol][curRow]);
		}
		// add and remove flags
		else if (minefield[curCol][curRow].unrevealed and !leftClick and !minefield[curCol][curRow].flag)
		{			
			minefield[curCol][curRow].flag = true;
			mines_minus_flags--;
		}
		else if (minefield[curCol][curRow].flag and !leftClick and minefield[curCol][curRow].unrevealed)
		{
			minefield[curCol][curRow].flag = false;
			mines_minus_flags++;
		}
	}
}


//return 1 = debug button    return 2 = reset button	return 3 = pause button		return 4 = leaderboard button
int Board::CheckIfButtonPressed(int& x, int& y)
{
	//Debug Button
	if (x >= (cols * 32) - 304 and x <= ((cols * 32) - 304) + debug.getTexture()->getSize().x and y >= 32 * (rows + 0.5) and y <= 32 * (rows + 0.5) + debug.getTexture()->getSize().y)
	{
		return 1;
	}
	//Reset button
	if (x >= ((cols / 2.0) * 32) - 32 and x <= ((cols / 2.0) * 32) - 32 + face_happy.getTexture()->getSize().x and y >= 32 * (rows + 0.5) and y <= 32 * (rows + 0.5) + face_happy.getTexture()->getSize().y)
	{
		return 2;
	}
	if (x >= (cols * 32) - 240 and x <= (cols * 32) - 240 + pause.getTexture()->getSize().x and y >= 32 * (rows + 0.5) and y <= 32 * (rows + 0.5) + pause.getTexture()->getSize().y)
	{	
		return 3;
	}
	if (x >= (cols * 32) - 176 and x <= (cols * 32) - 176 + leaderboard.getTexture()->getSize().x and y >= 32 * (rows + 0.5f) and y <= 32 * (rows + 0.5f) + leaderboard.getTexture()->getSize().y)
	{
		return 4;
	}
}

//Reset the board, reset all variables used in the game
void Board::ResetBoard()
{
	for (int i = 0; i < minefield.size(); i++)
	{
		for (int j = 0; j < minefield[i].size(); j++)
		{
			minefield[i][j].mine = false;
			minefield[i][j].flag = false;
			minefield[i][j].unrevealed = true;
			minefield[i][j].surroundingMines = 0;
			minefield[i][j].adjacent_tiles.clear();

		}
	}
	only_once_ = 0;
	leaderboard_open = false;
	only_once = 0;
	seconds = 0;
	seconds2 = 0;
	first_run = 0;
	first_run_ = 0;
	lost_time = 0;
	mines_minus_flags = mineCount;
	clicks = 0;
	gameWon = false;
	gameLost = false;
	AddMines();
	AdjacentTiles();	
	debugMode = false;
	gamePause = false;
}

//Checks to see if game is won
void Board::CheckIfWon()
{
	int count = 0;
	for (int i = 0; i < minefield.size(); i++)
	{
		for (int j = 0; j < minefield[i].size(); j++)
		{
			if (!minefield[i][j].mine)
			{
				if (!minefield[i][j].unrevealed)
				{
					count++;
				}
			}
		}
	}
	if (count == (rows * cols - mineCount))
	{
		gameWon = true;
	}
}

//Calculates end time - parameterized inputed time
 float Board::Timer(chrono::time_point<chrono::system_clock> &start)
{
	chrono::time_point<chrono::system_clock> end = chrono::system_clock::now();
	chrono::duration<double> elapsed_seconds = end - start;
	
	return elapsed_seconds.count();
}


 //Write the leaderboard data into the file
 void Board::WriteData()
 {
	 vector<string> people;
	 for (int i = 0; i < Winners.size(); i++)
	 {
		 string temp = "";
		 int minutes = Winners[i].seconds / 60;
		 int seconds_ = Winners[i].seconds - (minutes * 60);
		 
		 string temp_ = Winners[i].name;
		 if (Winners[i].name[Winners[i].name.size() - 1] == '*')
		 {
			 string temp_ = Winners[i].name.substr(0, Winners[i].name.size() - 1);
		 }
		 if (minutes < 10)
		 {
			 if (seconds_ < 10)
			 {
				 temp += "0" + to_string(minutes) + ":" + "0" + to_string(seconds_) + "," + temp_;
			 }
			 else
			 {
				 temp += "0" + to_string(minutes) + ":" + to_string(seconds_) + "," + temp_;
			 }
			 
		 }
		 else 
		 {
			 if (seconds_ < 10)
			 {
				 temp += to_string(minutes) + ":" + "0" + to_string(seconds_) + "," + temp_;
			 }
			 else
			 {
				 temp += to_string(minutes) + ":" + to_string(seconds_) + "," + temp_;
			 }			 
		 }		 
		 people.push_back(temp);				 
	 }
	 fstream file;
	 file.open("leaderboard.txt", ios_base::out);
	 if (file.is_open())
	 {
		 for (int i = 0; i < people.size(); i++)
		 {
			 file << people[i] << endl;
		 }
	 }
	 file.close();
 }

 //Open Leaderboard window
 void Board::OpenLeaderboard()
 {
	 int LWIDTH = cols * 16;
	 int LHEIGHT = (rows * 16) + 50;
	 
	 sf::RenderWindow window(sf::VideoMode(LWIDTH, LHEIGHT), "Minesweeper");
	 sf::Font font;
	 if (!font.loadFromFile("font.ttf"))
	 {
		 cout << "Error opening font file." << endl;
	 }
	 sf::Text LEADERBOARD;
	 LEADERBOARD.setFont(font);
	 LEADERBOARD.setString("LEADERBOARD");
	 LEADERBOARD.setStyle(sf::Text::Bold | sf::Text::Underlined);
	 LEADERBOARD.setOutlineColor(sf::Color::White);
	 LEADERBOARD.setCharacterSize(20);
	 LEADERBOARD.setPosition(LWIDTH / 2.0f, LHEIGHT / 2.0f - 120);
	 sf::FloatRect bounds = LEADERBOARD.getLocalBounds();
	 LEADERBOARD.setOrigin(bounds.width / 2, bounds.height / 2);

	 //Contents
	 sf::Text text;
	 string contents = "";
	 int counter = 0;
	 for (int i = 0; i < Winners.size(); i++)
	 {
		 string temp = "";
		 int minutes = Winners[i].seconds / 60;
		 int seconds = Winners[i].seconds % 60;
		 
		 if (minutes < 10)
		 {
			 if (seconds < 10)
			 {
				 temp += to_string(i + 1) + ".      " + '0' + to_string(minutes) + ':' + '0' + to_string(seconds) + "     " + Winners[i].name;
			 }
			 else
			 {
				 temp += to_string(i + 1) + ".      " + '0' + to_string(minutes) + ':' + to_string(seconds) + "     " + Winners[i].name;
			 }
		 }
		 else
		 {
			 if (seconds < 10)
			 {
				 temp += to_string(i + 1) + ".      " + to_string(minutes) + ':' + '0' + to_string(seconds) + "     " + Winners[i].name;
			 }
			 else
			 {
				 temp += to_string(i + 1) + ".      " + to_string(minutes) + ':' + to_string(seconds) + "     " + Winners[i].name;
			 }
		 }
		 if (Winners[i].seconds == this->seconds and Winners[i].name == userName and counter == 0)
		 {
			 temp += '*';
			 counter++;
		 }
		 temp += "\n\n";
		 contents += temp;		 
	 }
	 text.setString(contents);
	 text.setFont(font);
	 text.setStyle(sf::Text::Bold);
	 text.setOutlineColor(sf::Color::White);
	 text.setCharacterSize(18);
	 text.setPosition(LWIDTH / 2.0f, LHEIGHT / 2.0f + 20);
	 sf::FloatRect bounds_ = text.getLocalBounds();
	 text.setOrigin(bounds_.width / 2, bounds_.height / 2);
	 
	 chrono::time_point<chrono::system_clock> start = chrono::system_clock::now();
	 while (window.isOpen())
	 {
		 sf::Event event;
		 while (window.pollEvent(event))
		 {
			 if (event.type == sf::Event::Closed)
				 window.close();
		 }
		 window.clear(sf::Color::Blue);
		 window.draw(LEADERBOARD);
		 window.draw(text);
		 window.display();
	 }
	 //After window closes
	 leaderboard_open = false;
	 int seconds = Timer(start);
	 lost_time += seconds;
 }

 //Not apart of board class, reads the data from the leaderboard.txt, used in WelcomeWindow.cpp, called before board is created
 void ReadLeaderBoardFile(vector<Person>& Winners)
 {
	 fstream file;
	 file.open("leaderboard.txt", ios_base::in);
	 string temp;
	 if (file.is_open())
	 {
		 while (file >> temp)
		 {
			 string minutes = temp.substr(0, 2);
			 string seconds = temp.substr(3, 2);
			 string name = temp.substr(6, temp.size() - 1);

			 int time = stoi(minutes) * 60 + stoi(seconds);

			 Person person(name, time);
			 Winners.push_back(person);
		 }
	 }
	 file.close();
 }