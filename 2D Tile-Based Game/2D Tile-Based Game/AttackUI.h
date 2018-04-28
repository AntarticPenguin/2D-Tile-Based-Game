#pragma once
#include "UI.h"

class AttackUI : public UI
{
public:
	AttackUI(std::wstring UIName, std::wstring fileName, int width, int height);
	~AttackUI();

public:
	void Action(Character* character) override;
};