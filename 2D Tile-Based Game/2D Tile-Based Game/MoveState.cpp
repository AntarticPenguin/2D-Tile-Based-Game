#include "MoveState.h"
#include "Character.h"
#include "Map.h"
#include "ComponentSystem.h"

MoveState::MoveState()
{

}

MoveState::~MoveState()
{

}

void MoveState::Init(Character* character)
{
	State::Init(character);
	_movingDuration = 0.0f;
}

void MoveState::Update(float deltaTime)
{
	State::Update(deltaTime);

	if (eStateType::ET_NONE != _nextState)
	{
		_character->ChangeState(_nextState);
		return;
	}

	if (false == _character->IsLive())
		return;

	if (false == _character->IsMoving())
		return;

	if (_character->GetMoveTime() <= _movingDuration)
	{
		_movingDuration = 0.0f;
		_character->MoveStop();
		_nextState = eStateType::ET_IDLE;
	}
	else
	{
		_movingDuration += deltaTime;
		_character->Moving(deltaTime);
	}
}

void MoveState::Start()
{
	State::Start();
	_curState = eStateType::ET_MOVE;

	if (true == _character->IsMoving())
		return;

	Map* map = (Map*)ComponentSystem::GetInstance().FindComponent(L"tileMap");

	//충돌체크 : 타일 위의 컴포넌트를 리셋하기 전에 충돌 여부 체크
	int newTileX = _character->GetTileX();
	int newTileY = _character->GetTileY();
	switch (_character->GetDirection())
	{
	case eDirection::LEFT:	//left
		newTileX--;
		break;
	case eDirection::RIGHT: //right
		newTileX++;
		break;
	case eDirection::UP: //up
		newTileY--;
		break;
	case eDirection::DOWN: //down
		newTileY++;
		break;
	case eDirection::NONE:
		return;
	}

	std::list<Component*> collisionList;
	bool canMove = map->GetTileCollisionList(newTileX, newTileY, collisionList);
	if (false == canMove)
	{
		//충돌된 컴포넌트들끼리 메세지 교환
		//각 하위 클래스에서 재정의 : 충돌시 메세지
		Component* target = _character->Collision(collisionList);
		if (NULL != target && _character->IsAttackCooltime())
		{
			_character->ResetAttackCooltime();
			_character->SetTarget(target);
			_nextState = eStateType::ET_ATTACK;
		}
		else
		{
			_nextState = eStateType::ET_IDLE;
		}
	}
	else
	{
		_character->MoveStart(newTileX, newTileY);
	}
}

void MoveState::Stop()
{
	State::Stop();
}