#include <fstream>

#include "GameSystem.h"
#include "Map.h"
#include "Sprite.h"
#include "TileCell.h"
#include "TileObject.h"
#include "LifeTileObject.h"

Map::Map(std::wstring name) : Component(name)
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
	//맵용 스프라이트 리스트 작업
	//512 x 512 이미지를 32x32 크기의 타일로 쪼갬
	//16 x 16 = 256개의 타일 생성
	int srcX = 0;
	int srcY = 0;
	_tileSize = 32;
	for (int y = 0; y < 16; y++)
	{
		for (int x = 0; x < 16; x++)
		{
			Sprite* sprite = new Sprite(L"mountain_landscape.png", L"MapSprite.json");
			sprite->Init(srcX, srcY, _tileSize, _tileSize, 1.0f);
			_spriteList.push_back(sprite);
			srcX += _tileSize;
		}

		srcX = 0;
		srcY += _tileSize;
	}

	std::wstring wname = _name;			//16비트를
	std::string name = "";				//8비트로
	name.assign(wname.begin(), wname.end());

	char layer01Name[256];
	sprintf(layer01Name, "%sData_layer1.csv", name.c_str());	//sprintf는 유니코드 못씀

	char layer02Name[256];
	sprintf(layer02Name, "%sData_layer2.csv", name.c_str());

	//Load Map Script 1층
	{
		int line = 0;
		int row = 0;
		char record[1024];
		std::ifstream infile(layer01Name);
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
						TileCell* tileCell = new TileCell(x, row);

						WCHAR componentName[256];
						wsprintf(componentName, L"map_layer_01_%d_%d", line, x);
						TileObject* tileObject = new TileObject(componentName, _spriteList[index], x, row);
						tileObject->SetCanMove(true);

						tileCell->AddComponent(tileObject, true);
						rowList.push_back(tileCell);
						token = strtok(NULL, ",");
					}
					_tileMap.push_back(rowList);
					row++;
				}
				break;
			}
			line++;
		}
	}

	//1층(바닥)을 기반으로 2층을 로드
	{
		char record[1024];
		int line = 0;
		int row = 0;
		std::ifstream infile(layer02Name);
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
						TileCell* tileCell = rowList[x];
						WCHAR componentName[256];
						wsprintf(componentName, L"map_layer_02_%d_%d", line, x);
						if (0 <= index)
						{
							if (100100 == index)
							{
								LifeTileObject* tileObject = new LifeTileObject(componentName, _spriteList[14], x, row);
								tileCell->AddComponent(tileObject, true);
								tileObject->SetCanMove(true);
							}
							else
							{
								TileObject* tileObject = new TileObject(componentName, _spriteList[index], x, row);
								tileCell->AddComponent(tileObject, true);
							}
						}
						token = strtok(NULL, ",");
					}
					row++;
				}
				break;
			}
			line++;
		}
	}
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
	/*
	for (int y = 0; y < _mapHeight; y++)
	{
		for (int x = 0; x < _mapWidth; x++)
		{
			_tileMap[y][x]->MoveDeltaPosition(_deltaX, _deltaY);
			_tileMap[y][x]->Update(deltaTime);
		}
	}

	if (NULL != _viewer)
	{
		float deltaX = _viewer->GetMoveDeltaX() * deltaTime;
		float deltaY = _viewer->GetMoveDeltaY() * deltaTime;
		Scroll(-deltaX, -deltaY);
	}
	*/

	//보간없이
	int midX = GameSystem::GetInstance().GetClientWidth() / 2;
	int midY = GameSystem::GetInstance().GetClientHeight() / 2;

	//뷰어의 위치를 기준으로 시작 픽셀 위치를 계산(startX, startY)
	_startX = (-_viewer->GetTileX() * _tileSize) + midX - _tileSize / 2;
	_startY = (-_viewer->GetTileY() * _tileSize) + midY - _tileSize / 2;

	//해당위치에 타일을 그린다
	float posX = _startX;
	float posY = _startY;
	for (int y = 0; y < _mapHeight; y++)
	{
		for (int x = 0; x < _mapWidth; x++)
		{
			_tileMap[y][x]->SetPosition(posX, posY);
			_tileMap[y][x]->Update(deltaTime);
			posX += _tileSize;
		}
		posX = _startX;
		posY += _tileSize;
	}
}

void Map::Render()
{
	//그릴 영역(타일 개수)
	//최소 최대 x,y를 구함
	//최소 x,y = 뷰어의 현재 타일 x,y의 위치 - (중심축 / 타일사이즈) - 1
	//최대 x,y = 뷰어의 현재 타일 x,y의 위치 + (중심축 / 타일사이즈) + 1(잘리는)
	int midX = GameSystem::GetInstance().GetClientWidth() / 2;
	int midY = GameSystem::GetInstance().GetClientHeight() / 2;

	int minX = _viewer->GetTileX() - (midX / _tileSize) - 3;
	int maxX = _viewer->GetTileX() + (midX / _tileSize) + 3;
	int minY = _viewer->GetTileY() - (midY / _tileSize) - 3;
	int maxY = _viewer->GetTileY() + (midY / _tileSize) + 3;

	//예외처리(범위 밖으로 벗어났을 경우)
	if (minX < 0)
		minX = 0;
	if (_mapWidth <= maxX)
		maxX = _mapWidth;
	if (minY < 0)
		minY = 0;
	if (_mapHeight <= maxY)
		maxY = _mapHeight;

	for (int y = minY; y < maxY; y++)
	{
		for (int x = minX; x < maxX; x++)
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

int Map::GetWidth()
{
	return _mapWidth;
}

int Map::GetHeight()
{
	return _mapHeight;
}

TileCell* Map::GetTileCell(int tileX, int tileY)
{
	return _tileMap[tileY][tileX];
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

bool Map::GetTileCollisionList(int tileX, int tileY, std::list<Component*>& collisionList)
{
	if (tileX < 0)
		return false;
	if (_mapWidth <= tileX)
		return false;
	if (tileY < 0)
		return false;
	if (_mapHeight <= tileY)
		return false;

	return _tileMap[tileY][tileX]->GetCollisionList(collisionList);
}

std::list<Component*> Map::GetTileComponentList(int tileX, int tileY)
{
	return _tileMap[tileY][tileX]->GetComponentList();
}

void Map::InitViewer(Component* component)
{
	_viewer = component;

	int midX = GameSystem::GetInstance().GetClientWidth() / 2;
	int midY = GameSystem::GetInstance().GetClientHeight() / 2;

	//뷰어의 위치를 기준으로 시작 픽셀 위치를 계산(startX, startY)
	_startX = (-_viewer->GetTileX() * _tileSize) + midX - _tileSize / 2;
	_startY = (-_viewer->GetTileY() * _tileSize) + midY - _tileSize / 2;

	//해당위치에 타일을 그린다
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
}