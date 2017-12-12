#pragma once
#include "StageParts.h"

class LifeStageParts : public StageParts
{
public:
	LifeStageParts(Stage* stage);
	~LifeStageParts();

public:
	void CreateComponents(std::wstring mapName);
};