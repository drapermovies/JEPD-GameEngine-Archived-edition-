#include "Customer.h"

Customer::Customer()
{
	std::srand(time(NULL));
	GenerateGender();
	name = GenerateName();
}

std::string Customer::GenerateName()
{
	std::string path = "../x64/Debug/data/Names/";
	std::string temp_path = path;

	std::string new_name = "";

	if (gender == GENDER_MALE)
	{
		temp_path += "maleNames.txt";
	}
	else
	{
		temp_path += "femaleNames.txt";
	}

	std::ifstream fin;
	std::string line = "";
	char input = ' ';

	fin.open(temp_path);

	if (fin.fail())
	{
		return " ";
	}

	int num_lines = 0;
	int current_line = 0;
	while (std::getline(fin, line))
	{
		num_lines++;
	}
	fin.close();
	fin.open(temp_path);

	int random_line = std::rand() % num_lines;
	while (std::getline(fin, line))
	{
		++current_line;
		if (current_line == random_line)
		{
			new_name += line + " ";
		}
	}
	fin.close();

	path += "surnames.txt";

	fin.open(path);
	if (fin.fail())
	{
		return " ";
	}

	current_line = 0;
	num_lines = 0;
	while (std::getline(fin, line))
	{
		num_lines++;
	}
	fin.close();
	fin.open(path);

	random_line = std::rand() % num_lines;

	while (std::getline(fin, line))
	{
		++current_line;
		if (current_line == random_line)
		{
			new_name += line;
		}
	}

	return new_name;
}

void Customer::GenerateGender()
{
	int num = std::rand() % 2;
	if (num == 0)
	{
		gender = GENDER_MALE;
	}
	else
	{
		gender = GENDER_FEMALE;
	}
}
