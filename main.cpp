#include <SFML/Graphics.hpp>
#include "Game.h"

int main()
{
	Game* game = new Game();

	// create an 800x600 window, respect DPI settings.
	sf::RenderWindow window(sf::VideoMode(800, 600, sf::VideoMode::getDesktopMode().bitsPerPixel), "SFML works!");
	window.setFramerateLimit(60);
	
	sf::Clock clock;
	const sf::Time update_ms = sf::seconds(1.f / 60.f);
	sf::Time elapsed = clock.restart();
	sf::Time spriteTime;
	while (window.isOpen())
	{
		//input
		game->handleInput(window);

		//update
		elapsed += clock.restart();
		while(elapsed>=update_ms)
		{
			game->update(update_ms);
			elapsed -= update_ms;
		}
		
		//draw
		window.clear(sf::Color(0,0,50,255));
		game->draw(window);
		window.display();
    }
	delete game;
    return 0;
}
