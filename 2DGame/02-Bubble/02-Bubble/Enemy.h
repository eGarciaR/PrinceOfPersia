#ifndef _ENEMY_INCLUDE
#define _ENEMY_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Enemy
{

public:
	static Enemy &instance()
	{
		static Enemy p;

		return p;
	}
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, string file);
	void update(int deltaTime, glm::ivec2 posPlayer);
	void render();
	void die();
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	glm::ivec2 getEnemyPosition();
	void setEnemyCreated(bool created);
	bool isEnemyCreated();
	void setEnemyPositon(glm::ivec2 pos);
	int getEnemyHp();
private:
	bool bJumping, climbing, face_direction, jump_long, agachado, music_collision, music_step, music_collision_over, created, atacking, playerAttacking, blocked, music_on;
	glm::ivec2 tileMapDispl, posEnemy;
	int jumpAngle, startY, height_air, distancia;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
	int hp;
	void change_level_fast();
	void change_level();
};


#endif // _ENEMY_INCLUDE


