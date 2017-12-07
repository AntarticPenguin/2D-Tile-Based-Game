#pragma once
#include "StageParts.h"

class PathfinderStageParts : public StageParts
{
public:
	PathfinderStageParts(Stage* stage);
	~PathfinderStageParts();

public:
	void CreateComponents(std::wstring mapName);
};