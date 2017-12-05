#pragma once
#include "StageParts.h"

class DefaultStageParts : public StageParts
{
private:

public:
	DefaultStageParts(Stage* stage);
	~DefaultStageParts();

public:
	void CreateComponents(std::wstring mapName);
};