#pragma once
class Player;
class Enemy;

class SceneMain
{
public:
	SceneMain();
	~SceneMain();

	void Init();
	void End();

	void Update();
	void Draw();

private:
	int m_frameCount;
	Player* m_pPlayer;
	Enemy* m_pEnemy;
};

