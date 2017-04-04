#ifndef _LIFE_INCLUDE
#define _LIFE_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


class Life
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime, int x, int y);
	void render();
	Life life();
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


#endif // _LIFE_INCLUDE