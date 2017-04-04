#ifndef _DOOR_INCLUDE
#define _DOOR_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


class Door
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	Door door();
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	void changeAnimation();
	bool opened();

private:
	glm::ivec2 tileMapDispl, posDoor;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;

};


#endif // _DOOR_INCLUDE