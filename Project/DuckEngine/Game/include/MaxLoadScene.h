#pragma once

#include "Scene.h"


class MaxLoadScene : public Scene
{
public:
	void Load() override;
	void Start() override;
	void Update() override;
	void PostUpdate() override;
	void Exit() override;
	void Unload() override;

private:
	void SpawnSquare();
	void CheckAndSpawnSquare(float spawnInterval);
	void UpdateSquares(float deltaTime);

};