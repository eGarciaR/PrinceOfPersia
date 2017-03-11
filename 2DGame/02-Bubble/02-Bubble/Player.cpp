#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4


enum PlayerAnims
{
	STAND_RIGHT, STAND_LEFT, MOVE_LEFT, MOVE_RIGHT, START_RIGHT, STOP_RIGHT, START_LEFT, STOP_LEFT
};


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	bJumping = false;
	spritesheet.loadFromFile("images/prince-sprite.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(64, 64), glm::vec2(0.1, 0.1), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(8);
	
	sprite->setAnimationSpeed(STAND_LEFT, 8);
	sprite->addKeyframe(STAND_LEFT, glm::vec2(0.f, 0.2f));
		
	sprite->setAnimationSpeed(STAND_RIGHT, 8);
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.f, 0.f));
		
	sprite->setAnimationSpeed(MOVE_LEFT, 8);
	for (float i = 6; i > 3; --i) {
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.0f + (i / 10.0f),0.3f ));
	}
	for (float i = 4; i <= 6; ++i) {
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.0f + (i / 10.0f), 0.3f));
	}
		
	sprite->setAnimationSpeed(MOVE_RIGHT, 8);
	for (float i = 3; i < 6; ++i) {
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.0f + (i / 10.0f), 0.1f));
	}
	for (float i = 5; i >= 3; --i) {
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.0f + (i / 10.0f), 0.1f));
	}

	sprite->setAnimationSpeed(START_RIGHT, 8);
	sprite->addKeyframe(START_RIGHT, glm::vec2(0.0f, 0.1f));
	sprite->addKeyframe(START_RIGHT, glm::vec2(0.1f, 0.1f));
	sprite->addKeyframe(START_RIGHT, glm::vec2(0.2f, 0.1f));

	sprite->setAnimationSpeed(START_LEFT, 8);
	sprite->addKeyframe(START_LEFT, glm::vec2(0.9f, 0.3f));
	sprite->addKeyframe(START_LEFT, glm::vec2(0.8f, 0.3f));
	sprite->addKeyframe(START_LEFT, glm::vec2(0.7f, 0.3f));

	sprite->setAnimationSpeed(STOP_RIGHT, 8);
	sprite->addKeyframe(STOP_RIGHT, glm::vec2(0.7f, 0.1f));
	sprite->addKeyframe(STOP_RIGHT, glm::vec2(0.8f, 0.1f));
	sprite->addKeyframe(STOP_RIGHT, glm::vec2(0.9f, 0.1f));
	sprite->addKeyframe(STOP_RIGHT, glm::vec2(0.f, 0.f));

	sprite->setAnimationSpeed(STOP_LEFT, 8);
	sprite->addKeyframe(STOP_LEFT, glm::vec2(0.2f, 0.3f));
	sprite->addKeyframe(STOP_LEFT, glm::vec2(0.1f, 0.3f));
	sprite->addKeyframe(STOP_LEFT, glm::vec2(0.0f, 0.3f));
	sprite->addKeyframe(STOP_LEFT, glm::vec2(0.f, 0.2f));
		
	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	
}

void Player::update(int deltaTime)
{
	sprite->update(deltaTime);
	if(Game::instance().getSpecialKey(GLUT_KEY_LEFT))
	{
		if (sprite->animation() != START_LEFT && sprite->animation() != MOVE_LEFT) {	// Si a�n no ha empezado a moverse y no estaba corriendo -> Empezamos a correr
			sprite->changeAnimation(START_LEFT);	// Cambiamos animaci�n a empezar a correr
			sprite->resetTimeStarted();		// Reseteamos el tiempo de empezar a correr
			sprite->started();		// Empezamos a incrementar el tiempo de empezar a correr
		}
		if (sprite->animation() == START_LEFT) {	
			if (sprite->canRun())	  // Si ha empezado a correr y por el tiempo pasado ya puede hacer el ciclo de correr -> Cambiamos animaci�n
				sprite->changeAnimation(MOVE_LEFT);		// Cambiamos animaci�n al ciclo de correr
			else sprite->started();		// Si a�n no ha pasado el tiempo necesario de empezar a correr, incrementamos el tiempo
		}
		posPlayer.x -= 2;
		if (map->collisionMoveLeft(posPlayer, glm::ivec2(40, 32)))
		{
			posPlayer.x += 2;
			sprite->changeAnimation(STAND_LEFT);
		}
	}
	else if(Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
	{
		if (sprite->animation() != START_RIGHT && sprite->animation() != MOVE_RIGHT) {
			sprite->changeAnimation(START_RIGHT);
			sprite->resetTimeStarted();
			sprite->started();
		}
		if (sprite->animation() == START_RIGHT) {
			if (sprite->canRun())
				sprite->changeAnimation(MOVE_RIGHT);
			else sprite->started();
		}

		posPlayer.x += 2;
		
		if(map->collisionMoveRight(posPlayer, glm::ivec2(40, 32)))
		{
			posPlayer.x -= 2;
			sprite->changeAnimation(STAND_RIGHT);
		}
	}
	else
	{
		if (sprite->animation() == MOVE_LEFT) {
			sprite->changeAnimation(STOP_LEFT);
			sprite->resetTimePassed();
		}
		else if (sprite->animation() == MOVE_RIGHT) {
			sprite->changeAnimation(STOP_RIGHT);
			sprite->resetTimePassed();
		}
		else if (sprite->animation() == START_RIGHT) {
			sprite->changeAnimation(STAND_RIGHT);
		}
		else if (sprite->animation() == START_LEFT) {
			sprite->changeAnimation(STAND_LEFT);
		}
		else if (sprite->animation() == STOP_RIGHT) {
			posPlayer.x += 1;
			if (sprite->getTimePassed() > 25)
				sprite->changeAnimation(STAND_RIGHT);
			else sprite->addTime();
		}
		else if (sprite->animation() == STOP_LEFT) {
			posPlayer.x -= 1;
			if (sprite->getTimePassed() > 25)
				sprite->changeAnimation(STAND_LEFT);
			else sprite->addTime();
		}
	}
	
	if(bJumping)
	{
		jumpAngle += JUMP_ANGLE_STEP;
		if(jumpAngle == 180)
		{
			bJumping = false;
			posPlayer.y = startY;
		}
		else
		{
			posPlayer.y = int(startY - 96 * sin(3.14159f * jumpAngle / 180.f));
			if(jumpAngle > 90)
				bJumping = !map->collisionMoveDown(posPlayer, glm::ivec2(32, 64), &posPlayer.y);
		}
	}
	else
	{
		posPlayer.y += FALL_STEP;
		if(map->collisionMoveDown(posPlayer, glm::ivec2(32, 64), &posPlayer.y))
		{
			if(Game::instance().getSpecialKey(GLUT_KEY_UP))
			{
				bJumping = true;
				jumpAngle = 0;
				startY = posPlayer.y;
			}
		}
	}
	
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::render()
{
	sprite->render();
}

void Player::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Player::setPosition(const glm::vec2 &pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}




