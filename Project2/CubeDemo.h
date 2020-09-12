#pragma once
#include "Scene.h"

class CubeDemo : public Scene
{
public:
	CubeDemo() = default;
	CubeDemo(TimerClass&, LightShaderClass&, TextureShaderClass&);
	CubeDemo(const CubeDemo&) = default;
	~CubeDemo();

	bool Initialize(float, float) override;
	void Shutdown() override;

	void Update() override;
private:
};