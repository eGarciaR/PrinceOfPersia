#ifndef _INTRO_INCLUDE
#define _INTRO_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


class Intro
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime, int x, int y);
	void render();
	Intro intro();
	void setPosition(const glm::vec2 &pos);

private:
	glm::ivec2 tileMapDispl, posIntro;
	int jumpAngle, startY, height_air, distancia, elapsedTime;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;

};


#endif // _INTRO_INCLUDE