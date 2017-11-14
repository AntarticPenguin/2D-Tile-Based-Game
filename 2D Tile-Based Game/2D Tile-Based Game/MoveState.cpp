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
	_character = character;
	_isMoving = false;
	_movingDuration = 0.0f;
}

void MoveState::Update(float deltaTime)
{
	if (false == _character->IsLive())
		return;

	if (false == _isMoving)
		return;


	_character->UpdateMove(deltaTime);
}

void MoveState::UpdateMove(float deltaTime)
{
	_movingDuration += deltaTime;
}

bool MoveState::IsMoving()
{
	return _isMoving;
}

float MoveState::GetMovingDuration()
{
	return _movingDuration;
}

void MoveState::Start()
{
	if (true == IsMoving())
		return;

	Map* map = (Map*)ComponentSystem::GetInstance().FindComponent(L"tileMap");

	//�浹üũ : Ÿ�� ���� ������Ʈ�� �����ϱ� ���� �浹 ���� üũ
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
	}

	std::list<Component*> collisionList;
	bool canMove = map->GetTileCollisionList(newTileX, newTileY, collisionList);
	if (false == canMove)
	{
		
		//�浹�� ������Ʈ�鳢�� �޼��� ��ȯ
		//�� ���� Ŭ�������� ������ : �浹�� �޼���
		_character->Collision(collisionList);
		return;
	}
	else
	{
		_character->MoveStart(newTileX, newTileY);
		_isMoving = true;
	}
}

void MoveState::Stop()
{
	_movingDuration = 0.0f;
	_isMoving = false;
}

void MoveState::SetMoving(bool isMoving)
{
	_isMoving = isMoving;
}