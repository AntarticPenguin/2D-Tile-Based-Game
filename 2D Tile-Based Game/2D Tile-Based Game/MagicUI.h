#pragma once
#include "UI.h"

class MagicUI : public UI
{
public:
	MagicUI(std::wstring UIName, std::wstring fileName, int width, int height);
	~MagicUI();

public:
	void Action(Character* character) override;
};