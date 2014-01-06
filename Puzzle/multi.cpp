#include "multi.h"
#include <iostream>

bool ishost = false;
int multiplayer(sf::RenderWindow & Window, sf::VertexArray & backgroundVertex, bool ishost);


int multiplayer_select(sf::RenderWindow & Window, sf::VertexArray & backgroundVertex)
{
	sf::Mouse mouse;
	sf::Vector2f mouseposition;
	int loop_colorchange = 0;

	sf::RectangleShape ipbox;
	ipbox.setSize(sf::Vector2f(400,50));
	ipbox.setFillColor(sf::Color::White);
	ipbox.setPosition(640 + 1280 / 4 - 200, 395);

	sf::Texture globeTexture;
	globeTexture.loadFromFile("Resources/images/globe.png");
	globeTexture.setSmooth(1);
	sf::Sprite mapeditSprite, globeSprite;
	globeSprite.setTexture(globeTexture);
	globeSprite.setColor(sf::Color(255,255,255,100));
	globeSprite.setPosition(640 - 250, 360 - 250);

	bool textmode;
	sf::Font customFont;
	customFont.loadFromFile("Resources/fonts/filename.otf");

	sf::TcpListener listener;
	sf::TcpSocket client, socket;

	sf::String ipstring("000.000.000.000");
	sf::Text hostText, connectText, ipText;
	hostText.setFont(customFont);
	hostText.setString("Host Game");
	hostText.setCharacterSize(50);
	hostText.setPosition(1280 / 4 - hostText.getGlobalBounds().width / 2, 300);
	connectText.setFont(customFont);
	connectText.setString("Connect to Host");
	connectText.setCharacterSize(50);
	connectText.setPosition(640 + 1280 / 4 - connectText.getGlobalBounds().width / 2, 300);
	ipText.setFont(customFont);
	ipText.setString(ipstring);
	ipText.setCharacterSize(30);
	ipText.setColor(sf::Color::Black);
	ipText.setPosition(640 + 1280 / 4 - ipText.getGlobalBounds().width / 2, 400);

	sf::Texture helpTexture;
	helpTexture.loadFromFile("Resources/images/help.png");
	sf::Sprite helpSprite;
	helpSprite.setTexture(helpTexture);
	helpSprite.setColor(sf::Color(255,255,255,95));
	helpSprite.setPosition(1280-50, 0);

	sf::Texture needhelpTexture;
needhelpTexture.loadFromFile("Resources/images/help/helpmulti.png");
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
			if(event.type == sf::Event::TextEntered && textmode)
			{
				if(ipstring.getSize() < 15)
				{
					if(event.text.unicode >= 32 && event.text.unicode <= 126)
						ipstring += (char)event.text.unicode; 
					else if(event.text.unicode == 8 && ipstring.getSize() > 0)
						ipstring.erase(ipstring.getSize() - 1);
				}
				else
					if(event.text.unicode == 8)
						ipstring.erase(ipstring.getSize() - 1);

				if (event.text.unicode == 13)
				{
					textmode = false;
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


		if(mouseposition.x > ipbox.getPosition().x && mouseposition.y > ipbox.getPosition().y && mouseposition.x < ipbox.getPosition().x + 400 && mouseposition.y < ipbox.getPosition().y + 50)
		{
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				textmode = true;
			}
		}
		else
		{
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				textmode = false;
			}
				if (mouseposition.x > 181 && mouseposition.y > 305 && mouseposition.x < 463 && mouseposition.y < 354)
				{
					if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
					{
						// Host game
						hostText.setString("Hosting Game...");
						hostText.setPosition(1280 / 4 - hostText.getGlobalBounds().width / 2, 300);
					}
				}
				else 
				{
					if (mouseposition.x > 730 && mouseposition.y > 305 && mouseposition.x < 1198 && mouseposition.y < 354)
					{
						if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
						{
							//	Connect to Host
							connectText.setCharacterSize(30);
							connectText.setString("Connecting to " + ipstring);
							connectText.setPosition(640 + 1280 / 4 - connectText.getGlobalBounds().width / 2, 300);
							
						}

					}
				}
			

		}

		if(textmode)
		{
			ipbox.setOutlineColor(sf::Color::Red);
			ipbox.setOutlineThickness(2);
			ipText.setString(ipstring + "|");
		}
		else
		{
			ipbox.setOutlineThickness(0);
			ipText.setString(ipstring);
			ipText.setPosition(640 + 1280 / 4 - ipText.getGlobalBounds().width / 2, 400);
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
		Window.draw(globeSprite);
		Window.draw(hostText);
		Window.draw(connectText);
		Window.draw(ipbox);
		Window.draw(ipText);
		Window.draw(helpSprite);
		Window.draw(needhelpSprite);
		Window.display();
	}
}