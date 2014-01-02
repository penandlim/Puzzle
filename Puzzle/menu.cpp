#include "menu.h"
#include "puzzle.h"

int colorchange(int color);
int premenu(sf::RenderWindow & Window, sf::VertexArray & top, sf::VertexArray & bot, sf::Texture & screenshot);

sf::Music intro_piano;
sf::Music intro_8bit;
bool applaunch = true;

int menu(sf::RenderWindow & Window, sf::VertexArray & backgroundVertex)
{
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

		intro_8bit.openFromFile("Resources/musics/intro_8bit.ogg");
		intro_piano.openFromFile("Resources/musics/intro_piano.ogg");
		intro_8bit.setVolume(100);
		intro_8bit.play();

		if (premenu(Window, top, bot, screenshot) == 1)
			return EXIT;
	}
	else
	{
		if(bgmisplaying == false)
			intro_piano.play();
		intro_8bit.stop();
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

	int loop_colorchange = 0, loop_cut = 0, loop_colorchange_max = 5, loop_animation = 0, animationState = 99, selection = SINGLE;

	

	sf::Font menuFont;
	menuFont.loadFromFile("Resources/fonts/blocks.otf");

	sf::Text menuTitle;
	menuTitle.setString("Puzzle Frenzy");
	menuTitle.setFont(menuFont);
	menuTitle.setCharacterSize(250);
	menuTitle.setScale(0.01,0.01);
	menuTitle.setPosition((1280 - menuTitle.getGlobalBounds().width) / 2, 360 - menuTitle.getGlobalBounds().height);

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
		if (menuTitle.getScale().x < 1)
		{
			if(1 - menuTitle.getScale().x < 0.2)
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
		if(intro_8bit.getVolume() > 0)
		{
			intro_piano.setVolume( intro_piano.getVolume() + 0.5);
			intro_8bit.setVolume( intro_8bit.getVolume() - 0.5);
		}

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

		//	Window section.
		Window.clear();
		Window.draw(backgroundVertex);
		Window.draw(menuTitle);
		if(menuTitle.getScale().x >= 1)
		{
			for (int i = 0; i < 4; i++)
			{
				if (menuOption[i].getColor().a < 255)
					menuOption[i].setColor( sf::Color(255,255,255, menuOption[i].getColor().a + 5));
				Window.draw(menuOption[i]);
			}
			if (menuOption[0].getColor().a == 250)
				animationState = STILL;
		}
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

	sf::Font frenzyFont;
	frenzyFont.loadFromFile("Resources/fonts/frenzy.ttf");

	sf::Text frenzyText;
	frenzyText.setStyle(sf::Text::Italic);
	frenzyText.setString("frenzy");
	frenzyText.setColor(sf::Color(0,0,0,100));
	frenzyText.setFont(puzzleFont);
	frenzyText.setCharacterSize(150);
	frenzyText.setPosition( (1280 - frenzyText.getGlobalBounds().width) / 2 - 25, 360 - frenzyText.getGlobalBounds().height + 250);

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
						intro_piano.setPlayingOffset(intro_8bit.getPlayingOffset());
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
			intro_8bit.setVolume( intro_8bit.getVolume() - 0.5);
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
		if(draw_crash)
		{
			Window.draw(slashSprite);
			Window.draw(whiteout);
		}
		Window.display();
	}

	screenshot.create(1280,720);
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
	bot[1].texCoords = sf::Vector2f(1280,720);
	bot[2].texCoords = sf::Vector2f(0,720);

	top[0].position = sf::Vector2f(0,0);
	top[1].position = sf::Vector2f(1280,720);
	top[2].position = sf::Vector2f(1280,0);

	top[0].texCoords = sf::Vector2f(0,0);
	top[1].texCoords = sf::Vector2f(1280,720);
	top[2].texCoords = sf::Vector2f(1280,0);
	
	return 0;
}