class Scene
{
public:
	virtual void Load();
	virtual void Start();
	virtual void Update();
	virtual void PostUpdate();
	virtual void Exit();
	virtual void Unload();

};