#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Enemy.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 16
#define FALL_STEP 2
#define SHIFT 112

enum EnemyAnims
{
	STAND_RIGHT, STAND_LEFT, MOVE_LEFT, MOVE_RIGHT, MOVE_TO_LEFT, MOVE_TO_RIGHT, DIED_RIGHT, DIED_LEFT
};


void Enemy::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	face_direction = true;
	spritesheet.loadFromFile("images/enemy-sprite.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(64, 64), glm::vec2(0.1, 0.2), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(9);

	sprite->setAnimationSpeed(STAND_LEFT, 8);
	sprite->addKeyframe(STAND_LEFT, glm::vec2(0.9f, 0.0f));
	sprite->setSpeed(STAND_LEFT, glm::vec2(0, 0));

	sprite->setAnimationSpeed(STAND_RIGHT, 8);
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.f, 0.f));
	sprite->setSpeed(STAND_RIGHT, glm::vec2(0, 0));

	sprite->setAnimationSpeed(DIED_RIGHT, 8);
	sprite->addKeyframe(DIED_RIGHT, glm::vec2(0.05f, 0.f));
	sprite->setSpeed(DIED_RIGHT, glm::vec2(0, 0));

	sprite->setAnimationSpeed(DIED_LEFT, 8);
	sprite->addKeyframe(DIED_LEFT, glm::vec2(0.9f, 0.f));
	sprite->setSpeed(DIED_LEFT, glm::vec2(0, 0));

	sprite->setAnimationSpeed(MOVE_LEFT, 8);
	for (float i = 8; i > 3; --i) {
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.0f + (i / 10.0f), 0.0f));
		sprite->setSpeed(MOVE_LEFT, glm::vec2(-1, 0));
	}
	/*for (float i = 4; i <= 6; ++i) {
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.0f + (i / 20.0f), 0.15f));
		sprite->setSpeed(MOVE_LEFT, glm::vec2(-2, 0));
	}*/

	sprite->setAnimationSpeed(MOVE_RIGHT, 8);
	for (float i = 0; i <= 4; ++i) {
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.0f + (i / 10.0f), 0.0f));
		sprite->setSpeed(MOVE_RIGHT, glm::vec2(1, 0));
	}

	sprite->setAnimationSpeed(MOVE_TO_LEFT, 8);
	for (float i = 4; i >= 0; --i) {
		sprite->addKeyframe(MOVE_TO_LEFT, glm::vec2(0.0f + (i / 20.0f), 0.5f));
		sprite->setSpeed(MOVE_TO_LEFT, glm::vec2(0, 0));
	}

	sprite->setAnimationSpeed(MOVE_TO_RIGHT, 8);
	for (float i = 0; i < 6; ++i) {
		sprite->addKeyframe(MOVE_TO_RIGHT, glm::vec2(0.0f + (i / 20.0f), 0.5f));
		sprite->setSpeed(MOVE_TO_RIGHT, glm::vec2(0, 0));
	}

	sprite->changeAnimation(1);
	tileMapDispl = tileMapPos;
}

void Enemy::update(int deltaTime, glm::ivec2 posPlayer)
{
	sprite->update(deltaTime);
	posEnemy.x += sprite->getSpeed().x;
	posEnemy.y += sprite->getSpeed().y;
	if (posEnemy.x >= posPlayer.x){
		face_direction = false;
		
	}
	else face_direction = true;
	if (!face_direction){
		if (sprite->animation() != MOVE_LEFT) sprite->changeAnimation(MOVE_LEFT);
		if (sprite->checkChangeAnimation(MOVE_LEFT)) sprite->changeAnimation(MOVE_LEFT);
	}
	else{
		if (sprite->animation() != MOVE_RIGHT) sprite->changeAnimation(MOVE_RIGHT);
		if (sprite->checkChangeAnimation(MOVE_RIGHT)) sprite->changeAnimation(MOVE_RIGHT);
	}
	
	//printf("%d: , %d: ", posEnemy.x, posEnemy.y);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
	
}

void Enemy::render()
{
	sprite->render();
}

void Enemy::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Enemy::setPosition(const glm::vec2 &pos)
{
	posEnemy = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

void Enemy::die(){
	if (face_direction) sprite->changeAnimation(DIED_RIGHT);
	else sprite->changeAnimation(DIED_LEFT);
}