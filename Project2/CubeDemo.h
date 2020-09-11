#pragma once
#include "Scene.h"

class CubeDemo : public Scene
{
public:
	CubeDemo() = default;
	CubeDemo(TimerClass&, LightShaderClass&);
	CubeDemo(const CubeDemo&) = default;
	~CubeDemo();

	bool Initialize() override;
	void Shutdown() override;

	void Update() override;
private:
};