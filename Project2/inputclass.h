#pragma once

class InputClass
{
public:
	InputClass() = default;
	InputClass(const InputClass&) = default;
	~InputClass() = default;

	void Initialize();

	void KeyDown(unsigned int);
	void KeyUp(unsigned int);

	bool IsKeyDown(unsigned int);

private:
	bool m_keys[256];
};

