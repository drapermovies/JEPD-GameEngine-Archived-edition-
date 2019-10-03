#include "inputclass.h"

void InputClass::Initialize()
{
	for (int i = 0; i < 256; i++)
	{
		m_keys[i] = false;
	}

	return;
}

void InputClass::KeyDown(unsigned int i)
{
	m_keys[i] = true;
	return;
}

void InputClass::KeyUp(unsigned int i)
{
	m_keys[i] = false;
	return;
}

bool InputClass::IsKeyDown(unsigned int i)
{
	return m_keys[i];
}
