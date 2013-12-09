#include "mapeditor.h"
#include "menu.h"

int mapeditor(sf::RenderWindow & Window, sf::VertexArray & backgroundVertex);


int mapeditor(sf::RenderWindow & Window, sf::VertexArray & backgroundVertex)
{
	enum Direction {STILL, LEFT, RIGHT, UP, DOWN, ANIMATION}; 
	sf::Vector2f futureboxPosition(43,43);
	sf::Vector2i cursorposition(0,0);
	int movement = STILL, movecount = 0;

	sf::Texture backgroundTexture;
	sf::Sprite backgroundSprite;

	backgroundTexture.loadFromFile("Resources/images/background.png");
	backgroundSprite.setTexture(backgroundTexture);
	backgroundSprite.setPosition(0,0);
	backgroundSprite.setColor(sf::Color(255,255,255,100));

	sf::RectangleShape selectingbox, pausemenu;
	selectingbox.setOutlineThickness(3);
	selectingbox.setSize(sf::Vector2f(75 - 2 * selectingbox.getOutlineThickness(), 75 -  2 *selectingbox.getOutlineThickness()));
	selectingbox.setFillColor(sf::Color::Transparent);
	selectingbox.setOutlineColor(sf::Color::White);
	selectingbox.setPosition(43,43);


	while(Window.isOpen())
	{
		sf::Event event;
        while (Window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                Window.close();
			if (event.type == sf::Event::KeyPressed && movement==STILL)		// Key responses only work when there is no animation
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
						if (cursorposition.y < 7)
						{
							movement = DOWN;
							futureboxPosition = sf::Vector2f(selectingbox.getPosition().x, selectingbox.getPosition().y + 75);
							cursorposition.y++;
						}
						break;
				}
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


		Window.clear();
		Window.draw(backgroundSprite);
		Window.draw(selectingbox);
		Window.display();
	}
}