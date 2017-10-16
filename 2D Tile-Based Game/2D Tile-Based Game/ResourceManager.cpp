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
	if (iter != _textureMap.end())	//������ ���� �ʾҴٸ� ã��
	{
		return iter->second;
	}

	Texture* texture = new Texture();
	texture->Init(fileName);

	_textureMap[fileName] = texture;

	return texture;
}

ResourceManager::ResourceManager()
{

}

ResourceManager::~ResourceManager()
{

}