#include "tilemap.h"
#include "puzzle.h"
#include "menu.h"
#include "mapeditor.h"
#include "multi.h"
#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include"SFML\dirent.h"


int level(sf::RenderWindow & Window, sf::VertexArray & backgroundVertex, int & levelnumber);
int pauseScreen(sf::RenderWindow & Window, sf::Sprite background, bool finished, int levelnumber);
int selectStage(sf::RenderWindow & Window, sf::VertexArray & backgroundVertex, int & levelnumber);
int classicorcustom(sf::RenderWindow & Window, sf::VertexArray & backgroundVertex);
int selectCustom(sf::RenderWindow & Window, sf::VertexArray & backgroundVertex, std::string filename[54]);
int customlevel(sf::RenderWindow & Window, sf::VertexArray & backgroundVertex, std::string filenamestr);

sf::Font blockFont;
sf::Font levelFont;
sf::View view;

bool needhelp = false;
bool windowhasfocus = true;
bool completedgame = false;
int classicorcustomint = 0;

int main()
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 0;

	sf::VideoMode desktopVM = sf::VideoMode::getDesktopMode();
	//view.reset(sf::FloatRect(0,0,1280,720));
	
	std::string filename[54];

	sf::RenderWindow Window(sf::VideoMode(desktopVM.width, desktopVM.height, 32),  "Color Puzzle" , sf::Style::Fullscreen , settings);
	view.reset(sf::FloatRect(0,0,1280,720));
	view.setViewport(sf::FloatRect(0,0,1,1));
	Window.setView(view);
	Window.setMouseCursorVisible(true);
	Window.setVerticalSyncEnabled(true);
	/*Window.setFramerateLimit(60);*/
	Window.setKeyRepeatEnabled(true);
	sf::VertexArray backgroundVertex(sf::Quads, 4);
	bool closeProgram = 0;
	int levelnumber = 0;


	//	main control menu loop
	while (!closeProgram)
	{
		switch(menu(Window, backgroundVertex))
		{
		case 0:
			classicorcustomint = 0;
			while (classicorcustom(Window,backgroundVertex) == 0)
			{
				if(classicorcustomint == 1)
				{
					while (selectStage(Window,backgroundVertex, levelnumber) == 0)
					{
						level(Window, backgroundVertex, levelnumber);
					}
				}
				else if (classicorcustomint == 2)
				{
					selectCustom(Window,backgroundVertex,filename);
				}
				else
					break;
			}
			break;
		case 1:
			multiplayer_select(Window,backgroundVertex);
			break;
		case 2:
			mapeditor(Window,backgroundVertex);
			break;
		case 3:
			closeProgram = true;
			break;
		}
	}
	Window.close();
    return 0;
}

int classicorcustom(sf::RenderWindow & Window, sf::VertexArray & backgroundVertex)
{
	sf::Font classicFont;
	classicFont.loadFromFile("Resources/fonts/classic.otf");
	sf::Text classicText;
	classicText.setFont(classicFont);
	classicText.setString("Classic");
	classicText.setCharacterSize(600);
    classicText.setOrigin(classicText.getGlobalBounds().width / 2, classicText.getGlobalBounds().height / 2);
	classicText.setScale(0.4,0.4);
	classicText.setPosition(640, 100);
	sf::Clock onesecondclock;
	onesecondclock.restart();
	sf::Mouse mouse;
	sf::Vector2f mouseposition;

	int loop_colorchange=0;

	sf::Texture needhelpTexture;
	needhelpTexture.loadFromFile("Resources/images/help/helpclassic.png");
	needhelpTexture.setSmooth(1);
	sf::Sprite needhelpSprite;
	needhelpSprite.setTexture(needhelpTexture);
	needhelpSprite.setColor(sf::Color::Transparent);

	sf::Texture helpTexture;
	helpTexture.loadFromFile("Resources/images/help.png");
	sf::Sprite helpSprite;
	helpSprite.setTexture(helpTexture);
	helpSprite.setColor(sf::Color(255,255,255,95));
	helpSprite.setPosition(1280-50, 0);

	sf::Font customFont;
	customFont.loadFromFile("Resources/fonts/filename.otf");
	sf::Text customText;
	customText.setFont(customFont);
	customText.setString("Custom");
	customText.setCharacterSize(450);
    customText.setOrigin(customText.getGlobalBounds().width / 2, customText.getGlobalBounds().height / 2);
	customText.setScale(0.4,0.4);
	customText.setPosition(640, 500);

	while(Window.isOpen())
	{
		sf::Event event;
		while (Window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                Window.close();
			if (event.type == sf::Event::KeyPressed)
			{
				switch (event.key.code)
				{
				case sf::Keyboard::Escape:
					classicorcustomint = 0;
					return 1;
				}
			}
			if (event.type == sf::Event::LostFocus)
				windowhasfocus = false;
			else if (event.type == sf::Event::GainedFocus)
				windowhasfocus = true;
		}
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

		if (windowhasfocus && mouseposition.x >317 && mouseposition.y > 73 && mouseposition.x < 967 && mouseposition.y < 298)
		{
			if (classicText.getScale().y < 0.45)
			{
				classicText.scale(1.02,1.02);
			}
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && onesecondclock.getElapsedTime().asMilliseconds() > 500)
			{
				classicorcustomint = 1;
				return 0;
			}
		}
		else
		{
			if (classicText.getScale().y > 0.4)
			{
				classicText.scale(0.98,0.98);
			}
		}

		if (windowhasfocus && mouseposition.x > 329 && mouseposition.y > 457 && mouseposition.x < 975 && mouseposition.y < 608)
		{
			if (customText.getScale().y < 0.45)
			{
				customText.scale(1.02,1.02);
			}
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && onesecondclock.getElapsedTime().asMilliseconds() > 1000)
			{
				classicorcustomint = 2;
				return 0;

				//DIR *dir;
				//struct dirent *ent;
				//if ((dir = opendir ("Custom_Maps\\")) != NULL) 
				//{
				//  /* print all the files and directories within directory */
				//	while ((ent = readdir (dir)) != NULL) 
				//	{
				//		if (std::string(ent->d_name)!= "." && std::string(ent->d_name)!="..")
				//			std::cout << ent->d_name << std::endl;
				//	}
				//	closedir (dir);
				//} 
				//else 
				//{
				//	/* could not open directory */
				//	perror ("");
				//	return EXIT_FAILURE;
				//}
			}
		}
		else
		{
			if (customText.getScale().y > 0.4)
			{
				customText.scale(0.98,0.98);
			}
		}

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

		Window.clear();
		Window.draw(backgroundVertex);
		Window.draw(classicText);
		Window.draw(customText);
		Window.draw(helpSprite);
		Window.draw(needhelpSprite);
		Window.display();
	}
}

int selectCustom(sf::RenderWindow & Window, sf::VertexArray & backgroundVertex, std::string filename[54])
{
	sf::Mouse mouse;
	sf::Vector2f mouseposition;
	int loop_colorchange=0;

	sf::Clock onesecondclock;
	onesecondclock.restart();
	sf::Texture helpTexture;
	helpTexture.loadFromFile("Resources/images/help.png");
	sf::Sprite helpSprite;
	helpSprite.setTexture(helpTexture);
	helpSprite.setColor(sf::Color(255,255,255,95));
	helpSprite.setPosition(1280-50, 0);

	sf::Texture needhelpTexture;
	needhelpTexture.loadFromFile("Resources/images/help/helpcustom.png");
	needhelpTexture.setSmooth(1);
	sf::Sprite needhelpSprite;
	needhelpSprite.setTexture(needhelpTexture);
	needhelpSprite.setColor(sf::Color::Transparent);


	for (int i = 0; i < 54; i++)
	{
		filename[i]="";
	}

	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir ("Custom_Maps\\")) != NULL) 
	{
		/* print all the files and directories within directory */
		int i =0;
		while ((ent = readdir (dir)) != NULL) 
		{
			if (std::string(ent->d_name)!= "." && std::string(ent->d_name)!="..")
			{
				/*std::cout << ent->d_name << std::endl;*/
				filename[i]=std::string(ent->d_name);
				i++;
			}
		}
		closedir (dir);
	} 
	else 
	{
		/* could not open directory */
		perror ("");
		return EXIT_FAILURE;
	}

	sf::Font customFont;
	customFont.loadFromFile("Resources/fonts/filename.otf");
	sf::Text customlevelsText[54];
	for (int i = 0; i < 54; i++)
	{
		customlevelsText[i].setFont(customFont);
		customlevelsText[i].setCharacterSize(20);
		customlevelsText[i].setString(filename[i]);
		customlevelsText[i].setPosition(40 + 400* (int) (i/18), 40 + (i % 18) * 35);
	}

	while(Window.isOpen())
	{
		sf::Event event;
		while (Window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                Window.close();
			if (event.type == sf::Event::KeyPressed)
			{
				switch (event.key.code)
				{
				case sf::Keyboard::Escape:
					classicorcustomint = 0;
					return 1;
				}
			}
			if (event.type == sf::Event::LostFocus)
				windowhasfocus = false;
			else if (event.type == sf::Event::GainedFocus)
				windowhasfocus = true;
		}
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

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && onesecondclock.getElapsedTime().asMilliseconds() > 500)
		{
			if(mouseposition.x > 40 && mouseposition.y > 40 && mouseposition.x < 1240 && mouseposition.y < 665)
			{
				if (filename[(int) (mouseposition.y - 40) / 35 + ((int) (mouseposition.x - 40) / 400 ) * 18] != "")
					customlevel(Window, backgroundVertex, filename[(int) (mouseposition.y - 40) / 35 + ((int) (mouseposition.x - 40) / 400 ) * 18]);
			}
		}

		Window.clear();
		Window.draw(backgroundVertex);
		for (int i = 0; i < 54; i++)
		{
			Window.draw(customlevelsText[i]);
		}
		Window.draw(helpSprite);
		Window.draw(needhelpSprite);
		Window.display();
	}
}

int customlevel(sf::RenderWindow & Window, sf::VertexArray & backgroundVertex, std::string filenamestr)
{
	enum Direction {STILL, LEFT, RIGHT, UP, DOWN, ANIMATION}; 
	sf::Vector2f futureboxPosition(43,43);
	sf::Vector2i cursorposition(0,0);
	int movement = STILL, movecount = 0;

	sf::RectangleShape selectingbox;
	selectingbox.setOutlineThickness(3);
	selectingbox.setSize(sf::Vector2f(75 - 2 * selectingbox.getOutlineThickness(), 150 -  2 *selectingbox.getOutlineThickness()));
	selectingbox.setFillColor(sf::Color::Transparent);
	selectingbox.setOutlineColor(sf::Color::White);
	selectingbox.setPosition(43,43);

	blockFont.loadFromFile("Resources/fonts/blocks.otf");
	levelFont.loadFromFile("Resources/fonts/levelfont2.otf");
	sf::Text displaylevel;
	displaylevel.setFont(levelFont);

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

	int maptile2d[8][16];					// 2d array maptiles. read from resources/maps/*.txt
	int maptile2d_copy[8][16];
	int slide_position_x[8][16];
	int possiblemoves;
	int loop_colorchange=0;

	bool finished = false, isScreenPaused = false;

	std::ostringstream filename;
	filename << "Custom_maps//" << filenamestr;

	std::ifstream in_maptile;
	in_maptile.open(filename.str());
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			in_maptile >> maptile2d[i][j];			// using ifstream to put integers into maptile array
			std::cout << maptile2d[i][j] << " ";
		}
		std::cout << std::endl;
	}
	in_maptile >> possiblemoves;
	in_maptile.close();

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
				}
			}
			if (event.type == sf::Event::KeyPressed && movement==STILL && windowhasfocus)		// Key responses only work when there is no animation
			{
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
		if (finished || movecount >=possiblemoves)
		{
			Window.clear();
			Window.draw(backgroundVertex);
			Window.draw(backgroundSprite);
			Window.draw(map);
			Window.draw(displaylevel);

			pauseScreenTexture.create(Window.getSize().x,Window.getSize().y);
			pauseScreenTexture.update(Window);
			pauseScreenSprite.setTexture(pauseScreenTexture);
			if (pauseScreen(Window, pauseScreenSprite, finished, -1) == 0 )
			{
				if(finished)
				{
					return 0;
				}
				else
				{
					std::cout<<"Try again" << std::endl;
				}
			}
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

		//	Displays level on the bottom of the screen
		std::string levelstring = filenamestr;
		displaylevel.setColor(sf::Color::White);
		displaylevel.setString(levelstring);
		displaylevel.setCharacterSize(160);
		displaylevel.setScale(0.5,0.5);
		displaylevel.setPosition( (1280 - displaylevel.getGlobalBounds().width) / 2, (720 - displaylevel.getGlobalBounds().height) / 2 + 295);


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



int selectStage(sf::RenderWindow & Window, sf::VertexArray & backgroundVertex, int & levelnumber)
{
	//	Play non 8bit version when returned to selecting stage from levels

	int loop_colorchange = 0;

	sf::Texture icons;
	icons.loadFromFile("Resources/images/tileset_classic.png");
	
	sf::Sprite icon_sprite[6][10];
	sf::Text icon_level[6][10];
	sf::Font icon_font;
	sf::Mouse mouse;
	sf::Vector2f mouseposition;
	icon_font.loadFromFile("Resources/fonts/blocks.otf");

	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			std::string levelstring = std::to_string(i*10 + j + 1);
			icon_sprite[i][j].setTexture(icons);
			if (i==5)
			{
				icon_sprite[i][j].setTextureRect( sf::IntRect(51, 0, 48, 50));
				icon_sprite[i][j].setColor(sf::Color(100,100,100,255));
			}
			else
				icon_sprite[i][j].setTextureRect( sf::IntRect(50*i + 51, 0, 48, 50));
			icon_sprite[i][j].setOrigin(icon_sprite[i][j].getGlobalBounds().width / 2, icon_sprite[i][j].getGlobalBounds().height / 2);
			icon_sprite[i][j].setPosition(40 + 125 * j + 75/2, 40 + /*128*/107 *i + 75/2);
			icon_sprite[i][j].setScale(0.5,0.5);
			icon_level[i][j].setFont(icon_font);
			icon_level[i][j].setString(levelstring);
			icon_level[i][j].setCharacterSize(100);
			icon_level[i][j].setOrigin(icon_level[i][j].getGlobalBounds().width / 2, icon_level[i][j].getGlobalBounds().height / 2 + 20);
			icon_level[i][j].setScale(0.15,0.15);
			icon_level[i][j].setPosition(icon_sprite[i][j].getPosition().x - 3, icon_sprite[i][j].getPosition().y);
		}
	}

	sf::Texture helpTexture;
	helpTexture.loadFromFile("Resources/images/help.png");
	sf::Sprite helpSprite;
	helpSprite.setTexture(helpTexture);
	helpSprite.setColor(sf::Color(255,255,255,95));
	helpSprite.setPosition(1280-50, 0);

	sf::Texture needhelpTexture;
	needhelpTexture.loadFromFile("Resources/images/help/helpclassic.png");
	needhelpTexture.setSmooth(1);
	sf::Sprite needhelpSprite;
	needhelpSprite.setTexture(needhelpTexture);
	needhelpSprite.setColor(sf::Color::Transparent);

	while(Window.isOpen())
	{
		sf::Event event;
		while (Window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                Window.close();
			if (event.type == sf::Event::KeyPressed)
			{
				switch (event.key.code)
				{
				case sf::Keyboard::Escape:
					return 1;
				}
			}
			if (event.type == sf::Event::LostFocus)
				windowhasfocus = false;
			else if (event.type == sf::Event::GainedFocus)
				windowhasfocus = true;
		}

		if (icon_sprite[4][9].getScale().x < 1.5)
		for (int i = 0; i < 6; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				if (icon_sprite[i][j].getScale().x < 1.5)
				{
					if (j==0)
					{
						icon_sprite[i][j].scale(1.05,1.05);
						icon_level[i][j].scale(1.05,1.05);
					}
					else if (icon_sprite[i][j-1].getScale().x > 0.65)
					{
						icon_sprite[i][j].scale(1.05,1.05);
						icon_level[i][j].scale(1.05,1.05);
					}
				}
			}
		}

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


		if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && windowhasfocus == true && icon_sprite[0][9].getScale().x >= 1.5)			//Checks if mouse left button is clicked and see where it is
		{
			levelnumber = 0;
			bool isMouseOnTileX = false;
			bool isMouseOnTileY = false;

			for (int j = 0; j < 10; j++)
			{
				if (mouseposition.x < 40 + 75 + 125 * j && mouseposition.x > 40 + 125 * j)
				{
					isMouseOnTileX = true;
					levelnumber = j + 1;
				}
			}

			for (int i = 0; i < 6; i++)
			{
				if (mouseposition.y < 40 + 75 + 107 * i && mouseposition.y > 40 + 107 * i)
				{
					isMouseOnTileY = true;
					levelnumber += i * 10;
				}
			}

			if (isMouseOnTileX && isMouseOnTileY)
			{
				std::cout << levelnumber << std::endl;
				break;
			}

		}

		if(completedgame)
			return 1;

		Window.clear();
		Window.draw(backgroundVertex);
		for (int i = 0; i < 6; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				Window.draw(icon_sprite[i][j]);
				Window.draw(icon_level[i][j]);
			}
		}
		Window.draw(helpSprite);
		Window.draw(needhelpSprite);
		Window.display();
	}
	return 0;
}


int level(sf::RenderWindow & Window, sf::VertexArray & backgroundVertex, int & levelnumber)
{
	enum Direction {STILL, LEFT, RIGHT, UP, DOWN, ANIMATION}; 
	sf::Vector2f futureboxPosition(43,43);
	sf::Vector2i cursorposition(0,0);
	int movement = STILL, movecount = 0;

	sf::RectangleShape selectingbox, pausemenu;
	selectingbox.setOutlineThickness(3);
	selectingbox.setSize(sf::Vector2f(75 - 2 * selectingbox.getOutlineThickness(), 150 -  2 *selectingbox.getOutlineThickness()));
	selectingbox.setFillColor(sf::Color::Transparent);
	selectingbox.setOutlineColor(sf::Color::White);
	selectingbox.setPosition(43,43);
	 
	pausemenu.setOutlineThickness (5);
	pausemenu.setSize(sf::Vector2f(400,300));
	pausemenu.setFillColor( sf::Color(0,255,128,255));
	pausemenu.setOutlineColor (sf::Color(0,170,90,255));
	pausemenu.setPosition( (1280 - pausemenu.getSize().x) / 2, (720 - pausemenu.getSize().y) / 2);

	blockFont.loadFromFile("Resources/fonts/blocks.otf");
	levelFont.loadFromFile("Resources/fonts/levelfont2.otf");
	sf::Text displaylevel;
	displaylevel.setFont(levelFont);

	sf::Texture helpTexture;
	helpTexture.loadFromFile("Resources/images/help.png");
	sf::Sprite helpSprite;
	helpSprite.setTexture(helpTexture);
	helpSprite.setColor(sf::Color(255,255,255,95));
	helpSprite.setPosition(1280-50, 0);

	sf::Texture needhelpTexture;
	needhelpTexture.loadFromFile("Resources/images/help/helpplay.png");
	needhelpTexture.setSmooth(1);
	sf::Sprite needhelpSprite;
	needhelpSprite.setTexture(needhelpTexture);
	needhelpSprite.setColor(sf::Color::Transparent);

	sf::Texture backgroundTexture, pauseScreenTexture;
	sf::Sprite backgroundSprite, pauseScreenSprite;

	backgroundTexture.loadFromFile("Resources/images/mapedit_background.png");
	backgroundTexture.setSmooth(1);
	backgroundSprite.setTexture(backgroundTexture);
	backgroundSprite.setPosition(40,40);
	backgroundSprite.setColor(sf::Color(255,255,255,100));

	int maptile2d[8][16];					// 2d array maptiles. read from resources/maps/*.txt
	int maptile2d_copy[8][16];
	int slide_position_x[8][16];
	int possiblemoves;
	int loop_colorchange=0;

	bool finished = false, isScreenPaused = false;

	std::ostringstream filename[61];
	filename[levelnumber] << "Resources/maps/" << levelnumber << ".txt";

	std::ifstream in_maptile[61];
	in_maptile[levelnumber].open(filename[levelnumber].str());
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			in_maptile[levelnumber] >> maptile2d[i][j];			// using ifstream to put integers into maptile array
			std::cout << maptile2d[i][j] << " ";
		}
		std::cout << std::endl;
	}
	in_maptile[levelnumber] >> possiblemoves;
	in_maptile[levelnumber].close();

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
				}
			}
			if (event.type == sf::Event::KeyPressed && movement==STILL && windowhasfocus)		// Key responses only work when there is no animation
			{
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
						if (cursorposition.y < 6)
						{
							movement = DOWN;
							futureboxPosition = sf::Vector2f(selectingbox.getPosition().x, selectingbox.getPosition().y + 75);
							cursorposition.y++;
						}
						break;

					case sf::Keyboard::Add:		// Increases levelnumber by 1 and resets the stage
						if (levelnumber != 60)
						{
							levelnumber++;
							filename[levelnumber].str("");
							filename[levelnumber].clear();
							filename[levelnumber] << "Resources/maps/" << levelnumber << ".txt";
							in_maptile[levelnumber].open(filename[levelnumber].str());
							for (int i = 0; i < 8; i++)
							{
								for (int j = 0; j < 16; j++)
								{
									in_maptile[levelnumber] >> maptile2d[i][j];			// using ifstream to put integers into maptile array
									//std::cout << maptile2d[i][j] << " ";
								}
								//std::cout << std::endl;
							}
							if (!map.load("Resources/images/tileset.png", sf::Vector2u(50, 50), maptile2d, 16, 8))
							return -1;
							futureboxPosition = sf::Vector2f(43,43);
							cursorposition = sf::Vector2i(0,0);
							finished = false;
							movecount = 0;
							in_maptile[levelnumber] >> possiblemoves;
							in_maptile[levelnumber].close();
						}
						break;

					case sf::Keyboard::Subtract:		// Decreases levelnumber by 1 and resets the stage
						if (levelnumber !=1)
						{
							levelnumber--;
							filename[levelnumber].str("");
							filename[levelnumber].clear();
							filename[levelnumber] << "Resources/maps/" << levelnumber << ".txt";
							in_maptile[levelnumber].open(filename[levelnumber].str());
							for (int i = 0; i < 8; i++)
							{
								for (int j = 0; j < 16; j++)
								{
									in_maptile[levelnumber] >> maptile2d[i][j];			// using ifstream to put integers into maptile array
									std::cout << maptile2d[i][j] << " ";
								}
								std::cout << std::endl;
							}
							if (!map.load("Resources/images/tileset.png", sf::Vector2u(50, 50), maptile2d, 16, 8))
							return -1;
							futureboxPosition = sf::Vector2f(43,43);
							cursorposition = sf::Vector2i(0,0);
							finished = false;
							movecount = 0;
							in_maptile[levelnumber] >> possiblemoves;
							in_maptile[levelnumber].close();
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
		if (finished || movecount >=possiblemoves)
		{
			Window.clear();
			Window.draw(backgroundVertex);
			Window.draw(backgroundSprite);
			Window.draw(map);
			Window.draw(displaylevel);

			pauseScreenTexture.create(Window.getSize().x,Window.getSize().y);
			pauseScreenTexture.update(Window);
			pauseScreenSprite.setTexture(pauseScreenTexture);
			if (pauseScreen(Window, pauseScreenSprite, finished, levelnumber) == 0 )
			{
				if(finished)
				{
					if (levelnumber == 60)
					{
						return 0;
					}
					else
					{
						levelnumber++;
						filename[levelnumber].str("");
						filename[levelnumber].clear();
						filename[levelnumber] << "Resources/maps/" << levelnumber << ".txt";
						std::cout << "Next level" << std::endl;
					}
				}
				else
				{
					std::cout<<"Try again" << std::endl;
				}
				in_maptile[levelnumber].open(filename[levelnumber].str());
				for (int i = 0; i < 8; i++)
				{
					for (int j = 0; j < 16; j++)
					{
						in_maptile[levelnumber] >> maptile2d[i][j];			// using ifstream to put integers into maptile array
						//std::cout << maptile2d[i][j] << " ";
					}
					//std::cout << std::endl;
				}
				if (!map.load("Resources/images/tileset.png", sf::Vector2u(50, 50), maptile2d, 16, 8))
				return -1;
				futureboxPosition = sf::Vector2f(43,43);
				cursorposition = sf::Vector2i(0,0);
				finished = false;
				movecount = 0;
				in_maptile[levelnumber] >> possiblemoves;
				in_maptile[levelnumber].close();
			}
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

		//	Displays level on the bottom of the screen
		std::string levelstring = std::to_string(levelnumber);
		displaylevel.setColor(sf::Color::White);
		displaylevel.setString(levelstring);
		displaylevel.setCharacterSize(160);
		displaylevel.setScale(0.5,0.5);
		displaylevel.setPosition( (1280 - displaylevel.getGlobalBounds().width) / 2, (720 - displaylevel.getGlobalBounds().height) / 2 + 295);


		// Draws objects on Window and displays them
		Window.clear();
		Window.draw(backgroundVertex);
		Window.draw(backgroundSprite);
		Window.draw(map);
		Window.draw(displaylevel);
		Window.draw(selectingbox);
		Window.draw(helpSprite);
		Window.draw(needhelpSprite);
		Window.display();
	}

	return 0;
}




int pauseScreen(sf::RenderWindow & Window, sf::Sprite background, bool finished, int levelnumber)  // This function is called when stage is completed or failed
{
	background.setScale(sf::Vector2f((float)1280 / Window.getSize().x, (float) 720 / Window.getSize().y));

	// Loads failed soundeffect
	sf::SoundBuffer youdiedbuffer;
	youdiedbuffer.loadFromFile("Resources/sounds/youdied.ogg");
	sf::Sound youdied;
	youdied.setBuffer(youdiedbuffer);

	sf::Text message;
	message.setFont(blockFont);

	std::string levelstring = std::to_string(levelnumber);
	if (finished)
	{
		if (levelnumber == -1)
		{
			message.setString("Level Complete");
			message.setColor(sf::Color::White);
		}
		else
		{
			message.setString("Level " + levelstring + " Complete");
			message.setColor(sf::Color::White);
			if(levelnumber == 60)
			{
				message.setString("Game Over");
				message.setColor(sf::Color::White);
				completedgame = true;
			}
		}
	}
	else
	{
		youdied.play();
		message.setString("Try Again");
		message.setColor(sf::Color::Red);
		intro_piano.pause();
	}
	message.setCharacterSize(400);
	message.setScale(0.5,0.5);
	message.setPosition( (1280 - message.getGlobalBounds().width) /2 , (720 - message.getGlobalBounds().height)/2 - message.getGlobalBounds().height /2);

	while(1)
	{
		sf::Event event;
        while (Window.pollEvent(event))
        {
			if (event.type == sf::Event::KeyPressed)
			{
				switch (event.key.code)
				{
					case sf::Keyboard::Escape:
						if (intro_piano.getStatus() == sf::Music::Status::Paused)
							intro_piano.play();
						return 0;
						break;
					case sf::Keyboard::Right:
						break;
					case sf::Keyboard::Left:
						break;
					case sf::Keyboard::Space:	// Ends the pause screen and goes back to the game
						if (intro_piano.getStatus() == sf::Music::Status::Paused)
							intro_piano.play();
						return 0;
						break;
					case sf::Keyboard::Return:
						if (intro_piano.getStatus() == sf::Music::Status::Paused)
							intro_piano.play();
						return 0;
						break;
				}
			}
			if(event.type == sf::Event::MouseButtonPressed)
					if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
					{
						if (intro_piano.getStatus() == sf::Music::Status::Paused)
							intro_piano.play();
						return 0;
					}
		}

		Window.clear();
		Window.draw(background);
		Window.draw(message);
		Window.display();
	}
}