#pragma once
#include "Scene.h"
#include "TextObject.h"

class CubeDemo : public Scene
{
public:
	CubeDemo() = default;
	CubeDemo(TimerClass&, ShaderManager&);
	CubeDemo(const CubeDemo&) = default;
	~CubeDemo();

	bool Initialize(float, float) override;
	void Shutdown() override;

	void Update(float) override;
};