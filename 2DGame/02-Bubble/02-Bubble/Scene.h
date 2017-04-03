#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Torch.h"
#include "Enemy.h"
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
	void clear_torchs();
	void play_music(char * s, bool loop);
	void stop_music();
	void set_game_over();
	void restart_game();
	bool game_over;
	void isEnemyVisible(bool visible);
private:
	void initShaders();

private:
	TileMap *map, *col;
	Player *player;
	Enemy *enemy;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	string level;
	vector<glm::ivec2> antorchas_pos;
	vector<Torch> torchs;
	bool enemyVisible;
};


#endif // _SCENE_INCLUDE

