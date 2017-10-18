#include "Map.h"
#include "Sprite.h"

Map::Map(LPCWSTR fileName)
{
	_startX = 0.0f;
	_startY = 0.0f;
	_deltaX = 0.0f;
	_deltaY = 0.0f;
}

Map::~Map()
{
	for (int y = 0; y < MAP_HEIGHT; y++)
	{
		for (int x = 0; x < MAP_WIDTH; x++)
		{
			_testTileMap[y][x]->Deinit();
			delete _testTileMap[y][x];
		}
	}
}

void Map::Init()
{
	for (int y = 0; y < MAP_HEIGHT; y++)
	{
		for (int x = 0; x < MAP_WIDTH; x++)
		{
			Sprite* sprite;
			int randValue = rand() % 4;
			switch (randValue)
			{
			case 0:
				sprite = new Sprite(L"character_sprite.png", L"character_sprite01.json");
				break;
			case 1:
				sprite = new Sprite(L"character_sprite.png", L"character_sprite02.json");
				break;
			case 2:
				sprite = new Sprite(L"character_sprite.png", L"character_sprite03.json");
				break;
			case 3:
				sprite = new Sprite(L"character_sprite.png", L"character_sprite04.json");
				break;
			}
			sprite->Init();
			_testTileMap[y][x] = sprite;
		}
	}
}

void Map::Deinit()
{

}

void Map::Update(float deltaTime)
{
	for (int y = 0; y < MAP_HEIGHT; y++)
	{
		for (int x = 0; x < MAP_WIDTH; x++)
		{
			_testTileMap[y][x]->Update(deltaTime);
		}
	}
}

void Map::Render()
{
	
	//Âï´Â ½ÃÀÛ À§Ä¡
	_startX += _deltaX;
	_startY += _deltaY;

	//½ÇÁ¦·Î ÂïÈú À§Ä¡
	float posX = _startX;
	float posY = _startY;
	int tileSize = 32;
	for (int y = 0; y < MAP_HEIGHT; y++)
	{
		for (int x = 0; x < MAP_WIDTH; x++)
		{
			_testTileMap[y][x]->SetPosition(posX, posY);
			_testTileMap[y][x]->Render();
			posX += 32;
		}
		posX = _startX;
		posY += tileSize;
	}
					
}

void Map::Release() 
{
	for (int y = 0; y < MAP_HEIGHT; y++)
	{
		for (int x = 0; x < MAP_WIDTH; x++)
		{
			_testTileMap[y][x]->Release();
		}
	}
}

void Map::Reset()
{
	for (int y = 0; y < MAP_HEIGHT; y++)
	{
		for (int x = 0; x < MAP_WIDTH; x++)
		{
			_testTileMap[y][x]->Reset();
		}
	}
}

void Map::Scroll(float deltaX, float deltaY)
{
	_deltaX = deltaX;
	_deltaY = deltaY;
}