#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Door.h"
#include "Game.h"

enum DoorAnims
{
	CLOSED, OPEN, STAY_OPEN
};


void Door::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	spritesheet.loadFromFile("images/door-sprite.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(64, 64), glm::vec2(0.25, 1), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(3);

	sprite->setAnimationSpeed(CLOSED, 8);
	sprite->addKeyframe(CLOSED, glm::vec2(0.f, 0.0f));
	sprite->setSpeed(CLOSED, glm::vec2(0, 0));

	sprite->setAnimationSpeed(OPEN, 1);
	sprite->addKeyframe(OPEN, glm::vec2(0.25f, 0.0f));
	sprite->addKeyframe(OPEN, glm::vec2(0.5f, 0.0f));
	sprite->addKeyframe(OPEN, glm::vec2(0.75f, 0.0f));
	sprite->setSpeed(OPEN, glm::vec2(0, 0));
	sprite->setSpeed(OPEN, glm::vec2(0, 0));
	sprite->setSpeed(OPEN, glm::vec2(0, 0));
	sprite->setSpeed(OPEN, glm::vec2(0, 0));

	sprite->setAnimationSpeed(STAY_OPEN, 1);
	sprite->addKeyframe(STAY_OPEN, glm::vec2(0.75f, 0.0f));
	sprite->setSpeed(STAY_OPEN, glm::vec2(0, 0));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;

}

void Door::changeAnimation() {
	if (sprite->animation() != OPEN && sprite->animation() != STAY_OPEN) sprite->changeAnimation(OPEN);
}

void Door::update(int deltaTime)
{
	sprite->update(deltaTime);
	//printf("%f ", x);
	//sprite->setPosition(glm::vec2(x, y));
	if (sprite->animation() == OPEN) {
		if (sprite->checkChangeAnimation(OPEN)) sprite->changeAnimation(STAY_OPEN);
	}
}

void Door::render()
{
	sprite->render();
}

void Door::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Door::setPosition(const glm::vec2 &pos)
{
	posDoor = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posDoor.x), float(tileMapDispl.y + posDoor.y)));
}
