#include <SFML/Graphics.hpp>
#include "Game.h"

int main()
{
	srand(time(NULL)); // required since C++11 <random> does not work properly on some versions of MinGW GCC
	// create an 800x600 window, respect DPI settings.
	sf::RenderWindow window(sf::VideoMode(800, 600, sf::VideoMode::getDesktopMode().bitsPerPixel), "SFML works!");
	window.setFramerateLimit(60);
	
	Game* game = new Game(window);

	sf::Clock clock;
	const sf::Time update_ms = sf::seconds(1.f / 60.f);
	sf::Time elapsed = clock.restart();
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
		window.clear(sf::Color::Black);
		game->draw(window);
		window.display();
    }
	delete game;
    return 0;
}
