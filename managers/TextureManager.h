
#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H
#include<iostream>
#include<unordered_map>
#include<utility>
#include<memory>
#include<string>
#include<SFML/Graphics.hpp>

class TextureManager
{
public:
	void loadTexture(std::string filename);
	sf::Image* getImage(std::string filename);
	sf::Texture* getTexture(std::string filename);
private:
	static const std::string imageDirectory;
	std::unordered_map<std::string,std::pair<std::unique_ptr<sf::Image>,std::unique_ptr<sf::Texture>>> m_lookupTable;
};

#endif
