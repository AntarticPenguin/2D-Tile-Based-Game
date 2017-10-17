#include <fstream>
#include <reader.h>	//JSON PARSING

#include "GameSystem.h"
#include "ResourceManager.h"

#include "Sprite.h"
#include "Frame.h"


Sprite::Sprite(LPCWSTR textureFileName, LPCWSTR scriptFileName) :
	_textureFileName(textureFileName), _scriptFileName(scriptFileName), _curFrame(0), _frameTime(0.0f), _srcTexture(NULL)
{

}

Sprite::~Sprite()
{
	if (NULL != _srcTexture)
	{
		_srcTexture->Deinit();
		delete _srcTexture;
		_srcTexture = NULL;
	}
}

void Sprite::Init()
{
	_device3d = GameSystem::GetInstance().GetDevice();
	_sprite = GameSystem::GetInstance().GetSprite();

	_srcTexture = ResourceManager::GetInstance().LoadTexture(_textureFileName);

	//JSON TEST
	{
		//파일을 읽어, 텍스트 정보를 파싱
	
		char inputBuffer[1000];
		std::ifstream infile(_scriptFileName);

		while (!infile.eof())
		{
			infile.getline(inputBuffer, 100);
			
			Json::Value root;		//key, value 구조는 Tree로 구성되어있다
			Json::Reader reader;
			bool isSuccess = reader.parse(inputBuffer, root);
			if (isSuccess)
			{
				std::string texture = root["texture"].asString();
				int x = root["x"].asInt();
				int y = root["y"].asInt();
				int width = root["width"].asInt();
				int height = root["height"].asInt();
				double framedelay = root["framedelay"].asDouble();

				Frame* frame = new Frame();
				frame->Init(_srcTexture, x, y, width, height, framedelay);
				_frameList.push_back(frame);
			}
		}
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

	_srcTexture->Deinit();
}

void Sprite::Update(float deltaTime)
{
	/*
		누적된 시간이 프레임 딜레이를 넘어가면
			- 다음 프레임
			- 누적된 시간 리셋
	*/
	_frameTime += deltaTime;

	if (_frameList[_curFrame]->GetFrameDelay() <= _frameTime)
	{
		_frameTime = 0;
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