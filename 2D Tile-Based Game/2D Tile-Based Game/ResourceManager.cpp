#include <stdio.h>
#include "ResourceManager.h"

ResourceManager* ResourceManager::_instance = NULL;

ResourceManager& ResourceManager::GetInstance()
{
	if (NULL == _instance)
		_instance = new ResourceManager();
	return *_instance;
}

Texture* ResourceManager::LoadTexture(LPCWSTR fileName)
{
	std::map<LPCWSTR, Texture*>::iterator iter = _textureMap.find(fileName);
	if (iter != _textureMap.end())	//끝까지 가지 않았다면 찾음
	{
		return iter->second;
	}

	Texture* texture = new Texture();
	texture->Init(fileName);

	_textureMap[fileName] = texture;

	return texture;
}

std::vector<std::string> ResourceManager::LoadScript(LPCWSTR fileName)
{
	std::map<LPCWSTR, std::vector<std::string>>::iterator iter = _scriptMap.find(fileName);
	if (iter != _scriptMap.end())	//끝까지 가지 않았다면 찾음
	{
		return iter->second;
	}

	char inputRecord[1000];
	std::ifstream infile(fileName);
	std::vector<std::string> textList;

	while (!infile.eof())
	{
		infile.getline(inputRecord, 100);
		textList.push_back(inputRecord);
	}
	_scriptMap[fileName] = textList;

	return textList;
}

ResourceManager::ResourceManager()
{

}

ResourceManager::~ResourceManager()
{

}