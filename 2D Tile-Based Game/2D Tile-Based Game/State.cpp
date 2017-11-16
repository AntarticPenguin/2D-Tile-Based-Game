#include "State.h"
#include "Character.h"
#include "Sprite.h"

State::State()
{

}

State::~State()
{
	for (int i = 0; i < _spriteList.size(); i++)
	{
		_spriteList[i]->Deinit();
		delete _spriteList[i];
	}
	_spriteList.clear();
}

void State::Init(Character* character)
{
	_character = character;

	_spriteList.clear();

	WCHAR textureFileName[256];
	wsprintf(textureFileName, L"%s.png", _character->GetTextureFileName().c_str());
	WCHAR scriptFileName[256];

	{
		wsprintf(scriptFileName, L"%s_left.json", _character->GetScriptFileName().c_str());
		Sprite* sprite = new Sprite(textureFileName, scriptFileName);
		sprite->Init();
		_spriteList.push_back(sprite);
	}
	{
		wsprintf(scriptFileName, L"%s_right.json", _character->GetScriptFileName().c_str());
		Sprite* sprite = new Sprite(textureFileName, scriptFileName);
		sprite->Init();
		_spriteList.push_back(sprite);
	}
	{
		wsprintf(scriptFileName, L"%s_up.json", _character->GetScriptFileName().c_str());
		Sprite* sprite = new Sprite(textureFileName, scriptFileName);
		sprite->Init();
		_spriteList.push_back(sprite);
	}
	{
		wsprintf(scriptFileName, L"%s_down.json", _character->GetScriptFileName().c_str());
		Sprite* sprite = new Sprite(textureFileName, scriptFileName);
		sprite->Init();
		_spriteList.push_back(sprite);
	}
}

void State::Update(float deltaTime)
{
	_spriteList[(int)_character->GetDirection()]->Update(deltaTime);
}

void State::Render()
{
	_spriteList[(int)_character->GetDirection()]->SetPosition(_character->GetX(), _character->GetY());
	_spriteList[(int)_character->GetDirection()]->Render();
}

void State::Release()
{
	for (int i = 0; i < _spriteList.size(); i++)
	{
		_spriteList[i]->Release();
	}
}

void State::Reset()
{
	for (int i = 0; i < _spriteList.size(); i++)
	{
		_spriteList[i]->Reset();
	}
}

void State::Start()
{

}

void State::Stop()
{

}