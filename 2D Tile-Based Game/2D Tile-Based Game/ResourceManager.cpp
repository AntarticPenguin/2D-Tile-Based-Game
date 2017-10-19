#include <stdio.h>
#include "ResourceManager.h"
using namespace std;

ResourceManager* ResourceManager::_instance = NULL;

ResourceManager& ResourceManager::GetInstance()
{
	if (NULL == _instance)
		_instance = new ResourceManager();
	return *_instance;
}

Texture* ResourceManager::LoadTexture(LPCWSTR fileName)
{
	map<LPCWSTR, Texture*>::iterator iter = _textureMap.find(fileName);
	if (iter != _textureMap.end())	//������ ���� �ʾҴٸ� ã��
	{
		return iter->second;
	}

	Texture* texture = new Texture();
	texture->Init(fileName);

	_textureMap[fileName] = texture;

	return texture;
}

vector<string> ResourceManager::LoadScript(LPCWSTR fileName)
{
	map<LPCWSTR, vector<string>>::iterator iter = _scriptMap.find(fileName);
	if (iter != _scriptMap.end())	//������ ���� �ʾҴٸ� ã��
	{
		return iter->second;
	}

	char inputRecord[1000];
	ifstream infile(fileName);
	vector<string> textList;

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