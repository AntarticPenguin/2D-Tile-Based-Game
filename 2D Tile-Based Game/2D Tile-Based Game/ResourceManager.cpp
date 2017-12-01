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

Texture* ResourceManager::LoadTexture(wstring fileName)
{
	map<wstring, Texture*>::iterator itr = _textureMap.find(fileName);
	if (itr != _textureMap.end())	//������ ���� �ʾҴٸ� ã��
	{
		return itr->second;
	}

	Texture* texture = new Texture();
	texture->Init(fileName.c_str());

	_textureMap[fileName] = texture;

	return texture;
}

vector<string> ResourceManager::LoadScript(wstring fileName)
{
	map<wstring, vector<string>>::iterator itr = _scriptMap.find(fileName);
	if (itr != _scriptMap.end())	//������ ���� �ʾҴٸ� ã��
	{
		return itr->second;
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