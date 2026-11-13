#pragma once
#include<memory>
class Player;
class Enemy;
class Map;

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
	std::shared_ptr<Player> m_pPlayer;
	std::shared_ptr<Enemy> m_pEnemy;
	std::shared_ptr<Map> m_pMap;
};

