#include <fstream>

#include "GameSystem.h"
#include "Map.h"
#include "Sprite.h"
#include "TileCell.h"
#include "TileObject.h"

Map::Map(LPCWSTR name) : Component(name)
{
	_startX = 0.0f;
	_startY = 0.0f;
	_deltaX = 0.0f;
	_deltaY = 0.0f;

	_spriteList.clear();
}

Map::~Map()
{

}

void Map::Init()
{
	//¸Ê¿ë ½ºÇÁ¶óÀÌÆ® ¸®½ºÆ® ÀÛ¾÷
	//512 x 512 ÀÌ¹ÌÁö¸¦ 32x32 Å©±âÀÇ Å¸ÀÏ·Î ÂÉ°·
	//16 x 16 = 256°³ÀÇ Å¸ÀÏ »ý¼º
	int srcX = 0;
	int srcY = 0;
	_tileSize = 32;
	for (int y = 0; y < 16; y++)
	{
		for (int x = 0; x < 16; x++)
		{
			Sprite* sprite = new Sprite(L"MapSprite.png", L"MapSprite.json");
			sprite->Init(srcX, srcY, _tileSize, _tileSize, 1.0f);
			_spriteList.push_back(sprite);
			srcX += _tileSize;
		}

		srcX = 0;
		srcY += _tileSize;
	}

	//Load Map Script 1Ãþ
	{
		char record[1024];
		int line = 0;
		std::ifstream infile("MapData_layer1.csv");
		while (!infile.eof())
		{
			infile.getline(record, 1024);
			
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
				//Map Data
				if(NULL != token)
				{
					std::vector<TileCell*> rowList;
					for (int x = 0; x < _mapWidth; x++)
					{
						int index = atoi(token);
						TileCell* tileCell = new TileCell();

						WCHAR componentName[256];
						wsprintf(componentName, L"map_layer_01_%d_%d", line, x);
						TileObject* tileObject = new TileObject(componentName, _spriteList[index]);

						tileCell->AddComponent(tileObject, true);
						rowList.push_back(tileCell);
						token = strtok(NULL, ",");
					}
					_tileMap.push_back(rowList);
				}
				break;
			}
			line++;
		}
	}

	//1Ãþ(¹Ù´Ú)À» ±â¹ÝÀ¸·Î 2ÃþÀ» ·Îµå
	{
		char record[1024];
		int line = 0;
		int row = 0;
		std::ifstream infile("MapData_layer2.csv");
		while (!infile.eof())
		{
			infile.getline(record, 1024);

			char* token = strtok(record, ",");
			switch (line)
			{
			case 0:
				break;
			case 1:
				break;
			default:
				//Map Data
				if (NULL != token)
				{
					std::vector<TileCell*> rowList = _tileMap[row];
					for (int x = 0; x < _mapWidth; x++)
					{
						int index = atoi(token);
						if (0 <= index)
						{
							TileCell* tileCell = rowList[x];

							WCHAR componentName[256];
							wsprintf(componentName, L"map_layer_02_%d_%d", line, x);
							TileObject* tileObject = new TileObject(componentName, _spriteList[index]);
							tileObject->SetCanMove(false);

							tileCell->AddComponent(tileObject, true);
						}
						token = strtok(NULL, ",");
					}
					//_tileMap.push_back(rowList);
					row++;
				}
				break;
			}
			line++;
		}
	}

	/*
	//Âï´Â ½ÃÀÛ À§Ä¡
	_startX += _deltaX;
	_startY += _deltaY;

	//½ÇÁ¦·Î ÂïÈú À§Ä¡
	float posX = _startX;
	float posY = _startY;
	for (int y = 0; y < _mapHeight; y++)
	{
		for (int x = 0; x < _mapWidth; x++)
		{
			_tileMap[y][x]->SetPosition(posX, posY);
			posX += _tileSize;
		}
		posX = _startX;
		posY += _tileSize;
	}
	*/
}

void Map::Deinit()
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

void Map::Update(float deltaTime)
{
	for (int y = 0; y < _mapHeight; y++)
	{
		for (int x = 0; x < _mapWidth; x++)
		{
			_tileMap[y][x]->MoveDeltaPosition(_deltaX, _deltaY);
			_tileMap[y][x]->Update(deltaTime);
		}
	}
}

void Map::Render()
{
	for (int y = 0; y < _mapHeight; y++)
	{
		for (int x = 0; x < _mapWidth; x++)
		{
			_tileMap[y][x]->Render();
		}
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

int Map::GetPositionX(int tileX, int tileY)
{
	return _tileMap[tileY][tileX]->GetPositionX();
}

int Map::GetPositionY(int tileX, int tileY)
{
	return _tileMap[tileY][tileX]->GetPositionY();
}

void Map::SetTileComponent(int tileX, int tileY, Component* component, bool isRender)
{
	_tileMap[tileY][tileX]->AddComponent(component, isRender);
}

void Map::ResetTileComponent(int tileX, int tileY, Component* component)
{
	_tileMap[tileY][tileX]->RemoveComponent(component);
}

bool Map::CanMoveTileMap(int tileX, int tileY)
{
	if (tileX < 0)
		return false;
	if (_mapWidth <= tileX)
		return false;
	if (tileY < 0)
		return false;
	if (_mapHeight <= tileY)
		return false;

	return _tileMap[tileY][tileX]->CanMove();
}

void Map::InitViewer(Component* component)
{
	Component* _viewer = component;

	//±×¸± ¿µ¿ª(Å¸ÀÏ °³¼ö)
	//ÃÖ¼Ò ÃÖ´ë x,y¸¦ ±¸ÇÔ
	//ÃÖ¼Ò x,y = ºä¾îÀÇ ÇöÀç Å¸ÀÏ x,yÀÇ À§Ä¡ - (Áß½ÉÃà / Å¸ÀÏ»çÀÌÁî) - 1
	//ÃÖ´ë x,y = ºä¾îÀÇ ÇöÀç Å¸ÀÏ x,yÀÇ À§Ä¡ + (Áß½ÉÃà / Å¸ÀÏ»çÀÌÁî) + 1
	int midX = GameSystem::GetInstance().GetClientWidth() / 2;
	int midY = GameSystem::GetInstance().GetClientHeight() / 2;

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> parent of 09a4751... 171103 ì¹´ë©”ë¼ì´ë™/ë¦¬íŒ©í† ë§
	int minX = _viewer->GetTileX() - (midX / _tileSize) - 1;
	int maxX = _viewer->GetTileX() + (midX / _tileSize) + 1;
	int minY = _viewer->GetTileY() - (midY / _tileSize) - 1;
	int maxY = _viewer->GetTileY() + (midY / _tileSize) + 1;

	//¿¹¿ÜÃ³¸®(¹üÀ§ ¹ÛÀ¸·Î ¹þ¾î³µÀ» °æ¿ì)
	if (minX < 0)
		minX = 0;
	if (_mapWidth <= maxX)
		maxX = _mapWidth - 1;
	if (minY < 0)
		minY = 0;
	if (_mapHeight <= maxY)
		maxY = _mapHeight - 1;
<<<<<<< HEAD
	
>>>>>>> bcf162d76dc561bd9a88489097c6f51d5c4b566b
=======

>>>>>>> parent of 09a4751... 171103 ì¹´ë©”ë¼ì´ë™/ë¦¬íŒ©í† ë§
	//ºä¾îÀÇ À§Ä¡¸¦ ±âÁØÀ¸·Î ½ÃÀÛ ÇÈ¼¿ À§Ä¡¸¦ °è»ê(startX, startY)
	_startX = (-_viewer->GetTileX() * _tileSize) + midX - _tileSize / 2;
	_startY = (-_viewer->GetTileY() * _tileSize) + midY - _tileSize / 2;

	//ÇØ´çÀ§Ä¡¿¡ Å¸ÀÏÀ» ±×¸°´Ù
	float posX = _startX;
	float posY = _startY;
	for (int y = 0; y < _mapHeight; y++)
	{
		for (int x = 0; x < _mapWidth; x++)
		{
			_tileMap[y][x]->SetPosition(posX, posY);
			posX += _tileSize;
		}
		posX = _startX;
		posY += _tileSize;
	}
	
	
	/*
	//Âï´Â ½ÃÀÛ À§Ä¡
	_startX += _deltaX;
	_startY += _deltaY;

	//½ÇÁ¦·Î ÂïÈú À§Ä¡
	float posX = _startX;
	float posY = _startY;
	for (int y = 0; y < _mapHeight; y++)
	{
		for (int x = 0; x < _mapWidth; x++)
		{
			_tileMap[y][x]->SetPosition(posX, posY);
			posX += _tileSize;
		}
		posX = _startX;
		posY += _tileSize;
	}
	*/
}