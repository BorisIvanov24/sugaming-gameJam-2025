#include "ResourceManager.h"

void ResourceManager::loadMusic(const std::string& musicName, const std::string& filePath)
{
	mapMusic[musicName] = LoadMusicStream(filePath.c_str());
}

const Music& ResourceManager::getMusic(const std::string& musicName) const
{
	return mapMusic.at(musicName);
}

void ResourceManager::loadTexture(const std::string& textureName, const std::string& filePath)
{
	map[textureName] = LoadTexture(filePath.c_str());
}

const Texture2D& ResourceManager::getTexture(const std::string& textureName) const
{
	return map.at(textureName);
}

void ResourceManager::setFont(const Font& font)
{
	this->font = font;
}

const Font& ResourceManager::getFont() const
{
	return font;
}
