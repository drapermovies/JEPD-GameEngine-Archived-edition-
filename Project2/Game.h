#pragma once

class Game
{
public:
	Game() = default;
	Game(const Game&) = default;
	~Game() = default;

	void Render();
	void Update();
private:
	int money = 2000;
	unsigned int day = 0;
	float current_time = 0;
};