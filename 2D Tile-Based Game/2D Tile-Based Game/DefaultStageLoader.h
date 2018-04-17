#pragma once
#include "StageLoader.h"

class DefaultStageLoader : public StageLoader
{
private:

public:
	DefaultStageLoader(Stage* stage);
	~DefaultStageLoader();

public:
	void CreateComponents(std::wstring mapName);
};