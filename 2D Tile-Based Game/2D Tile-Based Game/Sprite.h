#pragma once
#include <vector>
#include <d3dx9.h>

class Frame;
class Texture;

class Sprite
{
	//DirectX
private:
	LPDIRECT3DDEVICE9 _device3d;
	LPD3DXSPRITE _sprite;

	Texture* _srcTexture;
	std::wstring _textureFileName;
	std::wstring _scriptFileName;

private:
	std::vector<Frame*> _frameList;
	int _curFrame;
	float _frameTime;
	float _x;
	float _y;

	float _rotate;

public:
	Sprite(std::wstring textureFileName, std::wstring scriptFileName, float rotate = 0.0f);
	~Sprite();

public:
	void Init();
	void Init(int srcX, int srcY, int width, int height, float delay);
	void Deinit();

	void Update(float deltaTime);
	void Render();

	void Reset();
	void Release();

	void SetPosition(float x, float y);
};