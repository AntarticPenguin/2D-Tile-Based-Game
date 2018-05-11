#include "Skill.h"

Skill::Skill(std::wstring name, std::wstring fileName, int skillRange, int attackRange, int behaviorPoint, int damage) :
	_name(name), _fileName(fileName), _skillRange(skillRange), _attackRange(attackRange), _behaviorPoint(behaviorPoint), _damage(damage)
{

}

Skill::~Skill()
{
}

std::wstring Skill::GetName()
{
	return _name;
}

std::wstring Skill::GetFileName()
{
	return _fileName;
}

int Skill::GetSkillRange()
{
	return _skillRange;
}

int Skill::GetAttackRange()
{
	return _attackRange;
}

int Skill::GetSkillDamage()
{
	return _damage;
}