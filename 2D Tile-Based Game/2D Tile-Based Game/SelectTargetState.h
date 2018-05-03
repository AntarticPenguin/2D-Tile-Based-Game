#pragma once
#include "State.h"

enum class eUIType;
enum class ePathMode;
class Pathfinding;
class TileCell;

class SelectTargetState : public State
{
private:
	Pathfinding* _pathfinder;
	Pathfinding* _skillViewer;
	TileCell* _mouseOverCell;
	TileCell* _prevOverCell;

public:
	SelectTargetState();
	~SelectTargetState();

public:
	void Init(Character* character);
	void Update(float deltaTime);

	void Start();
	void Stop();

	void CancelUI(bool rightDown);

public:
	void ShowBaseRange();
	int GetViewRange();
	ePathMode GetPathMode();

	void SetNextStateByType();
	void DoMoveFunction();
	void DoAttackFunction();
	void DoMagicFunction();
};