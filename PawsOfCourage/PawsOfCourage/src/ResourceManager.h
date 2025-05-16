#pragma once
#include "raylib.h"
#include <string>
#include <unordered_map>

class ResourceManager
{
	std::unordered_map<std::string, Texture2D> map;

	public:
	void loadTexture(const std::string& textureName, const std::string& filePath);
	const Texture2D& getTexture(const std::string& textureName) const;

};

