#pragma once
#include <string>

class Skill
{
private:
	std::wstring _name;
	std::wstring _fileName;
	int _skillRange;		//��ų�� ����� �� �ִ� ����
	int _attackRange;		//��ų�� ���� ����
	int _damage;
	int _behaviorPoint;

public:
	Skill(std::wstring name, std::wstring fileName, int skillRange, int attackRange, int behaviorPoint, int damage);
	~Skill();

public:
	std::wstring GetName();
	std::wstring GetFileName();
	int GetSkillRange();
	int GetAttackRange();
	int GetSkillDamage();
};