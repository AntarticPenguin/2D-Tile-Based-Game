#include "SelectTargetState.h"
#include "Character.h"

SelectTargetState::SelectTargetState()
{
}

SelectTargetState::~SelectTargetState()
{
}

void SelectTargetState::Init(Character* character)
{
	State::Init(character);
}

void SelectTargetState::Update(float deltaTime)
{
	State::Update(deltaTime);

	//���콺 �����Ǵ� ���� ��ĥ�Ͽ� ǥ��
	TileCell* MouseOverCell;
}


void SelectTargetState::Start()
{
	State::Start();

	if (eStateType::ET_NONE != _nextState)
	{
		_character->ChangeState(_nextState);
		return;
	}

	_curState = eStateType::ET_SELECT_TARGET;
}

void SelectTargetState::Stop()
{
	State::Stop();
}