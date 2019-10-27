#pragma once
#include "Scene.h"

class CubeDemo : public Scene
{
public:
	CubeDemo() = default;
	CubeDemo(const CubeDemo&) = default;
	~CubeDemo() = default;

	void Initialize() override;
	void Shutdown() override;

	void Update() override;
	bool Render() override;
private:
};