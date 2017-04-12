#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "MagicDoor.h"
#include "Game.h"

enum DoorAnims
{
	CLOSED, OPEN, STAY_OPEN, CLOSE
};


void MagicDoor::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	spritesheet.loadFromFile("images/magicDoor-sprite.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(57, 63), glm::vec2(0.50, 0.5), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(4);

	sprite->setAnimationSpeed(CLOSED, 8);
	sprite->addKeyframe(CLOSED, glm::vec2(0.f, 0.0f));
	sprite->setSpeed(CLOSED, glm::vec2(0, 0));

	sprite->setAnimationSpeed(OPEN, 1);
	sprite->addKeyframe(OPEN, glm::vec2(0.5f, 0.0f));
	sprite->addKeyframe(OPEN, glm::vec2(0.0f, 0.5f));
	sprite->addKeyframe(OPEN, glm::vec2(0.5f, 0.5f));
	sprite->setSpeed(OPEN, glm::vec2(0, 0));
	sprite->setSpeed(OPEN, glm::vec2(0, 0));
	sprite->setSpeed(OPEN, glm::vec2(0, 0));

	sprite->setAnimationSpeed(CLOSE, 8);
	sprite->addKeyframe(CLOSE, glm::vec2(0.5f, 0.5f));
	sprite->addKeyframe(CLOSE, glm::vec2(0.0f, 0.5f));
	sprite->addKeyframe(CLOSE, glm::vec2(0.5f, 0.0f));
	sprite->setSpeed(CLOSE, glm::vec2(0, 0));
	sprite->setSpeed(CLOSE, glm::vec2(0, 0));
	sprite->setSpeed(CLOSE, glm::vec2(0, 0));

	sprite->setAnimationSpeed(STAY_OPEN, 1);
	sprite->addKeyframe(STAY_OPEN, glm::vec2(0.5f, 0.5f));
	sprite->setSpeed(STAY_OPEN, glm::vec2(0, 0));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;

}

void MagicDoor::changeAnimation() {
	if (sprite->animation() != OPEN && sprite->animation() != STAY_OPEN) sprite->changeAnimation(OPEN);
}

void MagicDoor::update(int deltaTime)
{
	sprite->update(deltaTime);
	//printf("%f ", x);
	//sprite->setPosition(glm::vec2(x, y));
	if (sprite->animation() == OPEN) {
		if (sprite->checkChangeAnimation(OPEN)) sprite->changeAnimation(STAY_OPEN);
	}
	else if (sprite->animation() == CLOSE) {
		if (sprite->checkChangeAnimation(CLOSE)) sprite->changeAnimation(CLOSED);
	}
}

void MagicDoor::render()
{
	sprite->render();
}

void MagicDoor::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void MagicDoor::setPosition(const glm::vec2 &pos)
{
	posDoor = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posDoor.x), float(tileMapDispl.y + posDoor.y)));
}

bool MagicDoor::opened(){
	if (sprite->animation() == STAY_OPEN) return true;
	return false;
}

void MagicDoor::closeMagicDoor(){
	if (sprite->animation() != CLOSE && sprite->animation() != CLOSED) sprite->changeAnimation(CLOSE);
}