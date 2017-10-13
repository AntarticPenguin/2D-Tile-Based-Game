#include "GameSystem.h"
#include "Sprite.h"
#include "Texture.h"
#include "Frame.h"


Sprite::Sprite() :
	_curFrame(0), _frameTime(0.0f), _srcTexture(NULL)
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

	
	_srcTexture = new Texture();
	_srcTexture->Init(L"character_sprite.png");
	
	//_srcTexture = ResourceManager::GetInstance().LoadTexture(L"character_sprite.png");

	{
		Frame* frame = new Frame();
		frame->Init(_srcTexture, 32 * 0, 0, 32, 32, 0.2f);
		_frameList.push_back(frame);
	}
	{
		Frame* frame = new Frame();
		frame->Init(_srcTexture, 32 * 2, 0, 32, 32, 0.2f);
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
		_frameList[_curFrame]->Render();
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