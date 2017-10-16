#pragma once
#include <d3dx9.h>
#include <map>
#include "Texture.h"

class ResourceManager
{
private:
	static ResourceManager* _instance;
	std::map<LPCWSTR, Texture*> _textureMap;

public:
	static ResourceManager& GetInstance();
	Texture* LoadTexture(LPCWSTR fileName);

private:
	ResourceManager();

public:
	~ResourceManager();
};
