#include "Scene.h"
#include "DuckEngine.h"

class LevelSelectScene : public Scene
{
public:
	void Load() override;
	void Start() override;
	void Update() override;
	void PostUpdate() override;
	void Exit() override;
	void Unload() override;




};