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
	STAND_RIGHT, STAND_LEFT, MOVE_LEFT, MOVE_RIGHT, START_RIGHT, STOP_RIGHT, START_LEFT, STOP_LEFT, FALL_RIGHT, FALL_LEFT, START_FALL_RIGHT, MOVE_TO_LEFT, MOVE_TO_RIGHT, MOVE_TO_LEFT_RUNNING, MOVE_TO_RIGHT_RUNNING
};


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	bJumping = false;
	spritesheet.loadFromFile("images/prince-sprite.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(64, 64), glm::vec2(0.1, 0.05), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(15);
	
	sprite->setAnimationSpeed(STAND_LEFT, 8);
	sprite->addKeyframe(STAND_LEFT, glm::vec2(0.f, 0.1f));
		
	sprite->setAnimationSpeed(STAND_RIGHT, 8);
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.f, 0.f));
		
	sprite->setAnimationSpeed(MOVE_LEFT, 8);
	for (float i = 6; i > 3; --i) {
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.0f + (i / 10.0f),0.15f ));
	}
	for (float i = 4; i <= 6; ++i) {
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.0f + (i / 10.0f), 0.15f));
	}
		
	sprite->setAnimationSpeed(MOVE_RIGHT, 8);
	for (float i = 3; i < 6; ++i) {
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.0f + (i / 10.0f), 0.05f));
	}
	for (float i = 5; i >= 3; --i) {
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.0f + (i / 10.0f), 0.05f));
	}

	sprite->setAnimationSpeed(START_RIGHT, 8);
	sprite->addKeyframe(START_RIGHT, glm::vec2(0.0f, 0.05f));
	sprite->addKeyframe(START_RIGHT, glm::vec2(0.1f, 0.05f));
	sprite->addKeyframe(START_RIGHT, glm::vec2(0.2f, 0.05f));

	sprite->setAnimationSpeed(START_LEFT, 8);
	sprite->addKeyframe(START_LEFT, glm::vec2(0.9f, 0.15f));
	sprite->addKeyframe(START_LEFT, glm::vec2(0.8f, 0.15f));
	sprite->addKeyframe(START_LEFT, glm::vec2(0.7f, 0.15f));

	sprite->setAnimationSpeed(STOP_RIGHT, 8);
	sprite->addKeyframe(STOP_RIGHT, glm::vec2(0.7f, 0.05f));
	sprite->addKeyframe(STOP_RIGHT, glm::vec2(0.8f, 0.05f));
	sprite->addKeyframe(STOP_RIGHT, glm::vec2(0.9f, 0.05f));
	sprite->addKeyframe(STOP_RIGHT, glm::vec2(0.f, 0.f));

	sprite->setAnimationSpeed(STOP_LEFT, 8);
	sprite->addKeyframe(STOP_LEFT, glm::vec2(0.2f, 0.15f));
	sprite->addKeyframe(STOP_LEFT, glm::vec2(0.1f, 0.15f));
	sprite->addKeyframe(STOP_LEFT, glm::vec2(0.0f, 0.15f));
	sprite->addKeyframe(STOP_LEFT, glm::vec2(0.f, 0.1f));

	sprite->setAnimationSpeed(FALL_RIGHT, 8);
	for (float i = 1; i < 10; ++i) {
		sprite->addKeyframe(FALL_RIGHT, glm::vec2(0.0f + (i / 10.0f), 0.25f));
	}

	sprite->setAnimationSpeed(START_FALL_RIGHT, 8);
	for (float i = 0; i < 4; ++i) {
		sprite->addKeyframe(START_FALL_RIGHT, glm::vec2(0.0f + (i / 10.0f), 0.2f));
	}

	sprite->setAnimationSpeed(MOVE_TO_LEFT, 8);
	for (float i = 4; i > 0; --i) {
		sprite->addKeyframe(MOVE_TO_LEFT, glm::vec2(0.0f + (i / 10.0f), 0.5f));
	}

	sprite->setAnimationSpeed(MOVE_TO_LEFT_RUNNING, 8);
	for (float i = 0; i < 10; ++i) {
		sprite->addKeyframe(MOVE_TO_LEFT_RUNNING, glm::vec2(0.0f + (i / 10.0f), 0.55f));
	}
	sprite->addKeyframe(MOVE_TO_LEFT_RUNNING, glm::vec2(0.9f, 0.55f));
		
	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	
}

void Player::update(int deltaTime)
{
	sprite->update(deltaTime);
	if(Game::instance().getSpecialKey(GLUT_KEY_LEFT))
	{
		if (sprite->animation() == STAND_RIGHT) {
			sprite->changeAnimation(MOVE_TO_LEFT);
			posPlayer.x += 2;
		}
		if ((sprite->animation() != START_LEFT && sprite->animation() != MOVE_LEFT) && sprite->animation() != MOVE_TO_LEFT) {	// Si aún no ha empezado a moverse y no estaba corriendo -> Empezamos a correr
			//sprite->changeAnimation(START_LEFT);	// Cambiamos animación a empezar a correr
			sprite->changeAnimation(START_LEFT);
		}
		if (sprite->animation() == START_LEFT) {	
			if (sprite->checkChangeAnimation(START_LEFT))	  // Si ha empezado a correr y por el tiempo pasado ya puede hacer el ciclo de correr -> Cambiamos animación
				sprite->changeAnimation(MOVE_LEFT);
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
		}
		if (sprite->animation() == START_RIGHT) {
			if (sprite->checkChangeAnimation(START_RIGHT))
				sprite->changeAnimation(MOVE_RIGHT);
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
		}
		else if (sprite->animation() == MOVE_RIGHT) {
			sprite->changeAnimation(STOP_RIGHT);
		}
		else if (sprite->animation() == START_RIGHT) {
			sprite->changeAnimation(STOP_RIGHT);
		}
		else if (sprite->animation() == START_LEFT) {
			sprite->changeAnimation(STOP_LEFT);
		}
		else if (sprite->animation() == STOP_RIGHT) {
			posPlayer.x += 1;
			if (sprite->checkChangeAnimation(STOP_RIGHT))
				sprite->changeAnimation(STAND_RIGHT);
		}
		else if (sprite->animation() == STOP_LEFT) {
			posPlayer.x -= 1;
			if (sprite->checkChangeAnimation(STOP_LEFT))
				sprite->changeAnimation(STAND_LEFT);
		}
		else if (sprite->animation() == FALL_RIGHT) {
			if (sprite->checkChangeAnimation(FALL_RIGHT))
				sprite->changeAnimation(STAND_RIGHT);
		}
		else if (sprite->animation() == MOVE_TO_LEFT) {
			if (sprite->checkChangeAnimation(MOVE_TO_LEFT))
				sprite->changeAnimation(STAND_LEFT);
		}
		else if (sprite->animation() == START_FALL_RIGHT) {
			if (sprite->checkChangeAnimation(START_FALL_RIGHT))
				sprite->changeAnimation(FALL_RIGHT);
		}
		else if (sprite->animation() == MOVE_TO_LEFT_RUNNING) {
			if (sprite->checkChangeAnimation(MOVE_TO_LEFT_RUNNING))
				sprite->changeAnimation(MOVE_LEFT);
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
	{	// Empieza a caer
		posPlayer.y += FALL_STEP;
		if(map->collisionMoveDown(posPlayer, glm::ivec2(32, 64), &posPlayer.y))
		{
			if(Game::instance().getSpecialKey(GLUT_KEY_UP))
			{
				bJumping = true;
				jumpAngle = 0;
				startY = posPlayer.y;
			}
			else if (sprite->isFalling()) {
				// Amortiguar caída
				sprite->changeAnimation(FALL_RIGHT);
			}
			sprite->falling(false);
		}
		else sprite->falling(true);
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




