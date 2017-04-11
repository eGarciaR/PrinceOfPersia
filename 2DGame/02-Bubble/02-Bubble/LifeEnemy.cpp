#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "LifeEnemy.h"
#include "Game.h"

enum PlayerAnims
{
	LIFE0, LIFE1, LIFE2, LIFE3, DIED
};


void LifeEnemy::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	spritesheet.loadFromFile("images/lives-enemy.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(320, 8), glm::vec2(1.0, 0.20), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(5);

	sprite->setAnimationSpeed(LIFE0, 8);
	sprite->addKeyframe(LIFE0, glm::vec2(0.f, 0.0f));
	sprite->setSpeed(LIFE0, glm::vec2(0, 0));

	sprite->setAnimationSpeed(LIFE1, 8);
	sprite->addKeyframe(LIFE1, glm::vec2(0.f, 0.20f));
	sprite->setSpeed(LIFE1, glm::vec2(0, 0));

	sprite->setAnimationSpeed(LIFE2, 8);
	sprite->addKeyframe(LIFE2, glm::vec2(0.f, 0.40f));
	sprite->setSpeed(LIFE2, glm::vec2(0, 0));

	sprite->setAnimationSpeed(LIFE3, 8);
	sprite->addKeyframe(LIFE3, glm::vec2(0.f, 0.60f));
	sprite->setSpeed(LIFE3, glm::vec2(0, 0));

	sprite->setAnimationSpeed(DIED, 8);
	sprite->addKeyframe(DIED, glm::vec2(0.f, 0.80f));
	sprite->setSpeed(DIED, glm::vec2(0, 0));

	sprite->changeAnimation(3);
	tileMapDispl = tileMapPos;
	setPosition(glm::vec2(32, 192));

}

void LifeEnemy::update(int deltaTime)
{
	sprite->update(deltaTime);
}

void LifeEnemy::render()
{
	sprite->render();
}

void LifeEnemy::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void LifeEnemy::setPosition(const glm::vec2 &pos)
{
	posTorch = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posTorch.x), float(tileMapDispl.y + posTorch.y)));
}

void LifeEnemy::changeAnimation(int hp){
	sprite->changeAnimation(hp);
}




