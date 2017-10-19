#include <fstream>
#include "Map.h"
#include "Sprite.h"

Map::Map(LPCWSTR fileName)
{
	_startX = 0.0f;
	_startY = 0.0f;
	_deltaX = 0.0f;
	_deltaY = 0.0f;

	_spriteList.clear();
}

Map::~Map()
{
	for (int y = 0; y < _mapHeight; y++)
	{
		for (int x = 0; x < _mapWidth; x++)
		{
			_tileMap[y][x]->Deinit();
			delete _tileMap[y][x];
		}
	}
}

void Map::Init()
{

	//맵용 스프라이트 리스트 작업
	//512 x 512 이미지를 32x32 크기의 타일로 쪼갬
	//16 x 16 = 256개의 타일 생성
	int srcX = 0;
	int srcY = 0;
	for (int y = 0; y < 16; y++)
	{
		for (int x = 0; x < 16; x++)
		{
			Sprite* sprite = new Sprite(L"MapSprite.png", L"MapSprite.json");
			sprite->Init(srcX, srcY, 32, 32, 1.0f);
			_spriteList.push_back(sprite);
			srcX += 32;
		}

		srcX = 0;
		srcY += 32;
	}

	//Load Map Script
	{
		char record[1000];
		int line = 0;
		std::ifstream infile("MapData.csv");
		while (!infile.eof())
		{
			infile.getline(record, 100);
			
			char* token = strtok(record, ",");
			switch (line)
			{
			case 0:
				if (NULL != token)
				{
					token = strtok(NULL, ",");
					_mapWidth = atoi(token);
					token = strtok(NULL, ",");
					_mapHeight = atoi(token);
				}
				break;
			case 1:
				break;
			default:
				break;
			}
			line++;
		}
	}

	_mapHeight = 16;
	_mapWidth = 16;

	int index = 0;
	for (int y = 0; y < _mapHeight; y++)
	{
		std::vector<Sprite*> rowList;
		for (int x = 0; x < _mapWidth; x++)
		{
			//_tileMap[y][x] = _spriteList[index];
			rowList.push_back(_spriteList[index]);
			index++;
		}
		_tileMap.push_back(rowList);
	}
	/*for (int y = 0; y < MAP_HEIGHT; y++)
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
			_tileMap[y][x] = sprite;
		}
	}*/
}

void Map::Deinit()
{

}

void Map::Update(float deltaTime)
{
	for (int y = 0; y < _mapHeight; y++)
	{
		for (int x = 0; x < _mapWidth; x++)
		{
			_tileMap[y][x]->Update(deltaTime);
		}
	}
}

void Map::Render()
{
	
	//찍는 시작 위치
	_startX += _deltaX;
	_startY += _deltaY;

	//실제로 찍힐 위치
	float posX = _startX;
	float posY = _startY;
	int tileSize = 32;
	for (int y = 0; y < _mapHeight; y++)
	{
		for (int x = 0; x < _mapWidth; x++)
		{
			_tileMap[y][x]->SetPosition(posX, posY);
			_tileMap[y][x]->Render();
			posX += tileSize;
		}
		posX = _startX;
		posY += tileSize;
	}
					
}

void Map::Release() 
{
	for (int y = 0; y < _mapHeight; y++)
	{
		for (int x = 0; x < _mapWidth; x++)
		{
			_tileMap[y][x]->Release();
		}
	}
}

void Map::Reset()
{
	for (int y = 0; y < _mapHeight; y++)
	{
		for (int x = 0; x < _mapWidth; x++)
		{
			_tileMap[y][x]->Reset();
		}
	}
}

void Map::Scroll(float deltaX, float deltaY)
{
	_deltaX = deltaX;
	_deltaY = deltaY;
}