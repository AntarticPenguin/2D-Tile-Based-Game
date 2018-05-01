#pragma once
#include "State.h"

enum class eUIType;
enum class ePathMode;
class Pathfinding;

class SelectTargetState : public State
{
private:
	Pathfinding* _pathfinder;

public:
	SelectTargetState();
	~SelectTargetState();

public:
	void Init(Character* character);
	void Update(float deltaTime);

	void Start();
	void Stop();

public:
	int GetViewRange();
	ePathMode GetPathMode();
	void SetNextStateByType();
};