#include "ResourceManager.h"

void ResourceManager::loadTexture(const std::string& textureName, const std::string& filePath)
{
	map[textureName] = LoadTexture(filePath.c_str());
}

const Texture2D& ResourceManager::getTexture(const std::string& textureName) const
{
	return map.at(textureName);
}
