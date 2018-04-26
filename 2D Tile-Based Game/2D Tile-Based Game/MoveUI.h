#pragma once
#include "UI.h"

class MoveUI : public UI
{
public:
	MoveUI(std::wstring UIName, std::wstring fileName, int width, int height);
	~MoveUI();

public:
	void Action(Character* character) override;
};