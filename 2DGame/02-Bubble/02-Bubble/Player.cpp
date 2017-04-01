#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 16
#define FALL_STEP 2
#define SHIFT 112

enum PlayerAnims
{
	STAND_RIGHT, STAND_LEFT, MOVE_LEFT, MOVE_RIGHT, START_RIGHT, STOP_RIGHT, START_LEFT, STOP_LEFT, FALL_RIGHT, FALL_LEFT, START_FALL_RIGHT,
	START_FALL_LEFT, MOVE_TO_LEFT, MOVE_TO_RIGHT, MOVE_TO_LEFT_RUNNING, MOVE_TO_RIGHT_RUNNING, START_JUMP_RIGHT, JUMP_RIGHT, JUMP_FALL_RIGHT,
	JUMP_LEFT, START_JUMP_LEFT, JUMP_FALL_LEFT, MOVE_STEP_LEFT, MOVE_STEP_RIGHT, CLIMB_RIGHT, CLIMB_LEFT, LONG_JUMP_RIGHT, LONG_JUMP_LEFT,
	FALL_DOWN_RIGHT, FALL_DOWN_LEFT, STAND_UP_RIGHT, STAND_UP_LEFT
};


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	bJumping = false;
	jump_long = false;
	climbing = false;
	agachado = false;
	face_direction = true;
	distancia = 64;
	spritesheet.loadFromFile("images/prince-sprite.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(64, 64), glm::vec2(0.05, 0.05), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(32);
	
	sprite->setAnimationSpeed(STAND_LEFT, 8);
	sprite->addKeyframe(STAND_LEFT, glm::vec2(0.f, 0.1f));
	sprite->setSpeed(STAND_LEFT, glm::vec2(0, 0));
		
	sprite->setAnimationSpeed(STAND_RIGHT, 8);
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.f, 0.f));
	sprite->setSpeed(STAND_RIGHT, glm::vec2(0, 0));
		
	sprite->setAnimationSpeed(MOVE_LEFT, 8);
	for (float i = 6; i > 3; --i) {
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.0f + (i / 20.0f),0.15f ));
		sprite->setSpeed(MOVE_LEFT, glm::vec2(-2, 0));
	}
	for (float i = 4; i <= 6; ++i) {
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.0f + (i / 20.0f), 0.15f));
		sprite->setSpeed(MOVE_LEFT, glm::vec2(-2, 0));
	}
		
	sprite->setAnimationSpeed(MOVE_RIGHT, 8);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.15f, 0.05f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.2f, 0.05f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25f, 0.05f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.2f, 0.05f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.15f, 0.05f));
	sprite->setSpeed(MOVE_RIGHT, glm::vec2(2, 0));
	sprite->setSpeed(MOVE_RIGHT, glm::vec2(2, 0));
	sprite->setSpeed(MOVE_RIGHT, glm::vec2(2, 0));
	sprite->setSpeed(MOVE_RIGHT, glm::vec2(2, 0));
	sprite->setSpeed(MOVE_RIGHT, glm::vec2(2, 0));

	sprite->setAnimationSpeed(START_RIGHT, 8);
	sprite->addKeyframe(START_RIGHT, glm::vec2(0.0f, 0.05f));
	sprite->addKeyframe(START_RIGHT, glm::vec2(0.05f, 0.05f));
	sprite->addKeyframe(START_RIGHT, glm::vec2(0.1f, 0.05f));
	sprite->setSpeed(START_RIGHT, glm::vec2(0, 0));
	sprite->setSpeed(START_RIGHT, glm::vec2(1, 0));
	sprite->setSpeed(START_RIGHT, glm::vec2(1, 0));

	sprite->setAnimationSpeed(START_LEFT, 8);
	sprite->addKeyframe(START_LEFT, glm::vec2(0.45f, 0.15f));
	sprite->addKeyframe(START_LEFT, glm::vec2(0.4f, 0.15f));
	sprite->addKeyframe(START_LEFT, glm::vec2(0.35f, 0.15f));
	sprite->setSpeed(START_LEFT, glm::vec2(-1, 0));
	sprite->setSpeed(START_LEFT, glm::vec2(-1, 0));
	sprite->setSpeed(START_LEFT, glm::vec2(-1, 0));

	sprite->setAnimationSpeed(STOP_RIGHT, 8);
	sprite->addKeyframe(STOP_RIGHT, glm::vec2(0.35f, 0.05f));
	sprite->addKeyframe(STOP_RIGHT, glm::vec2(0.4f, 0.05f));
	sprite->addKeyframe(STOP_RIGHT, glm::vec2(0.45f, 0.05f));
	sprite->addKeyframe(STOP_RIGHT, glm::vec2(0.f, 0.f));
	sprite->setSpeed(STOP_RIGHT, glm::vec2(1, 0));
	sprite->setSpeed(STOP_RIGHT, glm::vec2(1, 0));
	sprite->setSpeed(STOP_RIGHT, glm::vec2(1, 0));
	sprite->setSpeed(STOP_RIGHT, glm::vec2(1, 0));

	sprite->setAnimationSpeed(STOP_LEFT, 8);
	sprite->addKeyframe(STOP_LEFT, glm::vec2(0.1f, 0.15f));
	sprite->addKeyframe(STOP_LEFT, glm::vec2(0.05f, 0.15f));
	sprite->addKeyframe(STOP_LEFT, glm::vec2(0.0f, 0.15f));
	sprite->addKeyframe(STOP_LEFT, glm::vec2(0.f, 0.1f));
	sprite->setSpeed(STOP_LEFT, glm::vec2(-1, 0));
	sprite->setSpeed(STOP_LEFT, glm::vec2(-1, 0));
	sprite->setSpeed(STOP_LEFT, glm::vec2(-1, 0));
	sprite->setSpeed(STOP_LEFT, glm::vec2(-1, 0));

	sprite->setAnimationSpeed(FALL_DOWN_RIGHT, 6);
	for (float i = 1; i < 5; ++i) {
		sprite->addKeyframe(FALL_DOWN_RIGHT, glm::vec2(0.0f + (i / 20.0f), 0.25f));
		sprite->setSpeed(FALL_DOWN_RIGHT, glm::vec2(0, 0));
	}

	sprite->setAnimationSpeed(FALL_DOWN_LEFT, 6);
	for (float i = 18; i >= 15; --i) {
		sprite->addKeyframe(FALL_DOWN_LEFT, glm::vec2(0.0f + (i / 20.0f), 0.25f));
		sprite->setSpeed(FALL_DOWN_LEFT, glm::vec2(0, 0));
	}

	sprite->setAnimationSpeed(STAND_UP_RIGHT, 6);
	for (float i = 5; i < 10; ++i) {
		sprite->addKeyframe(STAND_UP_RIGHT, glm::vec2(0.0f + (i / 20.0f), 0.25f));
		sprite->setSpeed(STAND_UP_RIGHT, glm::vec2(0, 0));
	}

	sprite->setAnimationSpeed(STAND_UP_LEFT, 6);
	for (float i = 14; i >= 10; --i) {
		sprite->addKeyframe(STAND_UP_LEFT, glm::vec2(0.0f + (i / 20.0f), 0.25f));
		sprite->setSpeed(STAND_UP_LEFT, glm::vec2(0, 0));
	}

	sprite->setAnimationSpeed(FALL_RIGHT, 8);
	for (float i = 2; i < 10; ++i) {
		sprite->addKeyframe(FALL_RIGHT, glm::vec2(0.0f + (i / 20.0f), 0.25f));
		sprite->setSpeed(FALL_RIGHT, glm::vec2(0, 0));
	}

	sprite->setAnimationSpeed(FALL_LEFT, 8);
	for (float i = 17; i >= 10; --i) {
		sprite->addKeyframe(FALL_LEFT, glm::vec2(0.0f + (i / 20.0f), 0.25f));
		sprite->setSpeed(FALL_LEFT, glm::vec2(0, 0));
	}

	sprite->setAnimationSpeed(START_FALL_RIGHT, 8);
	for (float i = 0; i <= 4; ++i) {
		sprite->addKeyframe(START_FALL_RIGHT, glm::vec2(0.0f + (i / 20.0f), 0.2f));
		sprite->setSpeed(START_FALL_RIGHT, glm::vec2(0, 0));
	}

	sprite->setAnimationSpeed(START_FALL_LEFT, 8);
	for (float i = 19; i >= 16; --i) {
		sprite->addKeyframe(START_FALL_LEFT, glm::vec2(0.0f + (i / 20.0f), 0.2f));
		sprite->setSpeed(START_FALL_LEFT, glm::vec2(0, 0));
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

	sprite->setAnimationSpeed(MOVE_TO_LEFT_RUNNING, 8);
	for (float i = 0; i < 10; ++i) {
		sprite->addKeyframe(MOVE_TO_LEFT_RUNNING, glm::vec2(0.0f + (i / 20.0f), 0.55f));
		sprite->setSpeed(MOVE_TO_LEFT_RUNNING, glm::vec2(1, 0));
	}

	sprite->setAnimationSpeed(MOVE_TO_RIGHT_RUNNING, 8);
	for (float i = 19; i >= 10; --i) {
		sprite->addKeyframe(MOVE_TO_RIGHT_RUNNING, glm::vec2(0.0f + (i / 20.0f), 0.55f));
		sprite->setSpeed(MOVE_TO_RIGHT_RUNNING, glm::vec2(-1, 0));
	}

	sprite->setAnimationSpeed(START_JUMP_RIGHT, 8);
	for (float i = 0; i < 8; ++i) {
		sprite->addKeyframe(START_JUMP_RIGHT, glm::vec2(0.0f + (i / 20.0f), 0.6f));
		sprite->setSpeed(START_JUMP_RIGHT, glm::vec2(0, 0));
	}

	sprite->setAnimationSpeed(JUMP_RIGHT, 8);
	for (float i = 8; i < 10; ++i) {
		sprite->addKeyframe(JUMP_RIGHT, glm::vec2(0.0f + (i / 20.0f), 0.6f));
		sprite->setSpeed(JUMP_RIGHT, glm::vec2(0, 1));
	}

	sprite->setAnimationSpeed(START_JUMP_LEFT, 8);
	for (float i = 19; i >= 12; --i) {
		sprite->addKeyframe(START_JUMP_LEFT, glm::vec2(0.0f + (i / 20.0f), 0.6f));
		sprite->setSpeed(START_JUMP_LEFT, glm::vec2(0, 0));
	}

	sprite->setAnimationSpeed(JUMP_LEFT, 8);
	for (float i = 11; i >= 10; --i) {
		sprite->addKeyframe(JUMP_LEFT, glm::vec2(0.0f + (i / 20.0f), 0.6f));
		sprite->setSpeed(JUMP_LEFT, glm::vec2(0, 1));
	}

	sprite->setAnimationSpeed(MOVE_STEP_LEFT, 8);
	for (float i = 19; i >= 12; --i) {
		sprite->addKeyframe(MOVE_STEP_LEFT, glm::vec2(0.0f + (i / 20.0f), 0.75f));
		if (i > 16) sprite->setSpeed(MOVE_STEP_LEFT, glm::vec2(0, 0));
		else sprite->setSpeed(MOVE_STEP_LEFT, glm::vec2(-1, 0));
	}

	sprite->setAnimationSpeed(MOVE_STEP_RIGHT, 8);
	for (float i = 0; i < 8; ++i) {
		sprite->addKeyframe(MOVE_STEP_RIGHT, glm::vec2(0.0f + (i / 20.0f), 0.75f));
		if (i < 3) sprite->setSpeed(MOVE_STEP_RIGHT, glm::vec2(0, 0));
		else sprite->setSpeed(MOVE_STEP_RIGHT, glm::vec2(1, 0));
	}

	sprite->setAnimationSpeed(LONG_JUMP_RIGHT, 10);
	for (float i = 0; i < 5; ++i) {
		sprite->addKeyframe(LONG_JUMP_RIGHT, glm::vec2(0.0f + (i / 20.0f), 0.9f));
		sprite->setSpeed(LONG_JUMP_RIGHT, glm::vec2(2, 0));
	}
	for (float i = 4; i < 6; ++i) {
		sprite->addKeyframe(LONG_JUMP_RIGHT, glm::vec2(0.0f + (i / 20.0f), 0.9f));
		sprite->setSpeed(LONG_JUMP_RIGHT, glm::vec2(2, 0));
	}
	sprite->addKeyframe(LONG_JUMP_RIGHT, glm::vec2(0.0f + (6.0f / 20.0f), 0.9f));
	sprite->addKeyframe(LONG_JUMP_RIGHT, glm::vec2(0.0f + (7.0f / 20.0f), 0.9f));
	sprite->addKeyframe(LONG_JUMP_RIGHT, glm::vec2(0.0f + (8.0f / 20.0f), 0.9f));
	sprite->addKeyframe(LONG_JUMP_RIGHT, glm::vec2(0.0f + (9.0f / 20.0f), 0.9f));
	sprite->setSpeed(LONG_JUMP_RIGHT, glm::vec2(2, 0));
	sprite->setSpeed(LONG_JUMP_RIGHT, glm::vec2(2, 0));
	sprite->setSpeed(LONG_JUMP_RIGHT, glm::vec2(2, 0));
	sprite->setSpeed(LONG_JUMP_RIGHT, glm::vec2(2, 0));

	sprite->setAnimationSpeed(LONG_JUMP_LEFT, 10);
	for (float i = 19; i > 14; --i) {
		sprite->addKeyframe(LONG_JUMP_LEFT, glm::vec2(0.0f + (i / 20.0f), 0.9f));
		sprite->setSpeed(LONG_JUMP_LEFT, glm::vec2(-2, 0));
	}
	for (float i = 15; i > 13; --i) {
		sprite->addKeyframe(LONG_JUMP_LEFT, glm::vec2(0.0f + (i / 20.0f), 0.9f));
		sprite->setSpeed(LONG_JUMP_LEFT, glm::vec2(-2, 0));
	}
	sprite->addKeyframe(LONG_JUMP_LEFT, glm::vec2(0.0f + (13.0f / 20.0f), 0.9f));
	sprite->addKeyframe(LONG_JUMP_LEFT, glm::vec2(0.0f + (12.0f / 20.0f), 0.9f));
	sprite->addKeyframe(LONG_JUMP_LEFT, glm::vec2(0.0f + (11.0f / 20.0f), 0.9f));
	sprite->addKeyframe(LONG_JUMP_LEFT, glm::vec2(0.0f + (10.0f / 20.0f), 0.9f));
	sprite->setSpeed(LONG_JUMP_LEFT, glm::vec2(-2, 0));
	sprite->setSpeed(LONG_JUMP_LEFT, glm::vec2(-2, 0));
	sprite->setSpeed(LONG_JUMP_LEFT, glm::vec2(-2, 0));
	sprite->setSpeed(LONG_JUMP_LEFT, glm::vec2(-2, 0));

	sprite->setAnimationSpeed(JUMP_FALL_RIGHT, 8);
	sprite->addKeyframe(JUMP_FALL_RIGHT, glm::vec2(0.4f, 0.75f));
	sprite->addKeyframe(JUMP_FALL_RIGHT, glm::vec2(0.45f, 0.75f));
	sprite->setSpeed(JUMP_FALL_RIGHT, glm::vec2(0, -1));
	sprite->setSpeed(JUMP_FALL_RIGHT, glm::vec2(0, -1));

	sprite->setAnimationSpeed(JUMP_FALL_LEFT, 8);
	sprite->addKeyframe(JUMP_FALL_LEFT, glm::vec2(0.55f, 0.75f));
	sprite->addKeyframe(JUMP_FALL_LEFT, glm::vec2(0.5f, 0.75f));
	sprite->setSpeed(JUMP_FALL_LEFT, glm::vec2(0, -1));
	sprite->setSpeed(JUMP_FALL_LEFT, glm::vec2(0, -1));

	sprite->setAnimationSpeed(CLIMB_RIGHT, 8);
	for (float i = 0; i < 9; ++i) {
		sprite->addKeyframe(CLIMB_RIGHT, glm::vec2(0.0f + (i / 20.0f), 0.65f));
		sprite->setSpeed(CLIMB_RIGHT, glm::vec2(0, 0));
	}

	sprite->setAnimationSpeed(CLIMB_LEFT, 8);
	for (float i = 19; i >= 10; --i) {
		sprite->addKeyframe(CLIMB_LEFT, glm::vec2(0.0f + (i / 20.0f), 0.65f));
		sprite->setSpeed(CLIMB_LEFT, glm::vec2(0, 0));
	}
		
	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + 64), float(tileMapDispl.y + 0)));
	
}

void Player::update(int deltaTime)
{
	printf("%d\n",sprite->animation());
	sprite->update(deltaTime);
	if (strcmp(Scene::instance().getLevel().c_str(),"levels/prince-map1.txt") == 0) {
		if (posPlayer.x >= 249 && posPlayer.y >= 120) {
			Scene::instance().setLevel("levels/prince-map2.txt", glm::vec2(0, 120));
		}
		else if (posPlayer.x >= 208 && posPlayer.y >= 120) {
			Scene::instance().setLevel("levels/prince-map3.txt", glm::vec2(208, 0));
		}
	}
	else if (strcmp(Scene::instance().getLevel().c_str(), "levels/prince-map3.txt") == 0) {
		if (posPlayer.x >= 314 && posPlayer.y >= 56) {
			Scene::instance().setLevel("levels/prince-map4.txt", glm::vec2(6, 56));
		}
	}
	else if (strcmp(Scene::instance().getLevel().c_str(), "levels/prince-map4.txt") == 0) {
		if (posPlayer.x <= 5 && posPlayer.y >= 56) {
			Scene::instance().setLevel("levels/prince-map3.txt", glm::vec2(313, 56));
		}
	}
	if (Game::instance().getSpecialKey(GLUT_KEY_LEFT))
	{
		if (!bJumping && !climbing && distancia == 0 && sprite->animation() != FALL_RIGHT  && sprite->animation() != FALL_LEFT && sprite->animation() != LONG_JUMP_RIGHT && sprite->animation() != LONG_JUMP_LEFT){
			if (sprite->animation() == MOVE_RIGHT || sprite->animation() == STOP_RIGHT) {
				sprite->changeAnimation(MOVE_TO_LEFT_RUNNING);
				face_direction = false;
			}
			if (sprite->animation() == MOVE_TO_LEFT_RUNNING) {
				if (sprite->checkChangeAnimation(MOVE_TO_LEFT_RUNNING)) {
					sprite->changeAnimation(MOVE_LEFT);
				}
			}
			if (sprite->animation() == STAND_RIGHT) {
				sprite->changeAnimation(MOVE_TO_LEFT);
				posPlayer.x += 2;
				face_direction = false;
			}
			if (sprite->animation() == MOVE_TO_LEFT) {
				if (sprite->checkChangeAnimation(MOVE_TO_LEFT)) {
					sprite->changeAnimation(STAND_LEFT);
				}
			}
			if ((sprite->animation() != START_LEFT && sprite->animation() != MOVE_LEFT) && sprite->animation() != MOVE_TO_LEFT && sprite->animation() != MOVE_TO_LEFT_RUNNING) {	// Si aún no ha empezado a moverse y no estaba corriendo -> Empezamos a correr
				if (Game::instance().getSpecialKey(SHIFT)) {
					sprite->changeAnimation(MOVE_STEP_LEFT);
				}
				else sprite->changeAnimation(START_LEFT);
				face_direction = false;
			}
			if (sprite->animation() == START_LEFT) {
				if (sprite->checkChangeAnimation(START_LEFT))	  // Si ha empezado a correr y por el tiempo pasado ya puede hacer el ciclo de correr -> Cambiamos animación
					sprite->changeAnimation(MOVE_LEFT);
			}
			if (map->collisionMoveLeft(posPlayer, glm::ivec2(24, 64)))
			{
				sprite->changeAnimation(STAND_LEFT);
			}
			else if (map->collisionMoveRight(posPlayer, glm::ivec2(38, 64)) && sprite->animation() == MOVE_TO_LEFT_RUNNING)
			{
				sprite->changeAnimation(STAND_RIGHT);
				face_direction = true;
			}
		}
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
	{
		if (!bJumping && !climbing && distancia == 0 && sprite->animation() != FALL_RIGHT  && sprite->animation() != FALL_LEFT && sprite->animation() != LONG_JUMP_RIGHT && sprite->animation() != LONG_JUMP_LEFT )
		{
			if (sprite->animation() == MOVE_LEFT || sprite->animation() == STOP_LEFT) {
				sprite->changeAnimation(MOVE_TO_RIGHT_RUNNING);
				face_direction = true;
			}
			if (sprite->animation() == MOVE_TO_RIGHT_RUNNING) {
				if (sprite->checkChangeAnimation(MOVE_TO_RIGHT_RUNNING)) {
					sprite->changeAnimation(MOVE_RIGHT);
				}
			}
			if (sprite->animation() == STAND_LEFT) {
				sprite->changeAnimation(MOVE_TO_RIGHT);
				face_direction = true;
			}
			if (sprite->animation() == MOVE_TO_RIGHT) {
				if (sprite->checkChangeAnimation(MOVE_TO_RIGHT)) {
					sprite->changeAnimation(STAND_RIGHT);
				}
			}
			if (sprite->animation() == STAND_RIGHT) {
				if (Game::instance().getSpecialKey(SHIFT)) {
					sprite->changeAnimation(MOVE_STEP_RIGHT);
				}
				else sprite->changeAnimation(START_RIGHT);
				face_direction = true;
			}
			if (sprite->animation() == START_RIGHT) {
				if (sprite->checkChangeAnimation(START_RIGHT)){
					sprite->changeAnimation(MOVE_RIGHT);
				}
			}
			if (map->collisionMoveRight(posPlayer, glm::ivec2(38, 64)))
			{
				sprite->changeAnimation(STAND_RIGHT);
			}
			else if (map->collisionMoveLeft(posPlayer, glm::ivec2(24, 64)) && sprite->animation() == MOVE_TO_RIGHT_RUNNING)
			{
				sprite->changeAnimation(STAND_LEFT);
				face_direction = false;
			}
		}
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_DOWN)){
		if (!bJumping && !climbing && !agachado){
			agachado = true;
			if (face_direction) sprite->changeAnimation(FALL_DOWN_RIGHT);
			else sprite->changeAnimation(FALL_DOWN_LEFT);
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
			if (sprite->checkChangeAnimation(STOP_RIGHT))
				sprite->changeAnimation(STAND_RIGHT);
		}
		else if (sprite->animation() == STOP_LEFT) {
			if (sprite->checkChangeAnimation(STOP_LEFT))
				sprite->changeAnimation(STAND_LEFT);
		}
		else if (sprite->animation() == FALL_RIGHT) {
			if (sprite->checkChangeAnimation(FALL_RIGHT)){
				sprite->changeAnimation(STAND_RIGHT);
			}
		}
		else if (sprite->animation() == FALL_LEFT) {
			if (sprite->checkChangeAnimation(FALL_LEFT))
				sprite->changeAnimation(STAND_LEFT);
		}
		else if (sprite->animation() == FALL_DOWN_RIGHT) {
			if (sprite->checkChangeAnimation(FALL_DOWN_RIGHT)){
				if(!Game::instance().getSpecialKey(GLUT_KEY_DOWN)) sprite->changeAnimation(STAND_UP_RIGHT);
				agachado = false;
			}
		}
		else if (sprite->animation() == FALL_DOWN_LEFT) {
			if (sprite->checkChangeAnimation(FALL_DOWN_LEFT)){
				if (!Game::instance().getSpecialKey(GLUT_KEY_DOWN))sprite->changeAnimation(STAND_UP_LEFT);
				agachado = false;
			}
		}
		else if (sprite->animation() == STAND_UP_RIGHT) {
			if (sprite->checkChangeAnimation(STAND_UP_RIGHT)){
				sprite->changeAnimation(STAND_RIGHT);
			}
		}
		else if (sprite->animation() == STAND_UP_LEFT) {
			if (sprite->checkChangeAnimation(STAND_UP_LEFT)){
				sprite->changeAnimation(STAND_LEFT);
			}
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
		else if (sprite->animation() == START_FALL_LEFT) {
			if (sprite->checkChangeAnimation(START_FALL_LEFT))
				sprite->changeAnimation(FALL_LEFT);
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
				if (!climbing)sprite->changeAnimation(JUMP_FALL_RIGHT);
				else sprite->changeAnimation(CLIMB_RIGHT);
			}
		}
		else if (sprite->animation() == JUMP_FALL_RIGHT) {
			if (sprite->checkChangeAnimation(JUMP_FALL_RIGHT))
				sprite->changeAnimation(STAND_RIGHT);
		}
		else if (sprite->animation() == JUMP_LEFT) {
			if (sprite->checkChangeAnimation(JUMP_LEFT)){
				if (climbing) sprite->changeAnimation(CLIMB_LEFT);
				else sprite->changeAnimation(JUMP_FALL_LEFT);
			}
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
		else if (sprite->animation() == MOVE_STEP_RIGHT) {
			if (sprite->checkChangeAnimation(MOVE_STEP_RIGHT)) {
				sprite->changeAnimation(STAND_RIGHT);
			}
		}
		else if (sprite->animation() == MOVE_STEP_LEFT) {
			if (sprite->checkChangeAnimation(MOVE_STEP_LEFT)) {
				sprite->changeAnimation(STAND_LEFT);
			}
		}
		else if (sprite->animation() == LONG_JUMP_RIGHT) {
			if (sprite->checkChangeAnimation(LONG_JUMP_RIGHT)) {
				sprite->changeAnimation(MOVE_RIGHT);
			}
		}
		else if (sprite->animation() == LONG_JUMP_LEFT) {
			if (sprite->checkChangeAnimation(LONG_JUMP_LEFT)) {
				sprite->changeAnimation(MOVE_LEFT);
			}
		}
	}
	if (bJumping)
	{
		if (sprite->animation() == JUMP_RIGHT || sprite->animation() == JUMP_LEFT){
			jumpAngle += JUMP_ANGLE_STEP;
			if (!face_direction) sprite->changeAnimation(JUMP_LEFT);
			else sprite->changeAnimation(JUMP_RIGHT);
		}
		if (jumpAngle == height_air) //FIN DEL SALTO
		{
			bJumping = false;
		}
		else //SIGUE SALTANDO
		{
			posPlayer.y = int(startY - JUMP_HEIGHT * sin(3.14159f * jumpAngle / 180.f));
			if (jumpAngle > 90)
				bJumping = !map->collisionMoveDown(posPlayer, glm::ivec2(32, 8), &posPlayer.y);
		}
	}
	else if (climbing) {
		if (sprite->animation() == CLIMB_RIGHT || sprite->animation() == CLIMB_LEFT || sprite->animation() == JUMP_RIGHT || sprite->animation() == JUMP_LEFT){
			jumpAngle += JUMP_ANGLE_STEP;
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
				else if (sprite->checkChangeAnimation(CLIMB_LEFT)) {
					posPlayer.y = startY - 64;
					posPlayer.x -= 16;
					sprite->changeAnimation(STAND_LEFT);
				}
			}
			if (jumpAngle > 180){
				if (face_direction) climbing = !map->collisionMoveDown(posPlayer, glm::ivec2(32, 8), &posPlayer.y);
				else climbing = !map->collisionMoveDown(posPlayer, glm::ivec2(32, 9), &posPlayer.y);
			}

		}
	}
	/*else if (agachado){
		if (sprite->animation() == FALL_DOWN_LEFT || sprite->animation() == FALL_DOWN_RIGHT){
			if (sprite->check)
		}
	}*/
	else
	{	
		if (distancia > 2 && sprite->animation() != START_FALL_RIGHT && sprite->animation() != START_FALL_LEFT && sprite->animation() != FALL_RIGHT && sprite->animation() != FALL_LEFT) {
			if (face_direction)
				sprite->changeAnimation(START_FALL_RIGHT);
			else sprite->changeAnimation(START_FALL_LEFT);
		}
		posPlayer.y += FALL_STEP;
		if(map->collisionMoveDown(posPlayer, glm::ivec2(32, 8), &posPlayer.y))
		{
			distancia = 0;
			if (Game::instance().getSpecialKey(GLUT_KEY_UP))
			{
				if (sprite->animation() == MOVE_RIGHT || sprite->animation() == LONG_JUMP_RIGHT){
						sprite->changeAnimation(LONG_JUMP_RIGHT);
				}
				else if (sprite->animation() == MOVE_LEFT || sprite->animation() == LONG_JUMP_LEFT) {
					sprite->changeAnimation(LONG_JUMP_LEFT);
				}
				else {
					bJumping = true;
					distancia = 0;
					jumpAngle = 0;
					startY = posPlayer.y;
					if (!map->collisionMoveUp(posPlayer, glm::ivec2(32, 64), &posPlayer.y, face_direction)) {
						height_air = 180;
					}
					else {
						height_air = 48;
						distancia -= 10;
						if (face_direction){
							if (map->collisionClimb(posPlayer, glm::ivec2(32, 64), &posPlayer.y, true)) {
								climbing = true;
								bJumping = false;
								int posX = posPlayer.x % 32;
								if (posX > 17) posPlayer.x = posPlayer.x - (posX - 17);
								else if (posX < 17) posPlayer.x = posPlayer.x + (17 - posX);//else 
							}
						}
						else{
							if (map->collisionClimb(posPlayer, glm::ivec2(32, 64), &posPlayer.y, false)) {
								climbing = true;
								bJumping = false;
								int posX = (posPlayer.x % 32);
								if (posX > 17) posPlayer.x = posPlayer.x - (posX - 17);
								else if (posX < 17) posPlayer.x = posPlayer.x + (17 - posX);//else 
							}
						}
					}
					if (face_direction) sprite->changeAnimation(START_JUMP_RIGHT);
					else sprite->changeAnimation(START_JUMP_LEFT);
				}
			}
			else if (distancia >= 58) {
				// Amortiguar caída
				if (face_direction) sprite->changeAnimation(FALL_RIGHT);
				else sprite->changeAnimation(FALL_LEFT);
				distancia = 0;
			}
		}
		else {
			distancia += FALL_STEP;
		}
	}
	posPlayer.x += sprite->getSpeed().x;
	posPlayer.y += sprite->getSpeed().y;
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




