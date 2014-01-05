#include<SFML/Graphics.hpp>
#include<iostream>
#include<fstream>
#include<sstream>

class TileMap : public sf::Drawable, public sf::Transformable
{
public:

    bool load(const std::string& tileset, sf::Vector2u tileSize, int tiles[][16], unsigned int width, unsigned int height)
    {
        // load the tileset texture
        if (!m_tileset.loadFromFile(tileset))
            return false;

		

        // resize the vertex array to fit the level size
        m_vertices.setPrimitiveType(sf::Quads);
        m_vertices.resize(width * height * 4);

        // populate the vertex array, with one quad per tile
        for (unsigned int i = 0; i < width; ++i)
            for (unsigned int j = 0; j < height; ++j)
            {
                // get the current tile number
                int tileNumber = tiles[j][i];


					// find its position in the tileset texture
					int tu = tileNumber % (m_tileset.getSize().x / tileSize.x);
					int tv = tileNumber / (m_tileset.getSize().x / tileSize.x);

					// get a pointer to the current tile's quad
					sf::Vertex* quad = &m_vertices[(i + j * width) * 4];

					// define its 4 corners
					quad[0].position = sf::Vector2f(i * tileSize.x , j * tileSize.y );
					quad[1].position = sf::Vector2f((i + 1) * tileSize.x , j * tileSize.y);
					quad[2].position = sf::Vector2f((i + 1) * tileSize.x , (j + 1) * tileSize.y );
					quad[3].position = sf::Vector2f(i * tileSize.x , (j + 1) * tileSize.y );

					//for (unsigned int k = 0; k < 4; k++)
					//{
					//	quad[k].position = sf::Vector2f( quad[k].position.x * 1.5 + 40, quad[k].position.y * 1.5 + 40);
					//}

					// define its 4 texture coordinates
					quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
					quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
					quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
					quad[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);

            }

        return true;
    }


	bool swap(sf::Vector2i cursorposition, int tiles[][16], int & loopnumber)
	{
		sf::Vertex quadi[4];
		sf::Vertex* quad_top = &m_vertices[(cursorposition.x + cursorposition.y * 16) * 4];
		sf::Vertex* quad_bot = &m_vertices[(cursorposition.x + (cursorposition.y + 1) * 16) * 4];

		loopnumber++;

		for (int i = 0; i < 4; i++)
		{
			quad_top[i].position.y +=  0.25 * loopnumber;
			quad_bot[i].position.y -=  0.25 * loopnumber;
		}

		if (loopnumber == 20)
		{
			for (int i = 0; i < 4; i++)
			{
				quad_top[i].position.y = (int)quad_top[i].position.y - (int)quad_top[i].position.y % 50;
				if (quad_bot[i].position.y < 0)
					quad_bot[i].position.y = 0;						// exception to the top row. needs to be set to 0 manually to prevent disappearing.
				else
				quad_bot[i].position.y = (int)quad_bot[i].position.y + 50 - (int)quad_bot[i].position.y % 50;
			}
			for (int i = 0; i < 4; i++)
			{
				quadi[i] = quad_top[i];
				quad_top[i] = quad_bot[i];
				quad_bot[i] = quadi[i];
			}

			// Actually swapping the tile data. This is done after the animation crap.
			int placeholder;
			placeholder = tiles[cursorposition.y + 1][cursorposition.x];
			tiles[cursorposition.y + 1][cursorposition.x] = tiles[cursorposition.y][cursorposition.x];
			tiles[cursorposition.y][cursorposition.x] = placeholder;
			
			return true;
		}
		else
			return false;
	}


	void detect(int tiles[][16], int tiles_copy[][16])
	{
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 16; j++)
			{
				tiles_copy[i][j] = tiles[i][j];
			}
		}

		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 16; j++)
			{
				if(j<14)
				if (tiles[i][j] == tiles[i][j+1] && tiles[i][j] == tiles[i][j+2])
				{
					tiles_copy[i][j] = 0;
					tiles_copy[i][j+1] = 0;
					tiles_copy[i][j+2] = 0;
				}
				if(i<6)
				if (tiles[i][j] == tiles[i+1][j] && tiles[i][j] == tiles[i+2][j])
				{
					tiles_copy[i][j] = 0;
					tiles_copy[i+1][j] = 0;
					tiles_copy[i+2][j] = 0;
				}
			}
		}
	}

	bool slide_data(int tiles[][16], int tiles_copy[][16], int slide_position_x[][16])
	{
		int k = 0;
		bool needslide = false;

		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 16; j++)
			{
				tiles_copy[i][j] = tiles[i][j];
				while (tiles[i][j] != 0 && tiles[i][j - (k +1)] == 0 && j - (k+1) >= 0 )
				{
					k++;
					needslide = true;
				}
				tiles[i][j-k] = tiles[i][j];
				if (k !=0)
					tiles[i][j] = 0;
				slide_position_x[i][j] = (j-k) * 50;
				k=0;
			}
		}
		
		return needslide;
	}

	bool slide_animation(int slide_position_x[][16], int & loopnumber)
	{
		loopnumber++;
		bool needslideanim = false;
		for (unsigned int i = 0; i < 16; ++i)
            for (unsigned int j = 0; j < 8; ++j)
            {
				sf::Vertex* quad = &m_vertices[(i + j * 16) * 4];
				if (quad[0].position.x != slide_position_x[j][i])
				{
				// define positions of current tile's quad
				quad[0].position.x -= 0.8 * loopnumber;
				quad[1].position.x -= 0.8 * loopnumber;
				quad[2].position.x -= 0.8 * loopnumber;
				quad[3].position.x -= 0.8 * loopnumber;

				if (quad[0].position.x < slide_position_x[j][i])
					quad[0].position.x = slide_position_x[j][i];
				if (quad[1].position.x < slide_position_x[j][i] + 50)
					quad[1].position.x = slide_position_x[j][i] + 50;
				if (quad[2].position.x < slide_position_x[j][i] + 50)
					quad[2].position.x = slide_position_x[j][i] + 50;
				if (quad[3].position.x < slide_position_x[j][i])
					quad[3].position.x = slide_position_x[j][i];

					needslideanim = true;
				}
			}
		return needslideanim;

	}

	bool fix(int tiles[][16], int tiles_copy[][16], sf::Vector2u tileSize, int & loopnumber)
	{

		bool isallblack = true;
		loopnumber++;

		for (unsigned int i = 0; i < 8; i++)
		{
			for (unsigned int j = 0; j < 16; j++)
			{
				if (tiles_copy[i][j] != tiles[i][j])
				{
					// get a pointer to the current tile's quad
					sf::Vertex* quad = &m_vertices[(j + i * 16) * 4];

					isallblack = false;
					for (int k = 0; k < 4; k++)
					{
						quad[k].color = sf::Color( quad[k].color.r, quad[k].color.g, quad[k].color.b , 255 - loopnumber * 5);
					}
				}
				// NEED THIS TO CORRECTLY DISPLAY TILES AFTER SLIDING
				int tileNumber = tiles[i][j];
				// find its position in the tileset texture
				int tu = tileNumber % (m_tileset.getSize().x / tileSize.x);
				int tv = tileNumber / (m_tileset.getSize().x / tileSize.x);
				sf::Vertex* quad = &m_vertices[(j + i * 16) * 4];
				// define its 4 texture coordinates
				quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
				quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
				quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
				quad[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
				// define positions of current tile's quad
				quad[0].position = sf::Vector2f(j * tileSize.x , i * tileSize.y );
				quad[1].position = sf::Vector2f((j + 1) * tileSize.x , i * tileSize.y);
				quad[2].position = sf::Vector2f((j + 1) * tileSize.x , (i + 1) * tileSize.y );
				quad[3].position = sf::Vector2f(j * tileSize.x , (i + 1) * tileSize.y );
			}
		}
		if (loopnumber==51)
		isallblack = true;

		if (isallblack)
		{
		 for (unsigned int i = 0; i < 16; ++i)
            for (unsigned int j = 0; j < 8; ++j)
            {
				tiles[j][i] = tiles_copy[j][i];
                // get the current tile number
                int tileNumber = tiles[j][i];


					// find its position in the tileset texture
					int tu = tileNumber % (m_tileset.getSize().x / tileSize.x);
					int tv = tileNumber / (m_tileset.getSize().x / tileSize.x);

					// get a pointer to the current tile's quad
					sf::Vertex* quad = &m_vertices[(i + j * 16) * 4];

					// define positions of current tile's quad
					quad[0].position = sf::Vector2f(i * tileSize.x , j * tileSize.y );
					quad[1].position = sf::Vector2f((i + 1) * tileSize.x , j * tileSize.y);
					quad[2].position = sf::Vector2f((i + 1) * tileSize.x , (j + 1) * tileSize.y );
					quad[3].position = sf::Vector2f(i * tileSize.x , (j + 1) * tileSize.y );

					// define its 4 texture coordinates
					quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
					quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
					quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
					quad[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);

					for (int i = 0; i < 4; i++)
					{
						quad[i].color.a = 255;
					}
            }
			loopnumber=0;
			return true;
		}
		else
			return false;

	}

private:

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        // apply the transform
        states.transform *= getTransform();

        // apply the tileset texture
        states.texture = &m_tileset;

        // draw the vertex array
        target.draw(m_vertices, states);
    }

    sf::VertexArray m_vertices;
    sf::Texture m_tileset;
};