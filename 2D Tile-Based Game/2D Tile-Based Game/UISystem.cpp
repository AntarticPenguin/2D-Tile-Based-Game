#include <stdio.h>
#include "UISystem.h"

#include "Sprite.h"

UISystem* UISystem::_instance = NULL;

UISystem::UISystem()
{
	_moveButton = new Sprite(L"menu_MOVE.png");
	_moveButton->Init(0, 0, 32, 32, 1.0f);
	_moveButton->SetPosition(650, 500);
}

UISystem::~UISystem()
{
}

void UISystem::Render()
{
	_moveButton->Render();
}

UISystem& UISystem::GetInstance()
{
	if (NULL == _instance)
		_instance = new UISystem();
	return *_instance;
}

bool UISystem::CheckUIClick(int mouseX, int mouseY)
{
	//�ش�Ǵ� ��ǥ�� UI�� �����ϴ°�?
	if (CheckCollision(mouseX, mouseY))
		return true;
	return false;
}

bool UISystem::CheckCollision(int mouseX, int mouseY)
{
	int tileSize = 32;

	int left = 650 - (tileSize / 2);
	int top = 500 - (tileSize / 2);
	int right = 650 + (tileSize / 2);
	int bottom = 500 + (tileSize / 2);

	if (mouseX < left)
		return false;
	if (right < mouseX)
		return false;
	if (mouseY < top)
		return false;
	if (bottom < mouseY)
		return false;

	return true;
}