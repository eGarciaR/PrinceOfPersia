#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Torch.h"
#include "Life.h"
#include "LifeEnemy.h"
#include "Door.h"
#include "Enemy.h"
#include "Intro.h"
#include <windows.h> 
#include <conio.h>
#include <stdio.h>


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

	static Scene &instance()
	{
		static Scene S;

		return S;
	}

	void init();
	void update(int deltaTime);
	void render();
	string getLevel();
	void setLevel(string s, glm::vec2 &pos, string column, bool enemy, glm::ivec2 &posEnemy);
	void setAntorcha(glm::ivec2 &pos);
	void setDoor(glm::ivec2 &pos);
	void clear_torchs();
	void clear_doors();
	void play_music(char * s, bool loop);
	void stop_music();
	void set_game_over();
	void restart_game();
	bool game_over;
	void isEnemyVisible(bool visible);
	bool getEnemyVisible();
	void openDoor();
	bool isDoorOpened();
	void changeHealthAnimation(int hp);
	void changeHealthEnemyAnimation(int hp);
	float getCurrentTime();
	bool fin_intro;
	glm::ivec2 getEnemyPosition();
	glm::ivec2 getPlayerPosition();
	void setEnemyCreated(bool created);
	bool isEnemyCreated();
	void setEnemyPosition(glm::ivec2 pos);
	int getPlayerAnimation();
	void PlayerDamaged();
	bool PlayerDied();
	bool EnemyDied();
private:
	void initShaders();

private:
	TileMap *map, *col;
	Player *player;
	Enemy *enemy;
	Life *life;
	LifeEnemy *lifeEnemy;
	Intro *intro;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	string level;
	vector<glm::ivec2> antorchas_pos;
	vector<glm::ivec2> door_pos;
	vector<Torch> torchs;
	vector<Door> doors;
	bool enemyVisible;
	bool doorOpened;
};


#endif // _SCENE_INCLUDE

