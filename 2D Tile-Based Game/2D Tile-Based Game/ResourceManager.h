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
	std::map<LPCWSTR, Texture*> _textureMap;
public:
	Texture* LoadTexture(LPCWSTR fileName);

	//Script
private:
	std::map<LPCWSTR, std::vector<std::string>> _scriptMap;
public:
	std::vector<std::string> LoadScript(LPCWSTR fileName);

private:
	ResourceManager();

public:
	~ResourceManager();
};
