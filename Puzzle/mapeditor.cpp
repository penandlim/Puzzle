#include "puzzle.h"
#include "mapeditor.h"
#include "tilemap.h"
#include "menu.h"

int mapeditor(sf::RenderWindow & Window, sf::VertexArray & backgroundVertex);
int testlevel(sf::RenderWindow & Window, sf::VertexArray & backgroundVertex, sf::String sentence, int maptile2d[][16], int & movecount);
bool fileExists(std::string str);

int mapeditor(sf::RenderWindow & Window, sf::VertexArray & backgroundVertex)
{
	enum Direction {STILL, LEFT, RIGHT, UP, DOWN, ANIMATION};
	enum TileColor {BLANK, RED, GREEN, PURPLE, YELLOW, BLUE};

	sf::Vector2f futureboxPosition(43,43);
	sf::Vector2i cursorposition(0,0);
	int movement = STILL, movecount = 0, whichTileToPlaceNum = BLANK;

	sf::Texture tileTexture;
	sf::Sprite tileSprite[8][16], tileToPlaceSprite;

	sf::Font blockfont, filenamefont;
	blockfont.loadFromFile("Resources/fonts/blocks.otf");
	filenamefont.loadFromFile("Resources/fonts/filename.otf");
	sf::Text info, save, test;
	save.setFont(blockfont);
	save.setString("Save");
	save.setCharacterSize(40);
	save.setColor(sf::Color::Black);
	test.setFont(blockfont);
	test.setString("Test");
	test.setCharacterSize(40);
	test.setColor(sf::Color::Black);

	info.setFont(blockfont);
	info.setString("Tile to place: -              +");
	info.setPosition(50,655);

	tileTexture.loadFromFile("Resources/images/tileset.png");

	int custom_maptile2d[8][16],custom_maptile2d_copy[8][16];
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			custom_maptile2d[i][j] = 0;
			tileSprite[i][j].setPosition(40 + j * 75, 40 + i * 75);
			tileSprite[i][j].setTexture(tileTexture);
			tileSprite[i][j].setTextureRect(sf::IntRect(0, 0, 50, 50));
			tileSprite[i][j].setScale(1.5,1.5);
		}
	}
	tileToPlaceSprite.setPosition(210,650);
	tileToPlaceSprite.setTexture(tileTexture);
	tileToPlaceSprite.setTextureRect(sf::IntRect(0, 0, 50, 50));

	sf::Texture backgroundTexture;
	sf::Sprite backgroundSprite;

	backgroundTexture.loadFromFile("Resources/images/mapedit_background.png");
	backgroundSprite.setTexture(backgroundTexture);
	backgroundSprite.setPosition(40,40);
	backgroundSprite.setColor(sf::Color(255,255,255,100));

	sf::RectangleShape selectingbox, tileToPlaceBox, filenamebox;
	selectingbox.setOutlineThickness(3);
	selectingbox.setSize(sf::Vector2f(75 - 2 * selectingbox.getOutlineThickness(), 75 -  2 *selectingbox.getOutlineThickness()));
	selectingbox.setFillColor(sf::Color::Transparent);
	selectingbox.setOutlineColor(sf::Color::White);
	selectingbox.setPosition(43,43);

	tileToPlaceBox.setOutlineThickness(3);
	tileToPlaceBox.setSize(sf::Vector2f(50 - 2 * selectingbox.getOutlineThickness(), 50 -  2 *selectingbox.getOutlineThickness()));
	tileToPlaceBox.setFillColor(sf::Color::Transparent);
	tileToPlaceBox.setOutlineColor(sf::Color::White);
	tileToPlaceBox.setPosition(213,653);

	filenamebox.setFillColor(sf::Color::White);
	filenamebox.setSize(sf::Vector2f(400,50));
	filenamebox.setPosition(300, 650);

	sf::Texture helpTexture;
	helpTexture.loadFromFile("Resources/images/help.png");
	sf::Sprite helpSprite;
	helpSprite.setTexture(helpTexture);
	helpSprite.setColor(sf::Color(255,255,255,95));
	helpSprite.setPosition(1280-50, 0);

	sf::Texture needhelpTexture;
	needhelpTexture.loadFromFile("Resources/images/help/helpmapedit.png");
	needhelpTexture.setSmooth(1);
	sf::Sprite needhelpSprite;
	needhelpSprite.setTexture(needhelpTexture);
	needhelpSprite.setColor(sf::Color::Transparent);
	
	sf::Texture buttonTexture;
	buttonTexture.loadFromFile("Resources/images/button.png");
	sf::Sprite buttonSprite1, buttonSprite2;
	buttonSprite1.setTexture(buttonTexture);
	buttonSprite1.setColor(sf::Color(255,255,255,255));
	buttonSprite1.setPosition(750 ,650);
	buttonSprite2.setTexture(buttonTexture);
	buttonSprite2.setColor(sf::Color(100,100,100,255));
	buttonSprite2.setPosition(1000 ,650);

	save.setPosition(buttonSprite2.getPosition().x + 100 - save.getGlobalBounds().width / 2, buttonSprite2.getPosition().y + 25 - save.getGlobalBounds().height + 3);
	test.setPosition(buttonSprite1.getPosition().x + 100 - test.getGlobalBounds().width / 2, buttonSprite1.getPosition().y + 25 - test.getGlobalBounds().height + 3);

	sf::Text filenameText;
	sf::String sentence = "";
	filenameText.setColor(sf::Color::Black);
	filenameText.setString("file_name");		//		23 characters
	filenameText.setFont(filenamefont);
	filenameText.setPosition(filenamebox.getPosition().x, filenamebox.getPosition().y + 8);

	sf::Mouse mouse;
	sf::Vector2f mouseposition;
	std::string str;

	bool textmode = false, mousemode = true, allowsave = false;

	while(Window.isOpen())
	{
		sf::Event event;
        while (Window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                Window.close();
			if (event.type == sf::Event::KeyPressed && movement==STILL && !textmode)		// Key responses only work when there is no animation
			{
				mousemode = false;
				switch (event.key.code)
				{
					case sf::Keyboard::Escape:
						return 0;
						break;
					case sf::Keyboard::Right:
						if (cursorposition.x < 15)
						{
							movement = RIGHT;
							futureboxPosition = sf::Vector2f(selectingbox.getPosition().x + 75, selectingbox.getPosition().y);
							cursorposition.x++;
						}
						break;
					case sf::Keyboard::Left:
						if (cursorposition.x > 0)
						{
							movement = LEFT;
							futureboxPosition = sf::Vector2f(selectingbox.getPosition().x - 75, selectingbox.getPosition().y);
							cursorposition.x--;
						}
						break;
					case sf::Keyboard::Up:
						if (cursorposition.y > 0)
						{
							movement = UP;
							futureboxPosition = sf::Vector2f(selectingbox.getPosition().x, selectingbox.getPosition().y - 75);
							cursorposition.y--;
						}
						break;
					case sf::Keyboard::Down:
						if (cursorposition.y < 7)
						{
							movement = DOWN;
							futureboxPosition = sf::Vector2f(selectingbox.getPosition().x, selectingbox.getPosition().y + 75);
							cursorposition.y++;
						}
						break;
					case sf::Keyboard::Add:
						whichTileToPlaceNum++;
						if (whichTileToPlaceNum > BLUE)
							whichTileToPlaceNum = BLANK;
						break;
					case sf::Keyboard::Equal:
						whichTileToPlaceNum++;
						if (whichTileToPlaceNum > BLUE)
							whichTileToPlaceNum = BLANK;
						break;
					case sf::Keyboard::Dash:
						whichTileToPlaceNum--;
						if (whichTileToPlaceNum < BLANK)
							whichTileToPlaceNum = BLUE;
						break;
					case sf::Keyboard::Subtract:
						whichTileToPlaceNum--;
						if (whichTileToPlaceNum < BLANK)
							whichTileToPlaceNum = BLUE;
						break;
					case sf::Keyboard::Space:
						custom_maptile2d[cursorposition.y][cursorposition.x] = whichTileToPlaceNum;
						tileSprite[cursorposition.y][cursorposition.x].setTextureRect(sf::IntRect(whichTileToPlaceNum * 50, 0, 50, 50));
						break;
					case sf::Keyboard::Return:
						custom_maptile2d[cursorposition.y][cursorposition.x] = whichTileToPlaceNum;
						tileSprite[cursorposition.y][cursorposition.x].setTextureRect(sf::IntRect(whichTileToPlaceNum * 50, 0, 50, 50));
						break;
					case sf::Keyboard::BackSpace:
						custom_maptile2d[cursorposition.y][cursorposition.x] = 0;
						tileSprite[cursorposition.y][cursorposition.x].setTextureRect(sf::IntRect(0, 0, 50, 50));
						break;
				}
			}

			if(event.type == sf::Event::TextEntered && textmode)
			{
				if(sentence.getSize() < 22)
				{
					if(event.text.unicode >= 32 && event.text.unicode <= 126)
						sentence += (char)event.text.unicode; 
					else if(event.text.unicode == 8 && sentence.getSize() > 0)
						sentence.erase(sentence.getSize() - 1);
				}
				else
					if(event.text.unicode == 8)
						sentence.erase(sentence.getSize() - 1);

				if (event.text.unicode == 13)
				{
					filenamebox.setOutlineThickness(0);
					textmode = false;
					mousemode = true;
				}
			}
			if(event.type == sf::Event::MouseButtonPressed)
			{
				if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
				{
					if (mouseposition.x > 40 && mouseposition.x < 1240 && mouseposition.y > 40 && mouseposition.y < 640)
					{
						custom_maptile2d[cursorposition.y][cursorposition.x] = whichTileToPlaceNum;
						tileSprite[cursorposition.y][cursorposition.x].setTextureRect(sf::IntRect(whichTileToPlaceNum * 50, 0, 50, 50));
					}
					if(mouseposition.x > filenamebox.getPosition().x && mouseposition.x < filenamebox.getPosition().x + filenamebox.getSize().x && mouseposition.y > filenamebox.getPosition().y && mouseposition.y < filenamebox.getPosition().y + filenamebox.getSize().y)
					{
						filenamebox.setOutlineColor(sf::Color::Red);
						filenamebox.setOutlineThickness(1);
						textmode = true;
					}
					else
					{
						filenamebox.setOutlineThickness(0);
						textmode = false;
						mousemode = true;
					}
				}
				else if(sf::Mouse::isButtonPressed(sf::Mouse::Right) && (mouseposition.x > 40 && mouseposition.x < 1240 && mouseposition.y > 40 && mouseposition.y < 640))
				{
					custom_maptile2d[cursorposition.y][cursorposition.x] = BLANK;
					tileSprite[cursorposition.y][cursorposition.x].setTextureRect(sf::IntRect(0, 0, 50, 50));
				}
			}
			if (event.type == sf::Event::MouseWheelMoved)
			{
				if (event.mouseWheel.delta > 0)
					whichTileToPlaceNum++;
				else
					whichTileToPlaceNum--;
				if (whichTileToPlaceNum < BLANK)
					whichTileToPlaceNum = BLUE;
				if (whichTileToPlaceNum > BLUE)
					whichTileToPlaceNum = BLANK;
			}
			if (event.type == sf::Event::LostFocus)
				windowhasfocus = false;
			else if (event.type == sf::Event::GainedFocus)
				windowhasfocus = true;
		}
		if(textmode)
			filenameText.setString(sentence + "|");
		else
		{
			if(sentence == "")
				filenameText.setString("file_name");
			else
				filenameText.setString(sentence);
		}
			

		tileToPlaceSprite.setTextureRect(sf::IntRect(50 * whichTileToPlaceNum,0,50,50));

		mouseposition.x = (float) mouse.getPosition(Window).x * 1280 / Window.getSize().x;
		mouseposition.y = (float) mouse.getPosition(Window).y * 720 / Window.getSize().y;

		if (mouseposition.x > 1230 && mouseposition.y < 50)
		{
			if (helpSprite.getColor().a < 255)
			helpSprite.setColor(sf::Color(255,255,255,helpSprite.getColor().a + 10));
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
				needhelp = true;
		}
		else 
		{
			if (helpSprite.getColor().a > 95)
			helpSprite.setColor(sf::Color(255,255,255,helpSprite.getColor().a - 10));
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
				needhelp = false;
		}

		if(needhelp)
		{
			if (needhelpSprite.getColor().a < 255)
				needhelpSprite.setColor(sf::Color(255,255,255,needhelpSprite.getColor().a + 15));
		}
		else
		{
			if (needhelpSprite.getColor().a > 0)
				needhelpSprite.setColor(sf::Color(255,255,255,needhelpSprite.getColor().a - 15));
		}

		//		Hover mouse over button 1 (test)
		if (mouseposition.x > buttonSprite1.getPosition().x && mouseposition.x < buttonSprite1.getPosition().x + buttonSprite1.getGlobalBounds().width && mouseposition.y > buttonSprite1.getPosition().y && mouseposition.y < buttonSprite1.getPosition().y + buttonSprite1.getGlobalBounds().height)
		{
			buttonSprite1.setColor(sf::Color(117,133,255));
			if(mouse.isButtonPressed(sf::Mouse::Left))
			{
				for (int i = 0; i < 8; i++)
				{
					for (int j = 0; j < 16; j++)
					{
						custom_maptile2d_copy[i][j] = custom_maptile2d[i][j];
					}
				}
				allowsave = false;
				movecount = 0;
				if(testlevel(Window, backgroundVertex,sentence, custom_maptile2d_copy, movecount) == 0 && movecount !=0)
				{
					test.setString("Redo");
					allowsave = true;
					save.setString("Save");
					save.setPosition(buttonSprite2.getPosition().x + 100 - save.getGlobalBounds().width / 2, buttonSprite2.getPosition().y + 25 - save.getGlobalBounds().height + 3);
				}
			}
		}
		else 
		{
			buttonSprite1.setColor(sf::Color::White);
		}

		//		once tested completely this triggers
		if(allowsave)
		{
			buttonSprite2.setColor(sf::Color::White);
			if (mouseposition.x > buttonSprite2.getPosition().x && mouseposition.x < buttonSprite2.getPosition().x + buttonSprite2.getGlobalBounds().width && mouseposition.y > buttonSprite2.getPosition().y && mouseposition.y < buttonSprite2.getPosition().y + buttonSprite2.getGlobalBounds().height)
			{
				buttonSprite2.setColor(sf::Color(117,133,255));
				if(mouse.isButtonPressed(sf::Mouse::Left))
				{
					if (sentence == "")
						str = ("Custom_Maps/file_name.txt");
					else
						str = "Custom_Maps/" + sentence + ".txt";
					if(!fileExists(str))	// if file doesnt exist, save
					{
						std::ofstream custom_map_file;
						custom_map_file.open (str);
						for (int i = 0; i < 8; i++)
						{
							for (int j = 0; j < 16; j++)
							{
								custom_map_file << custom_maptile2d[i][j];
								if(j < 15)
									custom_map_file<< " ";
							}
							custom_map_file << "\n";
						}
						custom_map_file << "\n" << movecount;
						custom_map_file.close();
						custom_map_file.clear();
						save.setString("Saved");
						save.setPosition(buttonSprite2.getPosition().x + 100 - save.getGlobalBounds().width / 2, buttonSprite2.getPosition().y + 25 - save.getGlobalBounds().height + 3);
						test.setString("Test");
						movecount = 0;
						allowsave = false;
					}
					else
					{
						save.setString("Duplicate");
						save.setPosition(buttonSprite2.getPosition().x + 100 - save.getGlobalBounds().width / 2, buttonSprite2.getPosition().y + 25 - save.getGlobalBounds().height + 3);

					}
				}
			}
		}
		else
		{
			buttonSprite2.setColor(sf::Color(100,100,100,255));
		}

		if (windowhasfocus && !textmode && mousemode)
		{
			if (mouseposition.x > 40 && mouseposition.x < 1240 && mouseposition.y > 40 && mouseposition.y < 640)
			{
				futureboxPosition.x = ((int) (mouseposition.x - 40)/75) * 75 + 43;
				futureboxPosition.y = ((int) (mouseposition.y - 40)/75) * 75 + 43;
				cursorposition.x = (int) (mouseposition.x - 40)/75;
				cursorposition.y = (int) (mouseposition.y - 40)/75;
			}
		}

		//	Moves selectingbox to futureboxPosition
		if ( abs(futureboxPosition.x - selectingbox.getPosition().x) < 2 && abs(futureboxPosition.y - selectingbox.getPosition().y) < 2)
		{
			selectingbox.setPosition(futureboxPosition);
			movement = STILL;
		}
		else
		{
			selectingbox.move( 0.4 * (futureboxPosition.x - selectingbox.getPosition().x), 0.4 * (futureboxPosition.y - selectingbox.getPosition().y) );
		}

		//	Renders Window
		Window.clear();
		Window.draw(backgroundSprite);
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 16; j++)
			{
				Window.draw(tileSprite[i][j]);
			}
		}
		Window.draw(tileToPlaceSprite);
		Window.draw(tileToPlaceBox);
		Window.draw(filenamebox);
		Window.draw(filenameText);
		Window.draw(selectingbox);
		Window.draw(info);
		Window.draw(buttonSprite1);
		Window.draw(buttonSprite2);
		Window.draw(test);
		Window.draw(save);
		Window.draw(helpSprite);
		Window.draw(needhelpSprite);
		Window.display();
	}
}






int testlevel(sf::RenderWindow & Window, sf::VertexArray & backgroundVertex, sf::String sentence, int maptile2d[][16], int & movecount)
{
	enum Direction {STILL, LEFT, RIGHT, UP, DOWN, ANIMATION}; 
	sf::Vector2f futureboxPosition(43,43);
	sf::Vector2i cursorposition(0,0);
	int movement = STILL;

	sf::RectangleShape selectingbox;
	selectingbox.setOutlineThickness(3);
	selectingbox.setSize(sf::Vector2f(75 - 2 * selectingbox.getOutlineThickness(), 150 -  2 *selectingbox.getOutlineThickness()));
	selectingbox.setFillColor(sf::Color::Transparent);
	selectingbox.setOutlineColor(sf::Color::White);
	selectingbox.setPosition(43,43);

	sf::Font blockFont, levelFont;
	blockFont.loadFromFile("Resources/fonts/blocks.otf");
	levelFont.loadFromFile("Resources/fonts/filename.otf");
	sf::Text displaylevel;
	displaylevel.setFont(levelFont);
	if (sentence == "")
		displaylevel.setString("file_name.txt");
	else
		displaylevel.setString(sentence + ".txt");
	displaylevel.setPosition( (1280 - displaylevel.getGlobalBounds().width) / 2, 650 );


	sf::Texture helpTexture;
	helpTexture.loadFromFile("Resources/images/help.png");
	sf::Sprite helpSprite;
	helpSprite.setTexture(helpTexture);
	helpSprite.setColor(sf::Color(255,255,255,95));
	helpSprite.setPosition(1280-50, 0);

	sf::Texture backgroundTexture, pauseScreenTexture;
	sf::Sprite backgroundSprite, pauseScreenSprite;

	backgroundTexture.loadFromFile("Resources/images/mapedit_background.png");
	backgroundTexture.setSmooth(1);
	backgroundSprite.setTexture(backgroundTexture);
	backgroundSprite.setPosition(40,40);
	backgroundSprite.setColor(sf::Color(255,255,255,100));

					// 2d array maptiles. read from resources/maps/*.txt
	int maptile2d_copy[8][16];
	int slide_position_x[8][16];
	int loop_colorchange=0;

	bool finished = false, isScreenPaused = false;

    // create the tilemap from the level definition
    TileMap map;
    if (!map.load("Resources/images/tileset.png", sf::Vector2u(50, 50), maptile2d, 16, 8))
        return -1;
	map.setPosition(40,40);
	map.setScale(1.5,1.5);

	sf::Mouse mouse;
	sf::Vector2f mouseposition;

	while(Window.isOpen())
	{
		sf::Event event;
        while (Window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                Window.close();
			if (event.type == sf::Event::LostFocus)
				windowhasfocus = false;
			else if (event.type == sf::Event::GainedFocus)
				windowhasfocus = true;
			if (event.type == sf::Event::MouseButtonPressed)
			{
				if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
				{
					if(maptile2d[cursorposition.y][cursorposition.x] == maptile2d[cursorposition.y +1][cursorposition.x])
							std::cout<< "Same box\n";
						else
						{
							movement = ANIMATION;
							movecount++;
							std::cout << movecount << " tries\n";
							int loopnumber = 0;

							while (!map.swap(cursorposition, maptile2d, loopnumber))
							{
								Window.clear();
								Window.draw(backgroundVertex);
								Window.draw(backgroundSprite);
								Window.draw(map);
								Window.draw(displaylevel);
								Window.draw(selectingbox);
								Window.display();
							}
							Window.clear();
							Window.draw(backgroundVertex);
							Window.draw(backgroundSprite);
							Window.draw(map);
							Window.draw(displaylevel);
							Window.draw(selectingbox);
							Window.display();

							while (map.slide_data(maptile2d, maptile2d_copy, slide_position_x))
							{

								loopnumber = 0;
								while (map.slide_animation(slide_position_x, loopnumber))
								{
									Window.clear();
									Window.draw(backgroundVertex);
									Window.draw(backgroundSprite);
									Window.draw(map);
									Window.draw(displaylevel);
									Window.draw(selectingbox);
									Window.display();
								}
								
								
								map.detect(maptile2d, maptile2d_copy);
								std::cout << std::endl;
								for (int i = 0; i < 8; i++)
								{
									for (int j = 0; j < 16; j++)
									{
										std::cout << maptile2d[i][j] << " ";
									}
									std::cout << std::endl;
								}
								for (int i = 0; i < 8; i++)
								{
									for (int j = 0; j < 16; j++)
									{
										std::cout << maptile2d_copy[i][j] << " ";
									}
									std::cout << std::endl;
								}
								std::cout << std::endl;
								loopnumber = 0;
								while(!map.fix(maptile2d,maptile2d_copy,sf::Vector2u(50,50), loopnumber))
								{
									Window.clear();
									Window.draw(backgroundVertex);
									Window.draw(backgroundSprite);
									Window.draw(map);
									Window.draw(displaylevel);
									Window.draw(selectingbox);
									Window.display();
								}

							}
							if(!map.slide_data(maptile2d, maptile2d_copy, slide_position_x))
							{
								map.detect(maptile2d, maptile2d_copy);
								loopnumber = 0;
								while (!map.fix(maptile2d, maptile2d_copy, sf::Vector2u(50, 50), loopnumber))
								{
									Window.clear();
									Window.draw(backgroundVertex);
									Window.draw(backgroundSprite);
									Window.draw(map);
									Window.draw(displaylevel);
									Window.draw(selectingbox);
									Window.display();
								}
								while (map.slide_data(maptile2d, maptile2d_copy, slide_position_x))
								{
									loopnumber = 0;
									while (map.slide_animation(slide_position_x, loopnumber))
									{
										Window.clear();
										Window.draw(backgroundVertex);
										Window.draw(backgroundSprite);
										Window.draw(map);
										Window.draw(displaylevel);
										Window.draw(selectingbox);
										Window.display();
									}
								
									map.detect(maptile2d, maptile2d_copy);
									loopnumber = 0;
									while(!map.fix(maptile2d,maptile2d_copy,sf::Vector2u(50,50), loopnumber))
									{
										Window.clear();
										Window.draw(backgroundVertex);
										Window.draw(backgroundSprite);
										Window.draw(map);
										Window.draw(displaylevel);
										Window.draw(selectingbox);
										Window.display();
									}

								}
							}
						}
				}
			}
			if (event.type == sf::Event::KeyPressed && movement==STILL && windowhasfocus)		// Key responses only work when there is no animation
			{
				switch (event.key.code)
				{
					case sf::Keyboard::Escape:
						return 1;
						break;
					case sf::Keyboard::Right:
						if (cursorposition.x < 15)
						{
							movement = RIGHT;
							futureboxPosition = sf::Vector2f(selectingbox.getPosition().x + 75, selectingbox.getPosition().y);
							cursorposition.x++;
						}
						break;
					case sf::Keyboard::Left:
						if (cursorposition.x > 0)
						{
							movement = LEFT;
							futureboxPosition = sf::Vector2f(selectingbox.getPosition().x - 75, selectingbox.getPosition().y);
							cursorposition.x--;
						}
						break;
					case sf::Keyboard::Up:
						if (cursorposition.y > 0)
						{
							movement = UP;
							futureboxPosition = sf::Vector2f(selectingbox.getPosition().x, selectingbox.getPosition().y - 75);
							cursorposition.y--;
						}
						break;
					case sf::Keyboard::Down:
						if (cursorposition.y < 6)
						{
							movement = DOWN;
							futureboxPosition = sf::Vector2f(selectingbox.getPosition().x, selectingbox.getPosition().y + 75);
							cursorposition.y++;
						}
						break;

					case sf::Keyboard::Space:		// swap, slide_data, slide_animation, fix. then if there are more blocks to cancel, it loops slide_data, slide_animation, fix.
						if(maptile2d[cursorposition.y][cursorposition.x] == maptile2d[cursorposition.y +1][cursorposition.x])
							std::cout<< "Same box\n";
						else
						{
							movement = ANIMATION;
							movecount++;
							std::cout << movecount << " tries\n";
							int loopnumber = 0;

							while (!map.swap(cursorposition, maptile2d, loopnumber))
							{
								Window.clear();
								Window.draw(backgroundVertex);
								Window.draw(backgroundSprite);
								Window.draw(map);
								Window.draw(displaylevel);
								Window.draw(selectingbox);
								Window.display();
							}
							Window.clear();
							Window.draw(backgroundVertex);
							Window.draw(backgroundSprite);
							Window.draw(map);
							Window.draw(displaylevel);
							Window.draw(selectingbox);
							Window.display();

							while (map.slide_data(maptile2d, maptile2d_copy, slide_position_x))
							{
								// DEBUG slide_position_x
								/*for (int i = 0; i < 8; i++)
								{
									for (int j = 0; j < 16; j++)
									{
										std::cout <<slide_position_x[i][j] << " ";
									}
									std::cout << std::endl;
								}*/

								loopnumber = 0;
								while (map.slide_animation(slide_position_x, loopnumber))
								{
									Window.clear();
									Window.draw(backgroundVertex);
									Window.draw(backgroundSprite);
									Window.draw(map);
									Window.draw(displaylevel);
									Window.draw(selectingbox);
									Window.display();
									// DEBUG loopnumber
									/*std::cout << loopnumber;*/
								}
								
								
								map.detect(maptile2d, maptile2d_copy);
								std::cout << std::endl;
								for (int i = 0; i < 8; i++)
								{
									for (int j = 0; j < 16; j++)
									{
										std::cout << maptile2d[i][j] << " ";
									}
									std::cout << std::endl;
								}
								for (int i = 0; i < 8; i++)
								{
									for (int j = 0; j < 16; j++)
									{
										std::cout << maptile2d_copy[i][j] << " ";
									}
									std::cout << std::endl;
								}
								std::cout << std::endl;
								loopnumber = 0;
								while(!map.fix(maptile2d,maptile2d_copy,sf::Vector2u(50,50), loopnumber))
								{
									Window.clear();
									Window.draw(backgroundVertex);
									Window.draw(backgroundSprite);
									Window.draw(map);
									Window.draw(displaylevel);
									Window.draw(selectingbox);
									Window.display();
								}

							}
							if(!map.slide_data(maptile2d, maptile2d_copy, slide_position_x))
							{
								map.detect(maptile2d, maptile2d_copy);
								loopnumber = 0;
								while (!map.fix(maptile2d, maptile2d_copy, sf::Vector2u(50, 50), loopnumber))
								{
									Window.clear();
									Window.draw(backgroundVertex);
									Window.draw(backgroundSprite);
									Window.draw(map);
									Window.draw(displaylevel);
									Window.draw(selectingbox);
									Window.display();
								}
								while (map.slide_data(maptile2d, maptile2d_copy, slide_position_x))
								{
									loopnumber = 0;
									while (map.slide_animation(slide_position_x, loopnumber))
									{
										Window.clear();
										Window.draw(backgroundVertex);
										Window.draw(backgroundSprite);
										Window.draw(map);
										Window.draw(displaylevel);
										Window.draw(selectingbox);
										Window.display();
									}
								
									map.detect(maptile2d, maptile2d_copy);
									loopnumber = 0;
									while(!map.fix(maptile2d,maptile2d_copy,sf::Vector2u(50,50), loopnumber))
									{
										Window.clear();
										Window.draw(backgroundVertex);
										Window.draw(backgroundSprite);
										Window.draw(map);
										Window.draw(displaylevel);
										Window.draw(selectingbox);
										Window.display();
									}

								}
							}
						}

						break;
				}
		    }
		}

		mouseposition.x = (float) mouse.getPosition(Window).x * 1280 / Window.getSize().x;
		mouseposition.y = (float) mouse.getPosition(Window).y * 720 / Window.getSize().y;

		if (mouseposition.x > 1230 && mouseposition.y < 50)
		{
			if (helpSprite.getColor().a < 255)
			helpSprite.setColor(sf::Color(255,255,255,helpSprite.getColor().a + 10));
		}
		else if (helpSprite.getColor().a > 95)
			helpSprite.setColor(sf::Color(255,255,255,helpSprite.getColor().a - 10));

		if (windowhasfocus)
		{
			if (mouseposition.x > 40 && mouseposition.x < 1240 && mouseposition.y > 40 && mouseposition.y < 640)
			{
				futureboxPosition.x = ((int) (mouseposition.x - 40)/75) * 75 + 43;
				futureboxPosition.y = ((int) (mouseposition.y - 40)/75) * 75 + 43;
				if(futureboxPosition.y == 568)
					futureboxPosition.y = 493;
				cursorposition.x = (int) (mouseposition.x - 40)/75;
				cursorposition.y = (int) (mouseposition.y - 40)/75;
				if(cursorposition.y > 6)
					cursorposition.y = 6;
			}
		}

		std::cout << cursorposition.x << " " << cursorposition.y << std::endl;

		// Check if all tiles are empty and if there are no more blocks, it proceeds to next level.
		finished = true;
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 16; j++)
			{
				if (maptile2d[i][j] != 0)
					finished = false;
			}
		}

		// Compares movecount with possiblemoves. And checks if user has completed the level or failed.
		if (finished)
		{
			return 0;
		}

		// Moves the selectingbox to next position that is set when arrow keys are pressed or when the stage is completed.
		selectingbox.move( 0.4 * (futureboxPosition.x - selectingbox.getPosition().x), 0.4 * (futureboxPosition.y - selectingbox.getPosition().y) );

		if ( abs(futureboxPosition.x - selectingbox.getPosition().x) < 2 && abs(futureboxPosition.y - selectingbox.getPosition().y) < 2)
		{
			selectingbox.setPosition(futureboxPosition);
			movement = STILL;
		}

		// loop for changing background color. randomly changes rgb by +-1 every (int i) frames
		loop_colorchange++;
		if (loop_colorchange == 4)
		{
			srand ((unsigned)time(NULL));
			for (int i = 0; i < 4; i++)
			{
				backgroundVertex[i].color = sf::Color(colorchange(backgroundVertex[i].color.r), colorchange(backgroundVertex[i].color.g), colorchange(backgroundVertex[i].color.b), 255);
			}
			loop_colorchange = 0;
		}


		// Draws objects on Window and displays them
		Window.clear();
		Window.draw(backgroundVertex);
		Window.draw(backgroundSprite);
		Window.draw(map);
		Window.draw(displaylevel);
		Window.draw(selectingbox);
		Window.draw(helpSprite);
		Window.display();
	}

	return 0;
}


bool fileExists(std::string str)
{
    std::ifstream infile(str);
    return infile.good();
}