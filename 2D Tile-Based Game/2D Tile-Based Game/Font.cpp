#include "GameSystem.h"
#include "Font.h"

Font::Font(std::wstring fontStyle, int size, D3DCOLOR color)
{
	_color = color;

	HRESULT hr = D3DXCreateFont(GameSystem::GetInstance().GetDevice(), size, 0, FW_BOLD, 1, false, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, fontStyle.c_str(), &_dxFont);

	if (FAILED(hr))
	{
		MessageBox(0, L"폰트 생성 실패", L"ERROR", MB_OK);
	}
}

Font::~Font()
{
	RELEASE_COM(_dxFont);
}

void Font::Render()
{
	RECT rect;
	rect.left = _x;
	rect.top = _y;
	rect.right = _x + _width;
	rect.bottom = _y + _height;

	D3DXMATRIX matrix;
	D3DXMatrixTransformation2D(&matrix, NULL, 0.0f, NULL, NULL, 0.0f, NULL);
	GameSystem::GetInstance().GetSprite()->SetTransform(&matrix);
	_dxFont->DrawTextW(GameSystem::GetInstance().GetSprite(), _text.c_str(), -1, &rect, DT_CENTER, _color);
}

void Font::SetRect(int x, int y, int width, int height)
{
	_x = x;
	_y = y;
	_width = width;
	_height = height;
}

void Font::SetText(std::wstring text)
{
	_text = text;
}

void Font::SetPosition(int x, int y)
{
	_x = x;
	_y = y;
}