#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Life.h"
#include "Game.h"

enum PlayerAnims
{
	LIFE0,LIFE1,LIFE2,LIFE3
};


void Life::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	spritesheet.loadFromFile("images/lives.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(320, 8), glm::vec2(1.0, 0.25), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(4);

	sprite->setAnimationSpeed(LIFE0, 8);
	sprite->addKeyframe(LIFE0, glm::vec2(0.f, 0.0f));
	sprite->setSpeed(LIFE0, glm::vec2(0, 0));

	sprite->setAnimationSpeed(LIFE1, 8);
	sprite->addKeyframe(LIFE1, glm::vec2(0.f, 0.25f));
	sprite->setSpeed(LIFE1, glm::vec2(0, 0));

	sprite->setAnimationSpeed(LIFE2, 8);
	sprite->addKeyframe(LIFE2, glm::vec2(0.f, 0.50f));
	sprite->setSpeed(LIFE2, glm::vec2(0, 0));

	sprite->setAnimationSpeed(LIFE3, 8);
	sprite->addKeyframe(LIFE3, glm::vec2(0.f, 0.75f));
	sprite->setSpeed(LIFE3, glm::vec2(0, 0));


	sprite->changeAnimation(3);
	tileMapDispl = tileMapPos;
	setPosition(glm::vec2(32,192));

}

void Life::update(int deltaTime, int x, int y)
{
	sprite->update(deltaTime);
	//sprite->setPosition(glm::vec2(x, y));
}

void Life::render()
{
	sprite->render();
}

void Life::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Life::setPosition(const glm::vec2 &pos)
{
	posTorch = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posTorch.x), float(tileMapDispl.y + posTorch.y)));
}

void Life::changeAnimation(int hp){
	sprite->changeAnimation(hp);
}



