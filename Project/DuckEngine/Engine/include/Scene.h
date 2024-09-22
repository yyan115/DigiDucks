#pragma once

class Scene {
public:
	virtual void Load() = 0;
	virtual void Start() = 0;
	virtual void Update() = 0;
	virtual void PostUpdate() = 0;
	virtual void Exit() = 0;
	virtual void Unload() = 0;

    virtual ~Scene() = default;
};