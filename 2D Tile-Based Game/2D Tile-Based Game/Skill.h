#pragma once
#include <string>

class Skill
{
private:
	std::wstring _name;
	std::wstring _fileName;

public:
	Skill(std::wstring name, std::wstring fileName);
	~Skill();

public:
	std::wstring GetName();
	std::wstring GetFileName();
};