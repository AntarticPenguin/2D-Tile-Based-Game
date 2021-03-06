#include <fstream>
#include <reader.h>	//JSON PARSING

#include "Sprite.h"

#include "GameSystem.h"
#include "ResourceManager.h"
#include "Frame.h"

Sprite::Sprite(std::wstring textureFileName) :
	_textureFileName(textureFileName)
{
	_rotate = 0.0f;
}

Sprite::Sprite(std::wstring textureFileName, std::wstring scriptFileName, float rotate) :
	_textureFileName(textureFileName), _scriptFileName(scriptFileName), _curFrame(0), _frameTime(0.0f), _srcTexture(NULL), _rotate(rotate)
{

}

Sprite::~Sprite()
{
	/*if (NULL != _srcTexture)
	{
		_srcTexture->Deinit();
		delete _srcTexture;
		_srcTexture = NULL;
	}*/
	_srcTexture = NULL;
}

void Sprite::Init()
{
	_device3d = GameSystem::GetInstance().GetDevice();
	_sprite = GameSystem::GetInstance().GetSprite();

	_srcTexture = ResourceManager::GetInstance().LoadTexture(_textureFileName);

	//JSON PARSING
	{
		//파일을 읽어, 텍스트 정보를 파싱
	
		std::vector<std::string> scriptTextList = ResourceManager::GetInstance().LoadScript(_scriptFileName);
		for (size_t i = 0; i < scriptTextList.size(); i++)
		{
			std::string record = scriptTextList[i];
			
			Json::Value root;		//key, value 구조는 Tree로 구성되어있다
			Json::Reader reader;
			bool isSuccess = reader.parse(record, root);
			if (isSuccess)
			{
				std::string texture = root["texture"].asString();
				int x = root["x"].asInt();
				int y = root["y"].asInt();
				int width = root["width"].asInt();
				int height = root["height"].asInt();
				double framedelay = root["framedelay"].asDouble();

				Frame* frame = new Frame();
				frame->Init(_srcTexture, x, y, width, height, _rotate, framedelay);
				_frameList.push_back(frame);
			}
		}
	}

	_curFrame = 0;
	_frameTime = 0.0f;
}

void Sprite::Init(int srcX, int srcY, int width, int height, float delay)
{
	_device3d = GameSystem::GetInstance().GetDevice();
	_sprite = GameSystem::GetInstance().GetSprite();

	_srcTexture = ResourceManager::GetInstance().LoadTexture(_textureFileName);

	{
		Frame* frame = new Frame();
		frame->Init(_srcTexture, srcX, srcY, width, height, _rotate, delay);
		_frameList.push_back(frame);
	}

	_curFrame = 0;
	_frameTime = 0.0f;
}

void Sprite::Deinit()
{
	for (std::vector<Frame*>::iterator iter = _frameList.begin(); iter != _frameList.end(); iter++)
	{
		Frame* frame = (*iter);
		frame->Deinit();
		delete frame;
	}
	_frameList.clear();

	//_srcTexture->Deinit();
	_srcTexture = NULL;
}

void Sprite::Update(float deltaTime)
{
	_frameTime += deltaTime;

	if (_frameList[_curFrame]->GetFrameDelay() <= _frameTime)
	{
		_frameTime = 0.0f;
		_curFrame = (_curFrame + 1) % _frameList.size();
	}
}

void Sprite::Render()
{
	if (_curFrame < _frameList.size())
	{
		_frameList[_curFrame]->SetPosition(_x, _y);
		_frameList[_curFrame]->Render();
	}
}

void Sprite::Reset()
{
	Init();
	for (std::vector<Frame*>::iterator iter = _frameList.begin(); iter != _frameList.end(); iter++)
	{
		Frame* frame = (*iter);
		frame->Reset();
	}
}

void Sprite::Release()
{
	for (std::vector<Frame*>::iterator iter = _frameList.begin(); iter != _frameList.end(); iter++)
	{
		Frame* frame = (*iter);
		frame->Release();
	}

	_srcTexture->Release();
}

void Sprite::SetPosition(float x, float y)
{
	_x = x;
	_y = y;
}

void Sprite::SetColor(D3DCOLOR color)
{
	for (size_t i = 0; i < _frameList.size(); i++)
		_frameList[i]->SetColor(color);
}