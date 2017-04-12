#ifndef _MAGICDOOR_INCLUDE
#define _MAGICDOOR_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


class MagicDoor
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	MagicDoor door();
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	void changeAnimation();
	bool opened();
	void closeMagicDoor();
	bool magicDoorOpened();
private:
	glm::ivec2 tileMapDispl, posDoor;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;

};


#endif // _MAGICDOOR_INCLUDE