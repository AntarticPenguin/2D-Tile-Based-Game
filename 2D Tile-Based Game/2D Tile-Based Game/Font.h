#pragma once
#include <d3dx9.h>
#include <string>

class Font
{
private:
	ID3DXFont* _dxFont;
	D3DCOLOR _color;
	std::wstring _text;

	int _x;
	int _y;
	int _width;
	int _height;

public:
	Font(std::wstring fontStyle, int size, D3DCOLOR color);
	~Font();

public:
	void Render();
	void SetRect(int x, int y, int width, int height);
	void SetText(std::wstring text);
	void SetPosition(int x, int y);
};