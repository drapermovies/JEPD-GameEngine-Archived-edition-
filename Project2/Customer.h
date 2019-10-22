#pragma once
#include <d3d11.h>
#include <string>
#include <time.h>

#include "GameObject.h"

class Customer : public GameObject
{
public:
	Customer();
	Customer(const Customer&) = default;
	~Customer() = default;

	std::string GetName() { return name; }
private:
	enum GENDER
	{
		GENDER_MALE = 0,
		GENDER_FEMALE = 1
	};

	GENDER gender = GENDER_MALE;

	std::string name = "";

	std::string GenerateName();
	void GenerateGender();
};