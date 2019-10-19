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


int walkDirection = 0;
sf::Texture texture;
sf::Time spriteTime;
AnimatedSprite sprite(16);

void update(sf::Time deltaTime)
{

	sprite.setPosition(sprite.getPosition()+sf::Vector2f(walkDirection*5,0));
	if(walkDirection!=0)
	{
		spriteTime += deltaTime;
		if(walkDirection == 1) sprite.setAnimationNumber(0);
		else sprite.setAnimationNumber(1);
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
	texture.loadFromFile("combined.png");
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
					walkDirection = -1;
				}
				else if (event.key.code == sf::Keyboard::Right)
				{
					walkDirection = 1;
				}
			}
			else if(event.type == sf::Event::KeyReleased)
			{
				if (event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::Right)
				{
					walkDirection = 0;
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
				{
					walkDirection = -1;
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
				{
					walkDirection = 1;
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
