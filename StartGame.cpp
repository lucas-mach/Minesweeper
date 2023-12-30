#include "StartGame.h"

//Open Welcome Window
void StartGame() {

	bool gameWindow = false;	//will set to true if "Enter" is pressed inside Welcome Window
	string userName = "";	//Initialize userName

	sf::RenderWindow window(sf::VideoMode(800, 600), "Minesweeper");
	sf::Font font;
	if (!font.loadFromFile("font.ttf"))
	{
		cout << "Error opening font file." << endl;
	}
	sf::Text text; // "WELCOME TO MINESWEEPER!"
	text.setFont(font);
	text.setString("WELCOME TO MINESWEEPER!");
	text.setStyle(sf::Text::Bold | sf::Text::Underlined);
	text.setOutlineColor(sf::Color::White);
	text.setCharacterSize(24);
	sf::FloatRect bounds = text.getLocalBounds();
	text.setOrigin(bounds.width / 2, bounds.height / 2);
	text.setPosition(800 / 2.0f, 600 / 2.0f - 150);

	sf::Text namePrompt; //"Enter your name."
	namePrompt.setFont(font);
	namePrompt.setString("Enter your name.");
	namePrompt.setStyle(sf::Text::Bold);
	namePrompt.setOutlineColor(sf::Color::White);
	namePrompt.setCharacterSize(20);
	sf::FloatRect _bounds = namePrompt.getLocalBounds();
	namePrompt.setOrigin(_bounds.width / 2, _bounds.height / 2);
	namePrompt.setPosition(800 / 2.0f, 600 / 2.0f - 75);
	
	sf::Text inputText;	//User input
		inputText.setFont(font);
	inputText.setString("|");
	inputText.setStyle(sf::Text::Bold);
	inputText.setFillColor(sf::Color::Yellow);
	inputText.setCharacterSize(18);
	inputText.setPosition(800 / 2.0f, 600 / 2.0f - 45);
	
	

	//Keep track of string user is typing in window
	string userInput;

	while (window.isOpen())
	{
		for (unsigned int i = 0; i < userInput.size(); i++)
		{
			userInput[i] = tolower(userInput[i]);
		}
		userInput[0] = toupper(userInput[0]);
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::TextEntered)	//checks if enetered key is a char, under 10 chars, and adds "|" to the end 
			{
				if (isalpha(event.text.unicode))
				{
					if (userInput.size() < 11)
					{
						userInput += event.text.unicode;
						inputText.setString(userInput + "|");
						sf::FloatRect __bounds = inputText.getLocalBounds();
						inputText.setOrigin(__bounds.width / 2, __bounds.height / 2);
					}
				}
			}
			else if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::BackSpace)	//If key pressed is backspace, deletes last char
				{
					userInput = userInput.substr(0, userInput.size() - 1);
					inputText.setString(userInput + "|");
					sf::FloatRect __bounds = inputText.getLocalBounds();
					inputText.setOrigin(__bounds.width / 2, __bounds.height / 2);
				}
				if (event.key.code == sf::Keyboard::Enter)	//If key pressed is enter, save the "userName" and close the welcome window
				{
					userName = userInput;
					gameWindow = true;
					window.close();
				}
			}
		}
		window.clear(sf::Color::Blue);
		window.draw(text);
		window.draw(namePrompt);
		window.draw(inputText);
		window.display();
	}

	if ((userName.size() > 0) and gameWindow)
	{
		int row = 0;
		int col = 0;
		int mineCount = 0;

		//Read data from dimension file
		ReadFile(row, col, mineCount);

		//Vector of winners, initially empty
		vector<Person> Winners;

		//Read Leaderboard data into Winners vector, from leaderboard.txt
		ReadLeaderBoardFile(Winners);

		//Open game window with dimensions from file and name inputed in Welcome Window
		Board board(row, col, mineCount, userName, Winners);
		board.DrawGameWindow();
	}
}

//Reads file from boardconfig.txt and takes in rows, cols, minecount
void ReadFile(int &row, int &col, int &mineCount)
{
	fstream file;
	file.open("board_config.cfg", ios_base::in);
	if (file)
	{
		string temp;
		getline(file, temp);
		col = stoi(temp);
		
		getline(file, temp);
		row = stoi(temp);
		
		getline(file, temp);
		mineCount = stoi(temp);
		

	}
	file.close();
}