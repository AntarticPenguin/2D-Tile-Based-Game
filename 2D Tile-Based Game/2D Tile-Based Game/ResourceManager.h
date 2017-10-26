#pragma once
#include <Windows.h>
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include "Texture.h"

class ResourceManager
{
private:
	static ResourceManager* _instance;

public:
	static ResourceManager& GetInstance();

	//Texture
private:
	std::map<std::wstring, Texture*> _textureMap;
public:
	Texture* LoadTexture(std::wstring fileName);

	//Script
private:
	std::map<std::wstring, std::vector<std::string>> _scriptMap;
public:
	std::vector<std::string> LoadScript(std::wstring fileName);

private:
	ResourceManager();

public:
	~ResourceManager();
};
