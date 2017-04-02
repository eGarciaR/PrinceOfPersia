#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player
{

public:
	static Player &instance()
	{
		static Player p;

		return p;
	}
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	void die();
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	
private:
	bool bJumping, climbing, face_direction, jump_long,agachado;
	glm::ivec2 tileMapDispl, posPlayer;
	int jumpAngle, startY, height_air, distancia;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
	void change_level_fast();
	void change_level();
};


#endif // _PLAYER_INCLUDE


