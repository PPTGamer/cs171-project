#include "TextureManager.h"
const std::string TextureManager::imageDirectory = "images/";
void TextureManager::loadTexture(std::string filename)
{
	m_lookupTable[filename] = std::make_pair(
		std::make_unique<sf::Image>(),
		std::make_unique<sf::Texture>()
	);
	
	if(!m_lookupTable[filename].first->loadFromFile(TextureManager::imageDirectory + filename))
	{
		m_lookupTable.erase(filename);
		std::cout<<"Image \""<<filename<<"\" failed to load."<<std::endl;
		return;
	}
	if(!m_lookupTable[filename].second->loadFromImage(*m_lookupTable[filename].first))
	{
		m_lookupTable.erase(filename);
		std::cout<<"Texture \""<<filename<<"\" failed to load."<<std::endl;
		return;
	}
	std::cout<<"Texture \""<<filename<<"\" loaded."<<std::endl;
}

sf::Image* TextureManager::getImage(std::string filename)
{
	if(m_lookupTable.find(filename)==m_lookupTable.end())
	{
		std::cout<<"Image \""<<filename<<"\" cannot be retrieved; it was not previously loaded."<<std::endl;
		return nullptr;
	}
	return m_lookupTable[filename].first.get();
}

sf::Texture* TextureManager::getTexture(std::string filename)
{
	if(m_lookupTable.find(filename)==m_lookupTable.end())
	{
		std::cout<<"Texture \""<<filename<<"\" cannot be retrieved; it was not previously loaded."<<std::endl;
		return nullptr;
	}
	return m_lookupTable[filename].second.get();
}
