#include "Skill.h"

Skill::Skill(std::wstring name, std::wstring fileName) :
	_name(name), _fileName(fileName)
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