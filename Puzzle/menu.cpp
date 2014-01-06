#include "menu.h"
#include "puzzle.h"
#include<iostream>

int colorchange(int color);
int premenu(sf::RenderWindow & Window, sf::VertexArray & top, sf::VertexArray & bot, sf::Texture & screenshot);

sf::Music intro_piano;
bool applaunch = true;

int menu(sf::RenderWindow & Window, sf::VertexArray & backgroundVertex)
{
	/*if (applaunch == true)
	{
		while(Window.isOpen())
		{
			sf::Event event;
			while (Window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					Window.close();
			}
			Window.display();
			if(event.type == sf::Event::KeyPressed)
			{
				std::this_thread::sleep_for(std::chrono::seconds(3));
				break;
			}
		}
	}*/

	completedgame = false;

	enum animation{LEFT, RIGHT, STILL};
	enum mode{SINGLE, MULTI, EDIT, EXIT};

	/*sf::VertexArray transition(sf::Quads);
	transition.resize(72 * 4);*/
	sf::VertexArray top(sf::Triangles, 3);
	sf::VertexArray bot(sf::Triangles, 3);

	sf::Texture screenshot;

	if (applaunch == true)
	{
		srand(time(NULL));

		backgroundVertex[0].position = sf::Vector2f(0,0);
		backgroundVertex[1].position = sf::Vector2f(1280,0);
		backgroundVertex[2].position = sf::Vector2f(1280,720);
		backgroundVertex[3].position = sf::Vector2f(0,720);

		backgroundVertex[0].color = sf::Color(77, 129, 166, 255);
		backgroundVertex[1].color = sf::Color(255, 185, 231, 255);
		backgroundVertex[2].color = sf::Color(255, 185, 231, 255);
		backgroundVertex[3].color = sf::Color(93, 134, 172, 255);

		intro_piano.openFromFile("Resources/musics/intro_piano.ogg");
		intro_piano.setLoop(true);

		if (premenu(Window, top, bot, screenshot) == 1)
			return EXIT;
	}
	applaunch = false;

	/*bool start = false, draw_crash = false;*/

	//sf::Texture brokenTexture;
	//brokenTexture.loadFromFile("Resources/images/broken.png");

	//sf::Sprite brokenSprite;
	//brokenSprite.setTexture(brokenTexture);
	//sf::Clock crashTimer;

	//while(!start)
	//{
	//	sf::Event event;
	//	while (Window.pollEvent(event))
 //       {
 //           if (event.type == sf::Event::Closed)
 //               Window.close();
	//		if (event.type == sf::Event::KeyPressed)
	//		{
	//			switch (event.key.code)
	//			{
	//				case sf::Keyboard::Space:
	//					draw_crash = true;
	//					crashTimer.restart();
	//					break;
	//			}
	//		}
	//	}
	//	if(draw_crash)
	//		if(crashTimer.getElapsedTime().asSeconds() > 2)
	//			start = true;

	//	Window.clear();
	//	Window.draw(transition, &screenshot);
	//	if(draw_crash)
	//		Window.draw(brokenSprite);
	//	Window.display();
	//}

	sf::Clock clickcount;

	int loop_colorchange = 0, loop_cut = 0, loop_colorchange_max = 5, loop_animation = 0, animationState = 99, selection = SINGLE;

	sf::Texture mapeditTexture, globeTexture;
	mapeditTexture.loadFromFile("Resources/images/mapedit.png");
	mapeditTexture.setSmooth(1);
	globeTexture.loadFromFile("Resources/images/globe.png");
	globeTexture.setSmooth(1);
	sf::Sprite mapeditSprite, globeSprite;
	globeSprite.setTexture(globeTexture);
	globeSprite.setColor(sf::Color(255,255,255,0));
	globeSprite.setPosition(640 - 250, 360 - 250);
	mapeditSprite.setTexture(mapeditTexture);
	mapeditSprite.setColor(sf::Color(255,255,255,0));
	float mapeditSpriteOpacity = 0;

	sf::Texture leftarrowTexture, rightarrowTexture;
	sf::Sprite leftarrowSprite, rightarrowSprite;
	leftarrowTexture.loadFromFile("Resources/images/leftarrow.png");
	leftarrowTexture.setSmooth(1);
	rightarrowTexture.loadFromFile("Resources/images/rightarrow.png");
	rightarrowTexture.setSmooth(1);
	leftarrowSprite.setTexture(leftarrowTexture);
	leftarrowSprite.setColor(sf::Color(255,255,255,0));
	rightarrowSprite.setTexture(rightarrowTexture);
	rightarrowSprite.setColor(sf::Color(255,255,255,0));

	sf::Font menuFont;
	menuFont.loadFromFile("Resources/fonts/blocks.otf");

	sf::Text menuTitle;
	menuTitle.setString("Puzzle Frenzy Redux");
	menuTitle.setFont(menuFont);
	menuTitle.setCharacterSize(250);
	menuTitle.setScale(0.01,0.01);
	menuTitle.setPosition((1280 - menuTitle.getGlobalBounds().width) / 2, 360 - menuTitle.getGlobalBounds().height);

	sf::Texture helpTexture;
	helpTexture.loadFromFile("Resources/images/help.png");
	sf::Sprite helpSprite;
	helpSprite.setTexture(helpTexture);
	helpSprite.setColor(sf::Color(255,255,255,95));
	helpSprite.setPosition(1280-50, 0);

	sf::Texture needhelpTexture;
	needhelpTexture.loadFromFile("Resources/images/help/helpmain.png");
	needhelpTexture.setSmooth(1);
	sf::Sprite needhelpSprite;
	needhelpSprite.setTexture(needhelpTexture);
	needhelpSprite.setColor(sf::Color::Transparent);

	sf::Text menuOption[4];
	menuOption[0].setString("Single-Play");
	menuOption[1].setString("Multi-Play");
	menuOption[2].setString("Map Editor");
	menuOption[3].setString("Exit");

	for (int i = 0; i < 4; i++)
	{
		menuOption[i].setFont(menuFont);
		menuOption[i].setCharacterSize(70);
		menuOption[i].setPosition( 1280 * i + (1280 - menuOption[i].getGlobalBounds().width) / 2, 600);
		menuOption[i].setColor(sf::Color::Transparent);
	}

	sf::RectangleShape blackscreen;
	blackscreen.setSize(sf::Vector2f(1280,720));
	blackscreen.setFillColor(sf::Color(0,0,0,0));

	sf::Mouse mouse;
	sf::Vector2f mouseposition;

	while(Window.isOpen())
	{
		sf::Event event;
        while (Window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                Window.close();
			if (event.type == sf::Event::KeyPressed)
			{
				if( animationState == STILL)
				switch (event.key.code)
				{
					case sf::Keyboard::Space:
						return selection;
						break;
					case sf::Keyboard::Return:
						return selection;
						break;
					case sf::Keyboard::Left:
						if (selection != SINGLE)
						{
							animationState = LEFT;
							loop_animation = 0;
							selection--;
						}
						break;
					case sf::Keyboard::Right:
						if (selection != EXIT)
						{
							animationState = RIGHT;
							loop_animation = 0;
							selection++;
						}
						break;
				}
			}
			if (event.type == sf::Event::MouseButtonPressed && animationState == STILL)
			{
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
				{
					if(mouseposition.x > 355 && mouseposition.y > 614 && mouseposition.x < 433 && mouseposition.y < 668)
					{
						if (selection != SINGLE)
						{
							animationState = LEFT;
							loop_animation = 0;
							selection--;
						}
					}
					else if(mouseposition.x > 845 && mouseposition.y > 614 && mouseposition.x < 845 + 78 && mouseposition.y < 668)
					{
						if (selection != EXIT)
						{
							animationState = RIGHT;
							loop_animation = 0;
							selection++;
						}
					}
					else if(mouseposition.x > 450 && mouseposition.y > 600 && mouseposition.x < 828 && mouseposition.y < 690)
					{
						if(clickcount.getElapsedTime().asMilliseconds() < 1000)
						{
							return selection;
						}
						clickcount.restart();
					}
				}
			}
		}
		loop_colorchange++;
		if (loop_colorchange >= loop_colorchange_max)
		{
			srand ((unsigned)time(NULL));
			for (int i = 0; i < 4; i++)
			{
				backgroundVertex[i].color = sf::Color(colorchange(backgroundVertex[i].color.r), colorchange(backgroundVertex[i].color.g), colorchange(backgroundVertex[i].color.b), 255);
			}
			loop_colorchange = 0;
		}
		if (menuTitle.getScale().x < 0.7)
		{
			if(menuTitle.getScale().x > 0.5)
			{
				menuTitle.setScale(menuTitle.getScale().x + 0.003,menuTitle.getScale().y + 0.003);
				menuTitle.setPosition((1280 - menuTitle.getGlobalBounds().width) / 2, 360 - menuTitle.getGlobalBounds().height);
			}
			else
			{
				menuTitle.setScale(menuTitle.getScale().x + 0.005,menuTitle.getScale().y + 0.005);
				menuTitle.setPosition((1280 - menuTitle.getGlobalBounds().width) / 2, 360 - menuTitle.getGlobalBounds().height);
			}
		}

		if (loop_cut < 300)
		{
			loop_cut++;
				/*for (int i = 0; i < 72; i++)
				{
					sf::Vertex* quad_transition = &transition[i*4];
					for (int k = 0; k < 4; k++)
					{
						quad_transition[k].position.y += change_y[i];
						quad_transition[k].position.y += loop_cut / 2;
					}
				
				}*/
			for (int i = 0; i < 3; i++)
			{
				bot[i].position.x -=1.28 * loop_cut;
				bot[i].position.y -=0.72 * loop_cut;
				top[i].position.x +=1.28 * loop_cut;
				top[i].position.y +=0.72 * loop_cut;
			}
		}
		if (selection == SINGLE)
		{
			if(leftarrowSprite.getColor().a > 0)
				leftarrowSprite.setColor(sf::Color(255,255,255,leftarrowSprite.getColor().a -5));
			if(globeSprite.getColor().a > 0)
				globeSprite.setColor(sf::Color(255,255,255,globeSprite.getColor().a -3));
		}
		else if (selection == MULTI)
		{
			if(leftarrowSprite.getColor().a < 255)
				leftarrowSprite.setColor(sf::Color(255,255,255,leftarrowSprite.getColor().a +5));
			if(globeSprite.getColor().a < 150)
				globeSprite.setColor(sf::Color(255,255,255,globeSprite.getColor().a +3));
		}
		else if (selection == EDIT)
		{
			if(rightarrowSprite.getColor().a < 255)
				rightarrowSprite.setColor(sf::Color(255,255,255,rightarrowSprite.getColor().a +5));
			if(globeSprite.getColor().a > 0)
				globeSprite.setColor(sf::Color(255,255,255,globeSprite.getColor().a -3));
			if(blackscreen.getFillColor().a > 0)
				blackscreen.setFillColor(sf::Color(0,0,0,blackscreen.getFillColor().a - 5));
		}
		else if (selection == EXIT)
		{
			if(rightarrowSprite.getColor().a > 0)
				rightarrowSprite.setColor(sf::Color(255,255,255,rightarrowSprite.getColor().a - 5));
			if(blackscreen.getFillColor().a < 255)
				blackscreen.setFillColor(sf::Color(0,0,0,blackscreen.getFillColor().a + 5));
		}

		
		if (selection == EDIT)			// if map editor is selected
		{
			if (mapeditSpriteOpacity < 255)
			{
				mapeditSpriteOpacity += 4.25;
				mapeditSprite.setColor(sf::Color(255,255,255, mapeditSpriteOpacity));
			}
		}
		else
		{
			if (mapeditSpriteOpacity > 0)
			{
				mapeditSpriteOpacity -= 4.25;
				mapeditSprite.setColor(sf::Color(255,255,255, mapeditSpriteOpacity));
			}
		}

		if(intro_piano.getVolume() < 100)
			intro_piano.setVolume( intro_piano.getVolume() + 0.5);

		if(animationState == LEFT)
		{
			loop_animation++;
			loop_colorchange_max = 1;
			for (int i = 0; i < 4; i++)
			{
				menuOption[i].move(20,0);
			}
			if (loop_animation == 64)
			{
				loop_colorchange_max = 5;
				animationState = STILL;
			}
		}

		else if(animationState == RIGHT)
		{
			loop_animation++;
			loop_colorchange_max = 1;
			for (int i = 0; i < 4; i++)
			{
				menuOption[i].move(-20,0);
			}
			if (loop_animation == 64)
			{
				loop_colorchange_max = 5;
				animationState = STILL;
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

		//	Window section.
		Window.clear();
		Window.draw(backgroundVertex);
		Window.draw(mapeditSprite);
		Window.draw(globeSprite);
		Window.draw(blackscreen);
		Window.draw(menuTitle);
		if(menuTitle.getScale().x >= 0.7 && menuOption[0].getColor().a < 255)
		{
			for (int i = 0; i < 4; i++)
			{
				if (menuOption[i].getColor().a < 255)
					menuOption[i].setColor( sf::Color(255,255,255, menuOption[i].getColor().a + 5));
			}
			if(rightarrowSprite.getColor().a < 255)
				rightarrowSprite.setColor(sf::Color(255,255,255, rightarrowSprite.getColor().a + 5));

			if (menuOption[0].getColor().a == 255)
				animationState = STILL;
		}
		for (int i = 0; i < 4; i++)
		{
			Window.draw(menuOption[i]);
		}
		Window.draw(leftarrowSprite);
		Window.draw(rightarrowSprite);
		Window.draw(needhelpSprite);
		Window.draw(helpSprite);
		if (loop_cut < 300)
			Window.draw(bot, &screenshot);
		Window.draw(top, &screenshot);
		Window.display();
	}
	return 0;
}




int colorchange(int color)
{
	int result = rand() % 3 - 1;
	result = color - result;

	if (result > 255)
		result = 255;
	else if (result < 0 )
		result = 0;

	return result;
}



int premenu(sf::RenderWindow & Window, sf::VertexArray & top, sf::VertexArray & bot, sf::Texture & screenshot)
{
	sf::Font puzzleFont;
	puzzleFont.loadFromFile("Resources/fonts/puzzle.ttf");

	sf::Text puzzleText;
	puzzleText.setString("PUZZLE");
	puzzleText.setColor(sf::Color(0,0,0,100));
	puzzleText.setFont(puzzleFont);
	puzzleText.setCharacterSize(150);
	puzzleText.setPosition( (1280 - puzzleText.getGlobalBounds().width) / 2, 360 - puzzleText.getGlobalBounds().height );

	sf::Font frenzyFont, nameFont;
	frenzyFont.loadFromFile("Resources/fonts/frenzy.ttf");
	nameFont.loadFromFile("Resources/fonts/filename.otf");

	sf::Text frenzyText, nameText;
	frenzyText.setStyle(sf::Text::Italic);
	frenzyText.setString("frenzy");
	frenzyText.setColor(sf::Color(0,0,0,100));
	frenzyText.setFont(puzzleFont);
	frenzyText.setCharacterSize(150);
	frenzyText.setPosition( (1280 - frenzyText.getGlobalBounds().width) / 2 - 25, 360 - frenzyText.getGlobalBounds().height + 250);
	nameText.setString("Jongseung Lim\nJonghwa Lim\nChris Morski");
	nameText.setColor(sf::Color(0,0,0,100));
	nameText.setFont(nameFont);
	nameText.setCharacterSize(20);
	nameText.setPosition(1100,640);

	sf::Text pressStart;
	pressStart.setString("Press Space");
	pressStart.setColor(sf::Color(0,0,0,100));
	pressStart.setFont(frenzyFont);
	pressStart.setCharacterSize(50);
	pressStart.setPosition( (1280 - pressStart.getGlobalBounds().width) / 2 - 25, 620);


	bool start = false, draw_crash = false;

	sf::RectangleShape whiteout;
	whiteout.setSize(sf::Vector2f(1280,720));
	whiteout.setFillColor(sf::Color::White);

	sf::Texture slashTexture;
	slashTexture.loadFromFile("Resources/images/slash.png");
	slashTexture.setSmooth(1);

	sf::Sprite slashSprite;
	slashSprite.setTexture(slashTexture);
	sf::Clock crashTimer;

	while(!start)
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
					case sf::Keyboard::Space:
						draw_crash = true;
						intro_piano.setVolume(0);
						intro_piano.play();
						crashTimer.restart();
						break;
					case sf::Keyboard::Escape:
						return 1;
						break;
				}
			}
		}
		if(draw_crash)
		{
			intro_piano.setVolume( intro_piano.getVolume() + 0.5);
			if(whiteout.getFillColor().a != 0)
				whiteout.setFillColor( sf::Color(255,255,255, whiteout.getFillColor().a - 5));
			if(crashTimer.getElapsedTime().asMilliseconds() > 1000)
			{
				start = true;
			}
		}

		Window.clear(sf::Color(72,85,11,255));
		Window.draw(puzzleText);
		Window.draw(frenzyText);
		Window.draw(pressStart);
		Window.draw(nameText);
		if(draw_crash)
		{
			Window.draw(slashSprite);
			Window.draw(whiteout);
		}
		Window.display();
	}

	screenshot.create(Window.getSize().x,Window.getSize().y);
	screenshot.update(Window);

	//for(int i = 0; i < 72; i++)
	//{
	//		sf::Vertex* quad_transition = &transition[i*4];
	//		quad_transition[0].position = sf::Vector2f(0, i * 10);
	//		quad_transition[1].position = sf::Vector2f(1280, i * 10);
	//		quad_transition[2].position = sf::Vector2f(1280, i * 10 + 10);
	//		quad_transition[3].position = sf::Vector2f(0, i * 10 + 10);

	//		quad_transition[0].texCoords = sf::Vector2f(0, i * 10);
	//		quad_transition[1].texCoords = sf::Vector2f(1280, i * 10);
	//		quad_transition[2].texCoords = sf::Vector2f(1280, i * 10 + 10);
	//		quad_transition[3].texCoords = sf::Vector2f(0, i * 10 + 10);
	//		//transitionMovement[j * 128 + i] = sf::Vector2f( (rand() % 300 - 100) * 1.28 / 100 , (rand() % 300 - 100) * 0.72 / 100);

	//		/*change_x[i] = 1.28 * (rand() % 201 - 100) / 100;*/
	//	change_y[i] = 7.2 * (rand() % 21 - 20) / 10;
	//}

	bot[0].position = sf::Vector2f(0,0);
	bot[1].position = sf::Vector2f(1280,720);
	bot[2].position = sf::Vector2f(0,720);

	bot[0].texCoords = sf::Vector2f(0,0);
	bot[1].texCoords = sf::Vector2f(Window.getSize().x,Window.getSize().y);
	bot[2].texCoords = sf::Vector2f(0,Window.getSize().y);

	top[0].position = sf::Vector2f(0,0);
	top[1].position = sf::Vector2f(1280,720);
	top[2].position = sf::Vector2f(1280,0);

	top[0].texCoords = sf::Vector2f(0,0);
	top[1].texCoords = sf::Vector2f(Window.getSize().x,Window.getSize().y);
	top[2].texCoords = sf::Vector2f(Window.getSize().x,0);
	
	return 0;
}