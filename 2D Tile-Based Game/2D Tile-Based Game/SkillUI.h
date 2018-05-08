#pragma once
#include "UI.h"

class SkillUI : public UI
{
public:
	SkillUI(std::wstring UIName, std::wstring fileName, int width, int height);
	~SkillUI();

public:
	void Action(Character* character) override;
};