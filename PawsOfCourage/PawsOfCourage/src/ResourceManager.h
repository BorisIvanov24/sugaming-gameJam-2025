#pragma once
#include "raylib.h"
#include <string>
#include <unordered_map>

class ResourceManager
{
	std::unordered_map<std::string, Texture2D> map;
	Font font;
	std::unordered_map<std::string, Music> mapMusic;

	public:

	void loadMusic(const std::string& musicName, const std::string& filePath);
	const Music& getMusic(const std::string& musicName) const;

	void loadTexture(const std::string& textureName, const std::string& filePath);
	const Texture2D& getTexture(const std::string& textureName) const;

	void setFont(const Font& font);
	const Font& getFont() const;

};

