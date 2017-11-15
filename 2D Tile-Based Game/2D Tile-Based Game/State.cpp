#include "State.h"
#include "Character.h"

State::State()
{

}

State::~State()
{

}

void State::Init(Character* character)
{
	_character = character;
}

void State::Update(float deltaTime)
{

}

void State::Start()
{

}

void State::Stop()
{

}