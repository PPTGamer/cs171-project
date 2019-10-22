#include <SFML/Graphics.hpp>

class AnimatedSprite : public sf::Sprite
{
	int animationFrame;
	int animationNumber;
	int maxFrame;
	void refreshFrame()
	{
		setTextureRect(sf::IntRect(64*animationFrame,64*animationNumber,64,64));
	}
public:
	AnimatedSprite(int _maxFrame) : maxFrame(_maxFrame)
	{
		animationFrame = 0;
		animationNumber = 0;
		refreshFrame();
	}
	void setAnimationNumber(int number)
	{
		animationNumber = number;
	}
	void setFrame(int frameNumber)
	{
		animationFrame = frameNumber % maxFrame;
		refreshFrame();
	}
	void advanceFrame(int offset)
	{
		animationFrame = (animationFrame + offset + maxFrame)%maxFrame;
		refreshFrame();
	}
	int getFrame()
	{
		return animationFrame;
	}
};

enum SpriteState
{
	IDLE1, IDLE2, LEFT, RIGHT, DOWN,UP
} spriteState;
sf::Texture texture;
sf::Time spriteTime;
AnimatedSprite sprite(16);
float speed = 5;
void update(sf::Time deltaTime)
{
	// Sprite position
	sf::Vector2f offset;
	switch (spriteState)
	{
		case UP:
			offset.y = -speed;
			break;
		case DOWN:
			offset.y = speed;
			break;
		case LEFT:
			offset.x = -speed;
			break;
		case RIGHT:
			offset.x = speed;
			break;
	}
	sprite.setPosition(sprite.getPosition()+offset);

	// Sprite graphics
	if(spriteState!=IDLE1)
	{
		spriteTime += deltaTime;
		sprite.setAnimationNumber(spriteState);
	}
	else
	{
		spriteTime = sf::Time::Zero;
		if(sprite.getFrame()/4%2==0) sprite.setFrame(sprite.getFrame()/8*8);
		else sprite.setFrame(sprite.getFrame()/8*8+8);
	}
	while(spriteTime >= sf::milliseconds(30))
	{
		sprite.advanceFrame(1);
		spriteTime -= sf::milliseconds(30);
	}
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML works!");
	texture.loadFromFile("images/robotsprite.png");
	sprite.setTexture(texture);
	//sprite.setScale(2.0f,2.0f);
	sprite.setPosition(0,200);

	sf::Clock clock;
	const sf::Time update_ms = sf::seconds(1.f / 60.f);
	sf::Time elapsed = clock.restart();
	sf::Time spriteTime;
	while (window.isOpen())
	{
		//input
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) window.close();
			else if(event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Left)
				{
					spriteState = LEFT;
				}
				else if (event.key.code == sf::Keyboard::Right)
				{
					spriteState = RIGHT;
				}
				else if (event.key.code == sf::Keyboard::Up)
				{
					spriteState = UP;
				}
				else if (event.key.code == sf::Keyboard::Down)
				{
					spriteState = DOWN;
				}
			}
			else if(event.type == sf::Event::KeyReleased)
			{
				if (event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::Right || event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::Down)
				{
					spriteState = IDLE1;
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
				{
					spriteState = LEFT;
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
				{
					spriteState = RIGHT;
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
				{
					spriteState = UP;
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
				{
					spriteState = DOWN;
				}
			}
		}
		//update
		elapsed += clock.restart();
		while(elapsed>=update_ms)
		{
			update(update_ms);
			elapsed -= update_ms;
		}
		//draw
		window.clear();
		window.draw(sprite);
		window.display();
    }

    return 0;
}
