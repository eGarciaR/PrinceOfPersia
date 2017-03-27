#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 16 /*Calibrar*/
#define FALL_STEP 2


enum PlayerAnims
{
	STAND_RIGHT, STAND_LEFT, MOVE_LEFT, MOVE_RIGHT, START_RIGHT, STOP_RIGHT, START_LEFT, STOP_LEFT, FALL_RIGHT, FALL_LEFT, START_FALL_RIGHT, 
	MOVE_TO_LEFT, MOVE_TO_RIGHT, MOVE_TO_LEFT_RUNNING, MOVE_TO_RIGHT_RUNNING, START_JUMP_RIGHT, JUMP_RIGHT, JUMP_FALL_RIGHT, JUMP_LEFT, 
	START_JUMP_LEFT, JUMP_FALL_LEFT, MOVE_STEP_LEFT, MOVE_STEP_RIGHT, CLIMB_RIGHT, CLIMB_LEFT
};


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	bJumping = false;
	climbing = false;
	spritesheet.loadFromFile("images/prince-sprite.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(64, 64), glm::vec2(0.05, 0.05), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(25);
	
	sprite->setAnimationSpeed(STAND_LEFT, 8);
	sprite->addKeyframe(STAND_LEFT, glm::vec2(0.f, 0.1f));
		
	sprite->setAnimationSpeed(STAND_RIGHT, 8);
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.f, 0.f));
		
	sprite->setAnimationSpeed(MOVE_LEFT, 8);
	for (float i = 6; i > 3; --i) {
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.0f + (i / 20.0f),0.15f ));
	}
	for (float i = 4; i <= 6; ++i) {
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.0f + (i / 20.0f), 0.15f));
	}
		
	sprite->setAnimationSpeed(MOVE_RIGHT, 8);
	for (float i = 3; i < 6; ++i) {
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.0f + (i / 20.0f), 0.05f));
	}
	for (float i = 5; i >= 3; --i) {
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.0f + (i / 20.0f), 0.05f));
	}

	sprite->setAnimationSpeed(START_RIGHT, 8);
	sprite->addKeyframe(START_RIGHT, glm::vec2(0.0f, 0.05f));
	sprite->addKeyframe(START_RIGHT, glm::vec2(0.05f, 0.05f));
	sprite->addKeyframe(START_RIGHT, glm::vec2(0.1f, 0.05f));

	sprite->setAnimationSpeed(START_LEFT, 8);
	sprite->addKeyframe(START_LEFT, glm::vec2(0.45f, 0.15f));
	sprite->addKeyframe(START_LEFT, glm::vec2(0.4f, 0.15f));
	sprite->addKeyframe(START_LEFT, glm::vec2(0.35f, 0.15f));

	sprite->setAnimationSpeed(STOP_RIGHT, 8);
	sprite->addKeyframe(STOP_RIGHT, glm::vec2(0.35f, 0.05f));
	sprite->addKeyframe(STOP_RIGHT, glm::vec2(0.4f, 0.05f));
	sprite->addKeyframe(STOP_RIGHT, glm::vec2(0.45f, 0.05f));
	sprite->addKeyframe(STOP_RIGHT, glm::vec2(0.f, 0.f));

	sprite->setAnimationSpeed(STOP_LEFT, 8);
	sprite->addKeyframe(STOP_LEFT, glm::vec2(0.1f, 0.15f));
	sprite->addKeyframe(STOP_LEFT, glm::vec2(0.05f, 0.15f));
	sprite->addKeyframe(STOP_LEFT, glm::vec2(0.0f, 0.15f));
	sprite->addKeyframe(STOP_LEFT, glm::vec2(0.f, 0.1f));

	sprite->setAnimationSpeed(FALL_RIGHT, 8);
	for (float i = 1; i < 10; ++i) {
		sprite->addKeyframe(FALL_RIGHT, glm::vec2(0.0f + (i / 20.0f), 0.25f));
	}

	sprite->setAnimationSpeed(START_FALL_RIGHT, 6);
	for (float i = 0; i <= 4; ++i) {
		sprite->addKeyframe(START_FALL_RIGHT, glm::vec2(0.0f + (i / 20.0f), 0.2f));
	}

	sprite->setAnimationSpeed(MOVE_TO_LEFT, 8);
	for (float i = 4; i >= 0; --i) {
		sprite->addKeyframe(MOVE_TO_LEFT, glm::vec2(0.0f + (i / 20.0f), 0.5f));
	}

	sprite->setAnimationSpeed(MOVE_TO_RIGHT, 8);
	for (float i = 0; i < 6; ++i) {
		sprite->addKeyframe(MOVE_TO_RIGHT, glm::vec2(0.0f + (i / 20.0f), 0.5f));
	}

	sprite->setAnimationSpeed(MOVE_TO_LEFT_RUNNING, 8);
	for (float i = 0; i < 10; ++i) {
		sprite->addKeyframe(MOVE_TO_LEFT_RUNNING, glm::vec2(0.0f + (i / 20.0f), 0.55f));
	}
	//sprite->addKeyframe(MOVE_TO_LEFT_RUNNING, glm::vec2(0.45f, 0.55f)); // why????

	sprite->setAnimationSpeed(MOVE_TO_RIGHT_RUNNING, 8);
	for (float i = 19; i >= 10; --i) {
		sprite->addKeyframe(MOVE_TO_RIGHT_RUNNING, glm::vec2(0.0f + (i / 20.0f), 0.55f));
	}

	sprite->setAnimationSpeed(START_JUMP_RIGHT, 8);
	for (float i = 0; i < 8; ++i) {
		sprite->addKeyframe(START_JUMP_RIGHT, glm::vec2(0.0f + (i / 20.0f), 0.6f));
	}

	sprite->setAnimationSpeed(JUMP_RIGHT, 8);
	for (float i = 8; i < 10; ++i) {
		sprite->addKeyframe(JUMP_RIGHT, glm::vec2(0.0f + (i / 20.0f), 0.6f));
	}

	sprite->setAnimationSpeed(START_JUMP_LEFT, 8);
	for (float i = 19; i >= 12; --i) {
		sprite->addKeyframe(START_JUMP_LEFT, glm::vec2(0.0f + (i / 20.0f), 0.6f));
	}

	sprite->setAnimationSpeed(JUMP_LEFT, 8);
	for (float i = 11; i >= 10; --i) {
		sprite->addKeyframe(JUMP_LEFT, glm::vec2(0.0f + (i / 20.0f), 0.6f));
	}

	sprite->setAnimationSpeed(MOVE_STEP_LEFT, 8);
	for (float i = 19; i >= 12; --i) {
		sprite->addKeyframe(MOVE_STEP_LEFT, glm::vec2(0.0f + (i / 20.0f), 0.8f));
	}

	sprite->setAnimationSpeed(JUMP_FALL_RIGHT, 8);
	sprite->addKeyframe(JUMP_FALL_RIGHT, glm::vec2(0.4f, 0.75f));
	sprite->addKeyframe(JUMP_FALL_RIGHT, glm::vec2(0.45f, 0.75f));

	sprite->setAnimationSpeed(JUMP_FALL_LEFT, 8);
	sprite->addKeyframe(JUMP_FALL_LEFT, glm::vec2(0.55f, 0.75f));
	sprite->addKeyframe(JUMP_FALL_LEFT, glm::vec2(0.5f, 0.75f));

	sprite->setAnimationSpeed(CLIMB_RIGHT, 8);
	/*for (float i = 0; i < 8; ++i) {
		sprite->addKeyframe(CLIMB_RIGHT, glm::vec2(0.0f + (i / 20.0f), 0.6f));
	}*/
	/*for (float i = 8; i < 10; ++i) {
		sprite->addKeyframe(CLIMB_RIGHT, glm::vec2(0.0f + (i / 20.0f), 0.6f));
	}*/
	for (float i = 0; i < 9; ++i) {
		sprite->addKeyframe(CLIMB_RIGHT, glm::vec2(0.0f + (i / 20.0f), 0.65f));
	}
		
	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	
}

void Player::update(int deltaTime)
{
	sprite->update(deltaTime);
	if (Game::instance().getSpecialKey(GLUT_KEY_LEFT))
	{
		if (!bJumping){
			/*Dado que haces el cambio soltando antes la tecla derecha, se tiene q hacer con el STOP*/
			//if (sprite->animation() == STOP_RIGHT/*sprite->animation() == MOVE_RIGHT || sprite->animation() == START_RIGHT*/) {
			//sprite->changeAnimation(MOVE_TO_LEFT_RUNNING); //NO SE XQ NARICES NO LO HACE!!!!!
			//posPlayer.x += 2;
			//}
			if (sprite->animation() == MOVE_RIGHT || sprite->animation() == STOP_RIGHT) {
				sprite->changeAnimation(MOVE_TO_LEFT_RUNNING);
			}
			if (sprite->animation() == MOVE_TO_LEFT_RUNNING) {
				if (sprite->checkChangeAnimation(MOVE_TO_LEFT_RUNNING)) {
					sprite->changeAnimation(MOVE_LEFT);
				}
				posPlayer.x += 3;
			}
			if (sprite->animation() == STAND_RIGHT) {
				sprite->changeAnimation(MOVE_TO_LEFT);
				posPlayer.x += 2;
			}
			if (sprite->animation() == MOVE_TO_LEFT) {
				if (sprite->checkChangeAnimation(MOVE_TO_LEFT)) {
					sprite->changeAnimation(STAND_LEFT);
				}
				posPlayer.x += 2;
			}
			if ((sprite->animation() != START_LEFT && sprite->animation() != MOVE_LEFT) && sprite->animation() != MOVE_TO_LEFT && sprite->animation() != MOVE_TO_LEFT_RUNNING) {	// Si aún no ha empezado a moverse y no estaba corriendo -> Empezamos a correr
				if (Game::instance().getSpecialKey(GLUT_ACTIVE_SHIFT))
					sprite->changeAnimation(MOVE_STEP_LEFT);
				else sprite->changeAnimation(START_LEFT);
			}
			if (sprite->animation() == START_LEFT) {
				if (sprite->checkChangeAnimation(START_LEFT))	  // Si ha empezado a correr y por el tiempo pasado ya puede hacer el ciclo de correr -> Cambiamos animación
					sprite->changeAnimation(MOVE_LEFT);
			}
			posPlayer.x -= 2;
			if (map->collisionMoveLeft(posPlayer, glm::ivec2(32, 64)))
			{
				posPlayer.x += 2;
				sprite->changeAnimation(STAND_LEFT);
			}
			else if (map->collisionMoveRight(posPlayer, glm::ivec2(32, 64)))
			{
				posPlayer.x -= 3;
				sprite->changeAnimation(STAND_RIGHT);
			}
		}
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
	{
		if (!bJumping){
			if (sprite->animation() == MOVE_LEFT || sprite->animation() == STOP_LEFT) {
				sprite->changeAnimation(MOVE_TO_RIGHT_RUNNING);
			}
			if (sprite->animation() == MOVE_TO_RIGHT_RUNNING) {
				if (sprite->checkChangeAnimation(MOVE_TO_RIGHT_RUNNING)) {
					sprite->changeAnimation(MOVE_RIGHT);
				}
				posPlayer.x -= 3;
			}
			if (sprite->animation() == STAND_LEFT) {
				sprite->changeAnimation(MOVE_TO_RIGHT);
				posPlayer.x -= 2;
			}
			if (sprite->animation() == MOVE_TO_RIGHT) {
				if (sprite->checkChangeAnimation(MOVE_TO_RIGHT)) {
					sprite->changeAnimation(STAND_RIGHT);
				}
				posPlayer.x -= 2;
			}
			if (sprite->animation() != START_RIGHT && sprite->animation() != MOVE_RIGHT && sprite->animation() != MOVE_TO_RIGHT && sprite->animation() != MOVE_TO_RIGHT_RUNNING) {
				sprite->changeAnimation(START_RIGHT);
			}
			if (sprite->animation() == START_RIGHT) {
				if (sprite->checkChangeAnimation(START_RIGHT)){
					sprite->changeAnimation(MOVE_RIGHT);
				}
			}
			posPlayer.x += 2;

			if (map->collisionMoveRight(posPlayer, glm::ivec2(32, 64)))
			{
				posPlayer.x -= 2;
				sprite->changeAnimation(STAND_RIGHT);
			}
			else if (map->collisionMoveLeft(posPlayer, glm::ivec2(32, 64)))
			{
				posPlayer.x += 3;
				sprite->changeAnimation(STAND_LEFT);
			}
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
		else if (sprite->animation() == FALL_LEFT) {
			if (sprite->checkChangeAnimation(FALL_LEFT))
				sprite->changeAnimation(STAND_LEFT);
		}
		else if (sprite->animation() == MOVE_TO_LEFT) {
			if (sprite->checkChangeAnimation(MOVE_TO_LEFT))
				sprite->changeAnimation(STAND_LEFT);
		}
		else if (sprite->animation() == MOVE_TO_RIGHT) {
			if (sprite->checkChangeAnimation(MOVE_TO_RIGHT))
				sprite->changeAnimation(STAND_RIGHT);
		}
		else if (sprite->animation() == START_FALL_RIGHT) {
			if (sprite->checkChangeAnimation(START_FALL_RIGHT))
				sprite->changeAnimation(FALL_RIGHT);
		}
		else if (sprite->animation() == MOVE_TO_LEFT_RUNNING) {
			if (sprite->checkChangeAnimation(MOVE_TO_LEFT_RUNNING))
				sprite->changeAnimation(MOVE_LEFT);
		}
		else if (sprite->animation() == MOVE_TO_RIGHT_RUNNING) {
			if (sprite->checkChangeAnimation(MOVE_TO_RIGHT_RUNNING))
				sprite->changeAnimation(MOVE_RIGHT);
		}
		else if (sprite->animation() == JUMP_RIGHT) {
			if (sprite->checkChangeAnimation(JUMP_RIGHT)) {
				if(!climbing)sprite->changeAnimation(JUMP_FALL_RIGHT);
				else sprite->changeAnimation(CLIMB_RIGHT);
			}
		}
		else if (sprite->animation() == JUMP_FALL_RIGHT) {
			if (sprite->checkChangeAnimation(JUMP_FALL_RIGHT))
				sprite->changeAnimation(STAND_RIGHT);
		}
		else if (sprite->animation() == JUMP_LEFT) {
			if (sprite->checkChangeAnimation(JUMP_LEFT))
			if (map->collisionClimb(posPlayer, glm::ivec2(32, 72), &posPlayer.y, false))
				sprite->changeAnimation(CLIMB_LEFT);
			else sprite->changeAnimation(JUMP_FALL_LEFT);
		}
		else if (sprite->animation() == JUMP_FALL_LEFT) {
			if (sprite->checkChangeAnimation(JUMP_FALL_LEFT))
				sprite->changeAnimation(STAND_LEFT);
		}
		else if (sprite->animation() == START_JUMP_RIGHT) {
			if (sprite->checkChangeAnimation(START_JUMP_RIGHT)) {
				sprite->changeAnimation(JUMP_RIGHT);
			}
		}
		else if (sprite->animation() == START_JUMP_LEFT) {
			if (sprite->checkChangeAnimation(START_JUMP_LEFT))
				sprite->changeAnimation(JUMP_LEFT);
		}
		else if (sprite->animation() == CLIMB_RIGHT) {
			//if (sprite->checkChangeAnimation(CLIMB_RIGHT))
				//sprite->changeAnimation(STAND_RIGHT);
		}
		else if (sprite->animation() == CLIMB_LEFT) {
			if (sprite->checkChangeAnimation(CLIMB_LEFT))
				sprite->changeAnimation(STAND_LEFT);
		}
	}
	if (bJumping)
	{
		if (sprite->animation() == JUMP_RIGHT || sprite->animation() == JUMP_LEFT){
			jumpAngle += JUMP_ANGLE_STEP;
			if (sprite->animation() == JUMP_LEFT) sprite->changeAnimation(JUMP_LEFT);
			if (sprite->animation() == JUMP_RIGHT) sprite->changeAnimation(JUMP_RIGHT);
		}
		if (jumpAngle == height_air) //FIN DEL SALTO
		{
			bJumping = false;
			//posPlayer.y = startY;
		}
		else //SIGUE SALTANDO
		{
			//if (sprite->animation() == START_JUMP_RIGHT /*&& sprite->checkChangeAnimation(START_JUMP_RIGHT)*/) {
			posPlayer.y = int(startY - JUMP_HEIGHT * sin(3.14159f * jumpAngle / 180.f));
			if (jumpAngle > 90)
				bJumping = !map->collisionMoveDown(posPlayer, glm::ivec2(32, 72), &posPlayer.y);
			//}
		}
	}
	else if (climbing) {
		if (sprite->animation() == CLIMB_RIGHT || sprite->animation() == CLIMB_LEFT || sprite->animation() == JUMP_RIGHT || sprite->animation() == JUMP_LEFT){
			jumpAngle += JUMP_ANGLE_STEP;
			//if (sprite->animation() != CLIMB_RIGHT) sprite->changeAnimation(CLIMB_RIGHT);
			//if (sprite->animation() == JUMP_RIGHT) sprite->changeAnimation(JUMP_RIGHT);
			//printf("JumpAngle:%d ", jumpAngle);
		}
		if (jumpAngle == 480) //FIN DEL SALTO
		{
			climbing = false;
			posPlayer.y = startY;
		}
		else //SIGUE SALTANDO
		{
			
			int pos = int(startY - (JUMP_HEIGHT+16) * sin(3.14159f * jumpAngle / 180.f));
			if (pos <= posPlayer.y) posPlayer.y = pos;
			else{
				if (sprite->checkChangeAnimation(CLIMB_RIGHT)) {
					posPlayer.y = startY - 64;
					sprite->changeAnimation(STAND_RIGHT);
				}
			}
			if (jumpAngle > 180)
				climbing = !map->collisionMoveDown(posPlayer, glm::ivec2(32, 72), &posPlayer.y);

		}
	}
	else
	{	
		if (sprite->animation() != START_FALL_RIGHT && sprite->isFalling()) 
			sprite->changeAnimation(START_FALL_RIGHT);
		posPlayer.y += FALL_STEP;
		if(map->collisionMoveDown(posPlayer, glm::ivec2(32, 72), &posPlayer.y))
		{
			if (Game::instance().getSpecialKey(GLUT_KEY_UP))
			{
				bJumping = true;
				jumpAngle = 0;
				startY = posPlayer.y;
				if (sprite->animation() == STAND_RIGHT) sprite->changeAnimation(START_JUMP_RIGHT);
				else if (sprite->animation() == STAND_LEFT) sprite->changeAnimation(START_JUMP_LEFT);
				if (!map->collisionMoveUp(posPlayer, glm::ivec2(32, 64), &posPlayer.y)) {
					height_air = 180;
				}
				else {
					if (map->collisionClimb(posPlayer, glm::ivec2(32, 64), &posPlayer.y, true)) {
						climbing = true;
						bJumping = false;
					}
					/*if (sprite->animation() == START_JUMP_RIGHT){
						if (map->collisionClimb(posPlayer, glm::ivec2(32, 72), &posPlayer.y, true))
							sprite->changeAnimation(CLIMB_RIGHT);
					}
					else if (sprite->animation() == START_JUMP_LEFT){
						if (map->collisionClimb(posPlayer, glm::ivec2(32, 72), &posPlayer.y, false))
							sprite->changeAnimation(CLIMB_LEFT);
					}*/
					height_air = 48;
					//if (sprite->animation() == STAND_RIGHT) sprite->changeAnimation(CLIMB_RIGHT);
					//else if (sprite->animation() == STAND_LEFT) sprite->changeAnimation(CLIMB_LEFT);
				}

			}
			else if (sprite->isFalling()) {
				// Amortiguar caída
				sprite->changeAnimation(FALL_RIGHT);
			}
			sprite->falling(false);
		}
		else {
			sprite->falling(true);
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




