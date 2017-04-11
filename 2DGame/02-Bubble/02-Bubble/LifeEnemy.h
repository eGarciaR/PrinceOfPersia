#ifndef _LIFEENEMY_INCLUDE
#define _LIFEENEMY_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


class LifeEnemy
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	LifeEnemy lifeEnemy();
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	void changeAnimation(int hp);

private:
	glm::ivec2 tileMapDispl, posTorch;
	int jumpAngle, startY, height_air, distancia;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;

};


#endif // _Lifeenemy_INCLUDE