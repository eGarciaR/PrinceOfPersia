#ifndef _TORCH_INCLUDE
#define _TORCH_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


class Torch
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime, int x, int y);
	void render();
	Torch torch();
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);

private:
	glm::ivec2 tileMapDispl, posTorch;
	int jumpAngle, startY, height_air, distancia;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;

};


#endif // _TORCH_INCLUDE