#include "UI.h"
#include "Sprite.h"

UI::UI(std::wstring UIName, std::wstring fileName, int width, int height) :
	_width(width), _height(height)
{
	_name = UIName;
	_sprite = new Sprite(fileName);
	_sprite->Init(0, 0, width, height, 1.0f);
}

UI::~UI()
{
}

void UI::SetPosition(float posX, float posY)
{
	_posX = posX;
	_posY = posY;

	_sprite->SetPosition(_posX, _posY);

	_collider.left = _posX - (_width / 2);
	_collider.top = _posY - (_height / 2);
	_collider.right = _posX + (_width / 2);
	_collider.bottom = _posY + (_height / 2);
}

void UI::Render()
{
	_sprite->Render();
}

bool UI::CheckCollision(int mouseX, int mouseY)
{
	if (mouseX < _collider.left)
		return false;
	if (_collider.right < mouseX)
		return false;
	if (mouseY < _collider.top)
		return false;
	if (_collider.bottom < mouseY)
		return false;

	WCHAR text[50];
	wsprintf(text, L"%s Å¬¸¯!\n", _name.c_str());
	OutputDebugStringW(text);

	return true;
}

void UI::Action(Character* character)
{

}