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
	FALL_DOWN_RIGHT, FALL_DOWN_LEFT, STAND_UP_RIGHT, STAND_UP_LEFT, AGACHADO_RIGHT, AGACHADO_LEFT, DIED_RIGHT, DIED_LEFT, DIED_FALL_RIGHT, DIED_FALL_LEFT,
	FALLING_LEFT, FALLING_RIGHT, DIED_FINISH_RIGHT, DIED_FINISH_LEFT, GET_SWORD_RIGHT, GET_SWORD_LEFT
};


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	bJumping = false;
	climbing = false;
	agachado = false;
	sword = false;
	music_collision_over = false;
	music_collision = false;
	face_direction = true;
	level = 1;
	distancia = 64;
	hp = 3;
	spritesheet.loadFromFile("images/prince-sprite.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(64, 64), glm::vec2(0.05, 0.05), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(44);
	
	sprite->setAnimationSpeed(STAND_LEFT, 8);
	sprite->addKeyframe(STAND_LEFT, glm::vec2(0.f, 0.1f));
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
	
	sprite->setAnimationSpeed(DIED_FALL_LEFT, 8);
	for (float i = 17; i >= 12; --i) {
		sprite->addKeyframe(DIED_FALL_LEFT, glm::vec2(0.0f + (i / 20.0f), 0.0f));
		sprite->setSpeed(DIED_FALL_LEFT, glm::vec2(0, 0));
	}
	sprite->setAnimationSpeed(DIED_FINISH_LEFT,8);
	sprite->addKeyframe(DIED_FINISH_LEFT, glm::vec2(13.0f/ 20.0f, 0.0f));
	sprite->setSpeed(DIED_FINISH_LEFT, glm::vec2(0, 0));

	sprite->setAnimationSpeed(DIED_FALL_RIGHT, 8);
	for (float i = 2; i < 8; ++i){	
		sprite->addKeyframe(DIED_FALL_RIGHT, glm::vec2(0.0f + (i / 20.0f), 0.0f));
		sprite->setSpeed(DIED_FALL_RIGHT, glm::vec2(0, 0));
	}

	sprite->setAnimationSpeed(DIED_FINISH_RIGHT, 8);
	sprite->addKeyframe(DIED_FINISH_RIGHT, glm::vec2(6.0f / 20.0f, 0.0f));
	sprite->setSpeed(DIED_FINISH_RIGHT, glm::vec2(0, 0));

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

	sprite->setAnimationSpeed(AGACHADO_RIGHT, 6);
	sprite->addKeyframe(AGACHADO_RIGHT, glm::vec2(0.15f, 0.25f));
	sprite->setSpeed(AGACHADO_RIGHT, glm::vec2(0, 0));

	sprite->setAnimationSpeed(FALL_DOWN_LEFT, 6);
	for (float i = 18; i >= 15; --i) {
		sprite->addKeyframe(FALL_DOWN_LEFT, glm::vec2(0.0f + (i / 20.0f), 0.25f));
		sprite->setSpeed(FALL_DOWN_LEFT, glm::vec2(0, 0));
	}

	sprite->setAnimationSpeed(AGACHADO_LEFT, 6);
	sprite->addKeyframe(AGACHADO_LEFT, glm::vec2(0.8f, 0.25f));
	sprite->setSpeed(AGACHADO_LEFT, glm::vec2(0, 0));

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
	for (float i = 0; i < 4; ++i) {
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
	for (float i = 0; i < 6; ++i) {
		sprite->addKeyframe(MOVE_TO_LEFT_RUNNING, glm::vec2(0.0f + (i / 20.0f), 0.55f));
		sprite->setSpeed(MOVE_TO_LEFT_RUNNING, glm::vec2(1, 0));
	}
	for (float i = 6; i < 10; ++i) {
		sprite->addKeyframe(MOVE_TO_LEFT_RUNNING, glm::vec2(0.0f + (i / 20.0f), 0.55f));
		sprite->setSpeed(MOVE_TO_LEFT_RUNNING, glm::vec2(0, 0));
	}

	sprite->setAnimationSpeed(MOVE_TO_RIGHT_RUNNING, 8);
	for (float i = 19; i >= 14; --i) {
		sprite->addKeyframe(MOVE_TO_RIGHT_RUNNING, glm::vec2(0.0f + (i / 20.0f), 0.55f));
		sprite->setSpeed(MOVE_TO_RIGHT_RUNNING, glm::vec2(-1, 0));
	}
	for (float i = 13; i >= 10; --i) {
		sprite->addKeyframe(MOVE_TO_RIGHT_RUNNING, glm::vec2(0.0f + (i / 20.0f), 0.55f));
		sprite->setSpeed(MOVE_TO_RIGHT_RUNNING, glm::vec2(0, 0));
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

	sprite->addKeyframe(LONG_JUMP_RIGHT, glm::vec2(0.0f + (5.0f / 20.0f), 0.9f));
	sprite->addKeyframe(LONG_JUMP_RIGHT, glm::vec2(0.0f + (6.0f / 20.0f), 0.9f));
	sprite->addKeyframe(LONG_JUMP_RIGHT, glm::vec2(0.0f + (7.0f / 20.0f), 0.9f));
	sprite->addKeyframe(LONG_JUMP_RIGHT, glm::vec2(0.0f + (8.0f / 20.0f), 0.9f));
	sprite->addKeyframe(LONG_JUMP_RIGHT, glm::vec2(0.0f + (9.0f / 20.0f), 0.9f));
	sprite->setSpeed(LONG_JUMP_RIGHT, glm::vec2(2, -1));
	sprite->setSpeed(LONG_JUMP_RIGHT, glm::vec2(4, 0));
	sprite->setSpeed(LONG_JUMP_RIGHT, glm::vec2(2, 1));
	sprite->setSpeed(LONG_JUMP_RIGHT, glm::vec2(2, 0));
	sprite->setSpeed(LONG_JUMP_RIGHT, glm::vec2(2, 0));

	sprite->setAnimationSpeed(LONG_JUMP_LEFT, 10);
	for (float i = 19; i > 14; --i) {
		sprite->addKeyframe(LONG_JUMP_LEFT, glm::vec2(0.0f + (i / 20.0f), 0.9f));
		sprite->setSpeed(LONG_JUMP_LEFT, glm::vec2(-2, 0));
	}

	sprite->addKeyframe(LONG_JUMP_LEFT, glm::vec2(0.0f + (14.0f / 20.0f), 0.9f));
	sprite->addKeyframe(LONG_JUMP_LEFT, glm::vec2(0.0f + (13.0f / 20.0f), 0.9f));
	sprite->addKeyframe(LONG_JUMP_LEFT, glm::vec2(0.0f + (12.0f / 20.0f), 0.9f));
	sprite->addKeyframe(LONG_JUMP_LEFT, glm::vec2(0.0f + (11.0f / 20.0f), 0.9f));
	sprite->addKeyframe(LONG_JUMP_LEFT, glm::vec2(0.0f + (10.0f / 20.0f), 0.9f));
	sprite->setSpeed(LONG_JUMP_LEFT, glm::vec2(-2, -1));
	sprite->setSpeed(LONG_JUMP_LEFT, glm::vec2(-4, 0));
	sprite->setSpeed(LONG_JUMP_LEFT, glm::vec2(-2, 1));
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

	sprite->setAnimationSpeed(FALLING_LEFT, 8);
	sprite->addKeyframe(FALLING_LEFT, glm::vec2(0.8f, 0.2f));
	sprite->setSpeed(FALLING_LEFT, glm::vec2(0, 0));

	sprite->setAnimationSpeed(FALLING_RIGHT, 8);
	sprite->addKeyframe(FALLING_RIGHT, glm::vec2(0.15f, 0.2f));
	sprite->setSpeed(FALLING_RIGHT, glm::vec2(0, 0));

	sprite->setAnimationSpeed(GET_SWORD_RIGHT, 6);
	for (float i = 0; i <= 8; ++i) {
		sprite->addKeyframe(GET_SWORD_RIGHT, glm::vec2(0.0f + (i / 20.0f), 0.95f));
		sprite->setSpeed(GET_SWORD_RIGHT, glm::vec2(0, 0));
	}

	sprite->setAnimationSpeed(GET_SWORD_LEFT, 6);
	for (float i = 19; i >= 11; --i) {
		sprite->addKeyframe(GET_SWORD_LEFT, glm::vec2(0.0f + (i / 20.0f), 0.95f));
		sprite->setSpeed(GET_SWORD_LEFT, glm::vec2(0, 0));
	}
		
	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + 64), float(tileMapDispl.y + 0)));
	
}

void Player::update(int deltaTime, ShaderProgram &program)
{
	sprite->update(deltaTime);
	change_level_fast();
	change_level();
	trapsManagement(program);
	if (Game::instance().getSpecialKey(GLUT_KEY_LEFT) && !Scene::instance().game_over)
	{
		if (sprite->animation() == LONG_JUMP_LEFT) {
			if (sprite->checkChangeAnimation(LONG_JUMP_LEFT)) {
				sprite->changeAnimation(MOVE_LEFT);
				if (!music_collision){
					Scene::instance().play_music("collision_step.wav", true);
					music_collision = true;
					music_step = true;
				}
			}
			if (map->collisionMoveLeft(posPlayer, glm::ivec2(24, 64))) sprite->changeAnimation(STAND_LEFT);
		}
		if (!bJumping && !climbing && distancia == 0 && sprite->animation() != FALL_RIGHT  && sprite->animation() != FALL_LEFT && sprite->animation() != LONG_JUMP_RIGHT && sprite->animation() != LONG_JUMP_LEFT){
			if (sprite->animation() == MOVE_RIGHT || sprite->animation() == STOP_RIGHT) {
				sprite->changeAnimation(MOVE_TO_LEFT_RUNNING);
				face_direction = false;
			}
			if (sprite->animation() == MOVE_LEFT || sprite->animation() == START_LEFT){
				if (!music_collision){
					Scene::instance().play_music("collision_step.wav", true);
					music_collision = true;
					music_step = true;
				}
			}
			if (sprite->animation() == MOVE_TO_LEFT_RUNNING) {
				if (sprite->checkChangeAnimation(MOVE_TO_LEFT_RUNNING)) {
					sprite->changeAnimation(MOVE_LEFT);
				}
			}
			if (sprite->animation() == STAND_RIGHT) {
				sprite->changeAnimation(MOVE_TO_LEFT);
				//posPlayer.x += 2;
				face_direction = false;
			}
			if (sprite->animation() == MOVE_TO_LEFT) {
				if (sprite->checkChangeAnimation(MOVE_TO_LEFT)) {
					sprite->changeAnimation(STAND_LEFT);
				}
			}
			//if ((sprite->animation() != START_LEFT && sprite->animation() != MOVE_LEFT) && sprite->animation() != MOVE_TO_LEFT && sprite->animation() != MOVE_TO_LEFT_RUNNING) {	// Si aún no ha empezado a moverse y no estaba corriendo -> Empezamos a correr
			if (sprite->animation() == STAND_LEFT){
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
			if (map->collisionDoor(posPlayer, glm::ivec2(24, 64), false)){
				if (!Scene::instance().isDoorOpened()){
					sprite->changeAnimation(STAND_LEFT);
				}
			}
			if (map->collisionMoveLeft(posPlayer, glm::ivec2(24, 64)))
			{
				if (!music_collision){
					Scene::instance().play_music("collision_wall.wav", false);
					music_collision = true;
				}
				sprite->changeAnimation(STAND_LEFT);
			}
			else if (map->collisionMoveRight(posPlayer, glm::ivec2(38, 64)) && sprite->animation() == MOVE_TO_LEFT_RUNNING)
			{
				if (!music_collision){
					Scene::instance().play_music("collision_wall.wav", false);
					music_collision = true;
				}
				sprite->changeAnimation(STAND_RIGHT);
				face_direction = true;
			}
		}
		if (sprite->animation() == FALL_RIGHT) {
			if (sprite->checkChangeAnimation(FALL_RIGHT)) {
				sprite->changeAnimation(MOVE_TO_LEFT);
				face_direction = false;
			}
		}
		else if (sprite->animation() == FALL_LEFT) {
			if (sprite->checkChangeAnimation(FALL_LEFT)) {
				sprite->changeAnimation(START_LEFT);
			}
		}
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT) && !Scene::instance().game_over)
	{
		if (sprite->animation() == LONG_JUMP_RIGHT) {
			if (sprite->checkChangeAnimation(LONG_JUMP_RIGHT)) {
				sprite->changeAnimation(MOVE_RIGHT);
				if (!music_collision){
					Scene::instance().play_music("collision_step.wav", true);
					music_collision = true;
					music_step = true;
				}
			}
			if (map->collisionMoveRight(posPlayer, glm::ivec2(38, 64))) sprite->changeAnimation(STAND_RIGHT);
		}
		if (!bJumping && !climbing && distancia == 0 && sprite->animation() != FALL_RIGHT  && sprite->animation() != FALL_LEFT && sprite->animation() != LONG_JUMP_RIGHT && sprite->animation() != LONG_JUMP_LEFT )
		{
			if (sprite->animation() == MOVE_LEFT || sprite->animation() == STOP_LEFT) {
				sprite->changeAnimation(MOVE_TO_RIGHT_RUNNING);
				face_direction = true;
			}
			if (sprite->animation() == MOVE_RIGHT || sprite->animation() == START_RIGHT){
				if (!music_collision){
					Scene::instance().play_music("collision_step.wav", true);
					music_collision = true;
					music_step = true;
				}
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
				if (!music_collision){
					Scene::instance().play_music("collision_wall.wav", false);
					music_collision = true;
				}
				sprite->changeAnimation(STAND_RIGHT);
			}
			else if (map->collisionMoveLeft(posPlayer, glm::ivec2(24, 64)) && sprite->animation() == MOVE_TO_RIGHT_RUNNING)
			{
				if (!music_collision){
					Scene::instance().play_music("collision_wall.wav", false);
					music_collision = true;
				}
				sprite->changeAnimation(STAND_LEFT);
				face_direction = false;
			}
		}
		if (sprite->animation() == FALL_RIGHT) {
			if (sprite->checkChangeAnimation(FALL_RIGHT)) {
				sprite->changeAnimation(START_RIGHT);
			}
		}
		else if (sprite->animation() == FALL_LEFT) {
			if (sprite->checkChangeAnimation(FALL_LEFT)) {
				sprite->changeAnimation(MOVE_TO_RIGHT);
				face_direction = true;
			}
		}
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_DOWN) && !Scene::instance().game_over){
		if (face_direction){
			if (sprite->checkChangeAnimation(FALL_DOWN_RIGHT)) sprite->changeAnimation(AGACHADO_RIGHT);
		}
		else{
			if (sprite->checkChangeAnimation(FALL_DOWN_LEFT)) sprite->changeAnimation(AGACHADO_LEFT);
		}
		if (!bJumping && !climbing && !agachado){
			agachado = true;
			if (face_direction) sprite->changeAnimation(FALL_DOWN_RIGHT);
			else sprite->changeAnimation(FALL_DOWN_LEFT);
		}
	}
	else
	{
		music_collision = false;
		if (music_step)	{
			Scene::instance().stop_music();
			music_step = false;
		}
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
		else if (sprite->animation() == START_FALL_RIGHT) {
			if (sprite->checkChangeAnimation(START_FALL_RIGHT)){
				sprite->changeAnimation(FALLING_RIGHT);
			}
		}
		else if (sprite->animation() == START_FALL_LEFT) {
			if (sprite->checkChangeAnimation(START_FALL_LEFT)){
				sprite->changeAnimation(FALLING_LEFT);
			}
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
				sprite->changeAnimation(STAND_UP_RIGHT);
				agachado = false;
			}
		}
		else if (sprite->animation() == FALL_DOWN_LEFT) {
			if (sprite->checkChangeAnimation(FALL_DOWN_LEFT)){
				sprite->changeAnimation(STAND_UP_LEFT);
				agachado = false;
			}
		}
		else if (sprite->animation() == AGACHADO_LEFT) {
			if (sprite->checkChangeAnimation(AGACHADO_LEFT)){
				sprite->changeAnimation(STAND_UP_LEFT);
				agachado = false;
			}
		}
		else if (sprite->animation() == AGACHADO_RIGHT) {
			if (sprite->checkChangeAnimation(AGACHADO_RIGHT)){
				sprite->changeAnimation(STAND_UP_RIGHT);
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
				if (!climbing){
					sprite->changeAnimation(JUMP_FALL_RIGHT);
				}
				else sprite->changeAnimation(CLIMB_RIGHT);
			}
		}
		else if (sprite->animation() == JUMP_FALL_RIGHT) {
			if (sprite->checkChangeAnimation(JUMP_FALL_RIGHT))
				sprite->changeAnimation(STAND_RIGHT);
		}
		else if (sprite->animation() == JUMP_LEFT) {
			if (sprite->checkChangeAnimation(JUMP_LEFT)){
				if (climbing) {
					sprite->changeAnimation(CLIMB_LEFT);
				}
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
		else if (sprite->animation() == DIED_FALL_RIGHT) {
			if (sprite->checkChangeAnimation(DIED_FALL_RIGHT)) {
				sprite->changeAnimation(DIED_FINISH_RIGHT);
				if (!Scene::instance().game_over) Scene::instance().set_game_over();
			}
		}
		else if (sprite->animation() == DIED_FALL_LEFT) {
			if (sprite->checkChangeAnimation(DIED_FALL_LEFT)) {
				sprite->changeAnimation(DIED_FINISH_LEFT);
				if (!Scene::instance().game_over) Scene::instance().set_game_over();
			}
		}
		else if (sprite->animation() == GET_SWORD_RIGHT) {
			if (sprite->checkChangeAnimation(GET_SWORD_RIGHT)) {
				sprite->changeAnimation(STAND_RIGHT);
			}
		}
		else if (sprite->animation() == GET_SWORD_LEFT) {
			if (sprite->checkChangeAnimation(GET_SWORD_LEFT)) {
				sprite->changeAnimation(STAND_LEFT);
			}
		}
		else if (sprite->animation() == DIED_RIGHT || sprite->animation() == DIED_LEFT){
			if(!Scene::instance().game_over) Scene::instance().set_game_over();
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
				bJumping = !map->collisionMoveDown(posPlayer, glm::ivec2(32, 8), &posPlayer.y, level);
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
					climbing = false;
					sprite->changeAnimation(STAND_RIGHT);
				}
				else if (sprite->checkChangeAnimation(CLIMB_LEFT)) {
					posPlayer.y = startY - 64;
					posPlayer.x -= 16;
					climbing = false;
					sprite->changeAnimation(STAND_LEFT);
				}
			}
			if (jumpAngle > 180){ //FIN SALTO
				if (face_direction) climbing = !map->collisionMoveDown(posPlayer, glm::ivec2(32, 9), &posPlayer.y, level);
				else climbing = !map->collisionMoveDown(posPlayer, glm::ivec2(32, 9), &posPlayer.y, level);
			}

		}
	}
	else
	{	
		if (!Scene::instance().game_over && distancia > 2 && sprite->animation() != START_FALL_RIGHT && sprite->animation() != START_FALL_LEFT && sprite->animation() != FALL_RIGHT && sprite->animation() != FALL_LEFT && sprite->animation() != FALLING_RIGHT && sprite->animation() != FALLING_LEFT) {
			if (face_direction)
				sprite->changeAnimation(START_FALL_RIGHT);
			else sprite->changeAnimation(START_FALL_LEFT);
		}
		if (sprite->animation() != LONG_JUMP_LEFT && sprite->animation() != LONG_JUMP_RIGHT && !Scene::instance().game_over)posPlayer.y += FALL_STEP;
		if (map->collisionMoveDown(posPlayer, glm::ivec2(32, 8), &posPlayer.y, level) && !Scene::instance().game_over)
		{
			if (distancia >= 128 && distancia <= 180){
				--hp;
				if (hp == 0){
					Scene::instance().changeHealthAnimation(hp);
					if (face_direction) sprite->changeAnimation(DIED_FALL_RIGHT);
					else sprite->changeAnimation(DIED_FALL_LEFT);
					distancia = 0;
				}
				else{
					if(hp > 0)Scene::instance().changeHealthAnimation(hp);
				}
			}
			if (distancia > 180){
				hp = 0;
				Scene::instance().changeHealthAnimation(hp);
				if (face_direction) sprite->changeAnimation(DIED_FALL_RIGHT);
				else sprite->changeAnimation(DIED_FALL_LEFT);
				distancia = 0;
			}
			else{
				glm::ivec2 posTile;
				if (map->collisionWith(posPlayer, glm::ivec2(32, 8), &posPlayer.y, 15, posTile)) {
					if (!music_collision && !music_collision_over){
						Scene::instance().play_music("collision_floor.wav", false);
						music_collision = true;
						music_collision_over = true;
					}
					if (face_direction) sprite->changeAnimation(DIED_RIGHT);
					else sprite->changeAnimation(DIED_LEFT);
					hp = 0;
					Scene::instance().changeHealthAnimation(hp);
				}
				else if (map->collisionWith(posPlayer, glm::ivec2(32, 8), &posPlayer.y, 21, posTile)) {
					map->changeTile(glm::ivec2(posTile.x, posTile.y), 5, program);
					vector<TileChange> traps = map->getTraps();
					for (int i = 0; i < traps.size(); ++i){
						if (traps[i].x == posTile.x && traps[i].y == posTile.y) map->changeTileTrap(i, 5);
					}
					Scene::instance().openDoor();
				}
				else if (map->collisionWith(posPlayer, glm::ivec2(32, 8), &posPlayer.y, 10, posTile) || map->collisionWith(posPlayer, glm::ivec2(32, 8), &posPlayer.y, 21, posTile)) {
					map->changeTile(glm::ivec2(posTile.x, posTile.y), 9, program);
				}
				else if (!sword && map->collisionWith(posPlayer, glm::ivec2(32, 8), &posPlayer.y, 23, posTile)){
					sword = true;
					map->changeTile(glm::ivec2(posTile.x,posTile.y), 5, program);
					map->changeTile(glm::ivec2(posTile.x+1, posTile.y), 5, program);
					if (face_direction)sprite->changeAnimation(GET_SWORD_RIGHT);
					else sprite->changeAnimation(GET_SWORD_LEFT);

				}
				else{
					if (Game::instance().getSpecialKey(GLUT_KEY_UP) && !Scene::instance().game_over)
					{
						if (sprite->animation() == MOVE_RIGHT || sprite->animation() == LONG_JUMP_RIGHT){
							sprite->changeAnimation(LONG_JUMP_RIGHT);
							if (music_step)	{
								Scene::instance().stop_music();
								music_step = false;
								music_collision = false;
							}
						}
						else if (sprite->animation() == MOVE_LEFT || sprite->animation() == LONG_JUMP_LEFT) {
							sprite->changeAnimation(LONG_JUMP_LEFT);
							if (music_step)	{
								Scene::instance().stop_music();
								music_step = false;
								music_collision = false;
							}
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
								if (posPlayer.y >= -8){
									height_air = 48;
									distancia -= 10;
									if (face_direction){
										if (map->collisionClimb(posPlayer, glm::ivec2(32, 64), &posPlayer.y, true)) {
											glm::ivec2 posAux = glm::ivec2(posPlayer.x, posPlayer.y - 64);
											if (map->collisionDoor(posAux, glm::ivec2(24, 64), true)){
												if (!Scene::instance().isDoorOpened()){
													sprite->changeAnimation(STAND_RIGHT);
												}
												else {
													climbing = true;
													bJumping = false;
													int posX = posPlayer.x % 32;
													if (posX > 17) posPlayer.x = posPlayer.x - (posX - 17);
													else if (posX < 17) posPlayer.x = posPlayer.x + (17 - posX);//else 
												}
											}
											else {
												climbing = true;
												bJumping = false;
												int posX = posPlayer.x % 32;
												if (posX > 17) posPlayer.x = posPlayer.x - (posX - 17);
												else if (posX < 17) posPlayer.x = posPlayer.x + (17 - posX);//else 
											}
										}
									}
									else{
										if (map->collisionClimb(posPlayer, glm::ivec2(32, 64), &posPlayer.y, false)) {
											glm::ivec2 posAux = glm::ivec2(posPlayer.x, posPlayer.y - 64);
											if (map->collisionDoor(posAux, glm::ivec2(24, 64), false)){
												if (!Scene::instance().isDoorOpened()){
													sprite->changeAnimation(STAND_LEFT);
												}
												else {
													climbing = true;
													bJumping = false;
													int posX = (posPlayer.x % 32);
													if (posX > 17) posPlayer.x = posPlayer.x - (posX - 17);
													else if (posX < 17) posPlayer.x = posPlayer.x + (17 - posX);//else 
												}
											}
											else {
												climbing = true;
												bJumping = false;
												int posX = (posPlayer.x % 32);
												if (posX > 17) posPlayer.x = posPlayer.x - (posX - 17);
												else if (posX < 17) posPlayer.x = posPlayer.x + (17 - posX);//else 
											}
										}
									}
								}
							}
							if (face_direction) sprite->changeAnimation(START_JUMP_RIGHT);
							else sprite->changeAnimation(START_JUMP_LEFT);
						}
					}
					else if (distancia >= 56) {
						// Amortiguar caída
						if (face_direction) sprite->changeAnimation(FALL_RIGHT);
						else sprite->changeAnimation(FALL_LEFT);
						distancia = 0;
						if (!music_collision){
							Scene::instance().play_music("collision_floor.wav", false);
							music_collision = true;
						}
					}
					else distancia = 0;
				}
			}
		}
		else {
			if (sprite->animation() != LONG_JUMP_LEFT && sprite->animation() != LONG_JUMP_RIGHT && !Scene::instance().game_over)distancia += FALL_STEP;
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

void Player::change_level(){
	if (strcmp(Scene::instance().getLevel().c_str(), "levels/prince-map1.txt") == 0) {
		if (posPlayer.x >= 208 && posPlayer.y >= 120)
			Scene::instance().setLevel("levels/prince-map3.txt", glm::vec2(208, -72), "levels/col3.txt", false, glm::ivec2(0, 120));
	}
	else if (strcmp(Scene::instance().getLevel().c_str(), "levels/prince-map3.txt") == 0) {
		if (posPlayer.x >= 314 && posPlayer.y >= 56) {
			Scene::instance().setLevel("levels/prince-map4.txt", glm::vec2(6, 56), "levels/col4.txt", true, glm::ivec2(310, 56));
		}
		else if (posPlayer.x == 0 && posPlayer.y == -8)
			Scene::instance().setLevel("levels/prince-map5.txt", glm::vec2(313, -8), "levels/col5.txt", false, glm::ivec2(0, 120));
	}
	else if (strcmp(Scene::instance().getLevel().c_str(), "levels/prince-map4.txt") == 0) {
		if (posPlayer.x <= 5 && posPlayer.y >= 56) {
			Scene::instance().setLevel("levels/prince-map3.txt", glm::vec2(313, 56), "levels/col3.txt", false, glm::ivec2(0, 120));
			Scene::instance().isEnemyVisible(false);
		}
		else if (posPlayer.x >= 314 && posPlayer.y >= 56) {
			Scene::instance().setLevel("levels/prince-map6.txt", glm::vec2(6, 56), "levels/col6.txt", false, glm::ivec2(0, 120));
			Scene::instance().isEnemyVisible(false);
		}
	}
	else if (strcmp(Scene::instance().getLevel().c_str(), "levels/prince-map5.txt") == 0){
		if (posPlayer.x >= 314 && posPlayer.y == -8)
			Scene::instance().setLevel("levels/prince-map3.txt", glm::vec2(6, -8), "levels/col3.txt", false, glm::ivec2(0, 120));
		else if (posPlayer.x <=5 && posPlayer.y == -8)
			Scene::instance().setLevel("levels/prince-map7.txt", glm::vec2(313, -8), "levels/col7.txt", false, glm::ivec2(0, 120));
	}
	else if (strcmp(Scene::instance().getLevel().c_str(), "levels/prince-map6.txt") == 0){
		if (posPlayer.x <= 5 && posPlayer.y == 56) {
			Scene::instance().setLevel("levels/prince-map4.txt", glm::vec2(313, 56), "levels/col4.txt", true, glm::ivec2(10, 56));
		}
		else if (posPlayer.x >= 313 && posPlayer.y == 56) {
			level = 2;
			Scene::instance().setLevel("levels/palace-map1.txt", glm::vec2(6, 56), "levels/map2-col1.txt", false, glm::ivec2(10, 56));
		}
	}
	else if (strcmp(Scene::instance().getLevel().c_str(), "levels/prince-map7.txt") == 0){
		if (posPlayer.x <= 158 && posPlayer.y >= 120)
			Scene::instance().setLevel("levels/prince-map9.txt", glm::vec2(158, -8), "levels/col9.txt", false, glm::ivec2(0, 120));
		else if (posPlayer.x <= 5 && posPlayer.y == -8)
			Scene::instance().setLevel("levels/prince-map8.txt", glm::vec2(311, -8), "levels/col8.txt", false, glm::ivec2(0, 120));
		else if (posPlayer.x >= 313 && posPlayer.y == -8)
			Scene::instance().setLevel("levels/prince-map5.txt", glm::vec2(6,-8), "levels/col5.txt", false, glm::ivec2(0, 120));
	}
	else if (strcmp(Scene::instance().getLevel().c_str(), "levels/prince-map8.txt") == 0){
		if (posPlayer.x <= 130 && posPlayer.y >= 120){
			distancia = 64;
			Scene::instance().setLevel("levels/prince-map10.txt", glm::vec2(126, -8), "levels/col10.txt", false, glm::ivec2(0, 120));
		}
		else if (posPlayer.x <= 5 && posPlayer.y == 56)
			Scene::instance().setLevel("levels/prince-map11.txt", glm::vec2(313, 56), "levels/col11.txt", false, glm::ivec2(0, 120));
		else if (posPlayer.x >= 313 && posPlayer.y == -8)
			Scene::instance().setLevel("levels/prince-map7.txt", glm::vec2(6, -8), "levels/col7.txt", false, glm::ivec2(0, 120));
	}
	else if (strcmp(Scene::instance().getLevel().c_str(), "levels/prince-map11.txt") == 0){
		if (posPlayer.x <= 5 && posPlayer.y == -8) {
			Scene::instance().setLevel("levels/prince-map12.txt", glm::vec2(313, -8), "levels/col12.txt", false, glm::ivec2(10, 56));
		}
		else if (posPlayer.x >= 313 && posPlayer.y == 56) {
			Scene::instance().setLevel("levels/prince-map8.txt", glm::vec2(6, 56), "levels/col8.txt", false, glm::ivec2(10, 56));
		}
	}
	else if (strcmp(Scene::instance().getLevel().c_str(), "levels/prince-map12.txt") == 0){
		if (posPlayer.x <= 5 && posPlayer.y == -8) {
			Scene::instance().setLevel("levels/prince-map13.txt", glm::vec2(313, -8), "levels/col13.txt", false, glm::ivec2(10, 56));
		}
		else if (posPlayer.x <= 5 && posPlayer.y == 56){
			if (!sword) Scene::instance().setLevel("levels/prince-map13.txt", glm::vec2(311, 56), "levels/col13.txt", false, glm::ivec2(0, 120));
			else Scene::instance().setLevel("levels/prince-map14.txt", glm::vec2(311, 56), "levels/col13.txt", false, glm::ivec2(0, 120));
		}
		else if (posPlayer.x >= 314 && posPlayer.y == -8)
			Scene::instance().setLevel("levels/prince-map11.txt", glm::vec2(6, -8), "levels/col11.txt", false, glm::ivec2(0, 120));
	}
	else if (strcmp(Scene::instance().getLevel().c_str(), "levels/prince-map13.txt") == 0){
		if (posPlayer.x >= 314 && posPlayer.y == -8) {
			Scene::instance().setLevel("levels/prince-map12.txt", glm::vec2(6, -8), "levels/col12.txt", false, glm::ivec2(10, 56));
		}
		else if (posPlayer.x >= 314 && posPlayer.y == 56)
			Scene::instance().setLevel("levels/prince-map12.txt", glm::vec2(6, 56), "levels/col12.txt", false, glm::ivec2(0, 120));
	}
	else if (strcmp(Scene::instance().getLevel().c_str(), "levels/prince-map14.txt") == 0){
		if (posPlayer.x >= 314 && posPlayer.y == -8) {
			Scene::instance().setLevel("levels/prince-map12.txt", glm::vec2(6, -8), "levels/col12.txt", false, glm::ivec2(10, 56));
		}
		else if (posPlayer.x >= 314 && posPlayer.y == 56)
			Scene::instance().setLevel("levels/prince-map12.txt", glm::vec2(6, 56), "levels/col12.txt", false, glm::ivec2(0, 120));
	}
	else if (strcmp(Scene::instance().getLevel().c_str(), "levels/palace-map1.txt") == 0){
		if (posPlayer.x <=5 && posPlayer.y == 56) {
			Scene::instance().setLevel("levels/palace-map5.txt", glm::vec2(313, 56), "levels/map2-col5.txt", false, glm::ivec2(10, 56));
			level == 1;
		}
		else if (posPlayer.x >= 314 && posPlayer.y == 56) {
			Scene::instance().setLevel("levels/palace-map2.txt", glm::vec2(6, 56), "levels/map2-col2.txt", false, glm::ivec2(10, 56));
		}
	}
	else if (strcmp(Scene::instance().getLevel().c_str(), "levels/palace-map2.txt") == 0){
		if (posPlayer.x <= 5 && posPlayer.y == 56) {
			Scene::instance().setLevel("levels/palace-map1.txt", glm::vec2(313, 56), "levels/map2-col1.txt", false, glm::ivec2(10, 56));
		}
		else if (posPlayer.x >= 314 && posPlayer.y == 56) {
			Scene::instance().setLevel("levels/palace-map3.txt", glm::vec2(6, 56), "levels/map2-col3.txt", false, glm::ivec2(10, 56));
		}
	}
	else if (strcmp(Scene::instance().getLevel().c_str(), "levels/palace-map3.txt") == 0){
		if (posPlayer.x <= 5 && posPlayer.y == 56) {
			Scene::instance().setLevel("levels/palace-map2.txt", glm::vec2(313, 56), "levels/map2-col2.txt", false, glm::ivec2(10, 56));
		}
		else if (posPlayer.x >= 314 && posPlayer.y == 56) {
			Scene::instance().setLevel("levels/palace-map4.txt", glm::vec2(6, 56), "levels/map2-col4.txt", false, glm::ivec2(10, 56));
		}
	}
	else if (strcmp(Scene::instance().getLevel().c_str(), "levels/palace-map4.txt") == 0){
		if (posPlayer.x <= 5 && posPlayer.y == 56) {
			Scene::instance().setLevel("levels/palace-map3.txt", glm::vec2(313, 56), "levels/map2-col3.txt", false, glm::ivec2(10, 56));
		}
		else if (posPlayer.x >= 314 && posPlayer.y == 56) {
			Scene::instance().setLevel("levels/palace-map9.txt", glm::vec2(6, 56), "levels/map2-col9.txt", false, glm::ivec2(10, 56));
		}
	}
	else if (strcmp(Scene::instance().getLevel().c_str(), "levels/palace-map5.txt") == 0){
		if (posPlayer.x <= 5 && posPlayer.y == 56) {
			Scene::instance().setLevel("levels/palace-map6.txt", glm::vec2(313, 56), "levels/map2-col6.txt", false, glm::ivec2(10, 56));
			level == 1;
		}
		else if (posPlayer.x >= 314 && posPlayer.y == 56) {
			Scene::instance().setLevel("levels/palace-map1.txt", glm::vec2(6, 56), "levels/map2-col1.txt", false, glm::ivec2(10, 56));
		}
	}
	else if (strcmp(Scene::instance().getLevel().c_str(), "levels/palace-map9.txt") == 0){
		if (posPlayer.x <= 5 && posPlayer.y == 56) {
			Scene::instance().setLevel("levels/palace-map4.txt", glm::vec2(313, 56), "levels/map2-col4.txt", false, glm::ivec2(10, 56));
		}
		else if (posPlayer.x >= 314 && posPlayer.y == 56) {
			Scene::instance().setLevel("levels/palace-map10.txt", glm::vec2(6, 56), "levels/map2-col10.txt", false, glm::ivec2(10, 56));
		}
	}
	else if (strcmp(Scene::instance().getLevel().c_str(), "levels/palace-map10.txt") == 0){
		if (posPlayer.x <= 5 && posPlayer.y == 56) {
			Scene::instance().setLevel("levels/palace-map9.txt", glm::vec2(313, 56), "levels/map2-col9.txt", false, glm::ivec2(10, 56));
		}
		else if (posPlayer.x >= 314 && posPlayer.y == 56) {
			Scene::instance().setLevel("levels/palace-map11.txt", glm::vec2(6, 56), "levels/map2-col11.txt", false, glm::ivec2(10, 56));
		}
	}
	else if (strcmp(Scene::instance().getLevel().c_str(), "levels/palace-map11.txt") == 0){
		if (posPlayer.x <= 5 && posPlayer.y == 56) {
			Scene::instance().setLevel("levels/palace-map10.txt", glm::vec2(313, 56), "levels/map2-col10.txt", false, glm::ivec2(10, 56));
		}
		else if (posPlayer.x >= 314 && posPlayer.y == 56) {
			Scene::instance().setLevel("levels/palace-map12.txt", glm::vec2(6, 56), "levels/map2-col12.txt", false, glm::ivec2(10, 56));
		}
		else if (posPlayer.x >= 314 && posPlayer.y == -8) {
			Scene::instance().setLevel("levels/palace-map12.txt", glm::vec2(6, -8), "levels/map2-col12.txt", false, glm::ivec2(10, 56));
		}
	}
	else if (strcmp(Scene::instance().getLevel().c_str(), "levels/palace-map6.txt") == 0){
		if (posPlayer.x <= 5 && posPlayer.y == 56) {
			Scene::instance().setLevel("levels/palace-map7.txt", glm::vec2(313, 56), "levels/map2-col7.txt", false, glm::ivec2(10, 56));
		}
		else if (posPlayer.x >= 314 && posPlayer.y == 56) {
			Scene::instance().setLevel("levels/palace-map5.txt", glm::vec2(6, 56), "levels/map2-col5.txt", false, glm::ivec2(10, 56));
		}
	}
	else if (strcmp(Scene::instance().getLevel().c_str(), "levels/palace-map7.txt") == 0){
		if (posPlayer.x <= 5 && posPlayer.y == -8) {
			Scene::instance().setLevel("levels/palace-map8.txt", glm::vec2(313, -8), "levels/map2-col8.txt", false, glm::ivec2(10, 56));
		}
		else if (posPlayer.x >= 314 && posPlayer.y == 56) {
			Scene::instance().setLevel("levels/palace-map6.txt", glm::vec2(6, 56), "levels/map2-col6.txt", false, glm::ivec2(10, 56));
		}
	}

}

void Player::change_level_fast(){
	if (Game::instance().getSpecialKey(GLUT_KEY_F1)){
		if(level == 1) Scene::instance().setLevel("levels/prince-map1.txt", glm::vec2(64, 0), "levels/col1.txt", false, glm::ivec2(0, 120));
		else Scene::instance().setLevel("levels/palace-map1.txt", glm::vec2(6, 56), "levels/map2-col1.txt", false, glm::ivec2(10, 56));
		sprite->changeAnimation(STAND_RIGHT);
		face_direction = true;
		Scene::instance().isEnemyVisible(false);
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_F3)){
		if(level == 1) Scene::instance().setLevel("levels/prince-map3.txt", glm::vec2(208, 0), "levels/col3.txt", false, glm::ivec2(0, 120));
		else Scene::instance().setLevel("levels/palace-map3.txt", glm::vec2(6, 56), "levels/map2-col3.txt", false, glm::ivec2(10, 56));
		sprite->changeAnimation(STAND_RIGHT);
		face_direction = true;
		Scene::instance().isEnemyVisible(false);
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_F4)){
		if(level == 1) Scene::instance().setLevel("levels/prince-map4.txt", glm::vec2(6, 56), "levels/col4.txt", true, glm::ivec2(250, 56));
		else Scene::instance().setLevel("levels/palace-map4.txt", glm::vec2(6, 56), "levels/map2-col4.txt", false, glm::ivec2(250, 56));
		sprite->changeAnimation(STAND_RIGHT);
		face_direction = true;
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_F5)){
		if (level == 1) {
			Scene::instance().setLevel("levels/prince-map5.txt", glm::vec2(313, -8), "levels/col5.txt", false, glm::ivec2(0, 120));
			sprite->changeAnimation(STAND_LEFT);
			face_direction = false;
		}
		else {
			Scene::instance().setLevel("levels/palace-map5.txt", glm::vec2(12, 56), "levels/map2-col5.txt", false, glm::ivec2(250, 56));
			sprite->changeAnimation(STAND_RIGHT);
			face_direction = true;
		}
		Scene::instance().isEnemyVisible(false);
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_F6)){
		if(level == 1) Scene::instance().setLevel("levels/prince-map6.txt", glm::vec2(6, 56), "levels/col6.txt", false, glm::ivec2(0, 120));
		else Scene::instance().setLevel("levels/palace-map6.txt", glm::vec2(6, 56), "levels/map2-col6.txt", false, glm::ivec2(0, 120));
		sprite->changeAnimation(STAND_RIGHT);
		face_direction = true;
		Scene::instance().isEnemyVisible(false);
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_F7)){
		if (level == 1) {
			Scene::instance().setLevel("levels/prince-map7.txt", glm::vec2(311, -8), "levels/col7.txt", false, glm::ivec2(0, 120));
			sprite->changeAnimation(STAND_LEFT);
			face_direction = false;
		}
		else {
			Scene::instance().setLevel("levels/palace-map7.txt", glm::vec2(311, 56), "levels/map2-col7.txt", false, glm::ivec2(0, 120));
			sprite->changeAnimation(STAND_LEFT);
			face_direction = false;
		}

		Scene::instance().isEnemyVisible(false);
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_F8)){
		if (level == 1) {
			Scene::instance().setLevel("levels/prince-map8.txt", glm::vec2(311, -8), "levels/col8.txt", false, glm::ivec2(0, 120));
			sprite->changeAnimation(STAND_LEFT);
			face_direction = false;
		}
		else {
			Scene::instance().setLevel("levels/palace-map8.txt", glm::vec2(313, -8), "levels/map2-col8.txt", false, glm::ivec2(0, 120));
			sprite->changeAnimation(STAND_LEFT);
			face_direction = false;
		}
		Scene::instance().isEnemyVisible(false);
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_F9)){
		if (level == 1) {
			Scene::instance().setLevel("levels/prince-map11.txt", glm::vec2(311, 56), "levels/col11.txt", false, glm::ivec2(0, 120));
			sprite->changeAnimation(STAND_LEFT);
			face_direction = false;
		}
		else {
			Scene::instance().setLevel("levels/palace-map9.txt", glm::vec2(6, 56), "levels/map2-col9.txt", false, glm::ivec2(0, 120));
			sprite->changeAnimation(STAND_RIGHT);
			face_direction = true;
		}
		Scene::instance().isEnemyVisible(false);
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_F2)){
		if (level == 1){
			Scene::instance().setLevel("levels/prince-map12.txt", glm::vec2(311, -8), "levels/col12.txt", false, glm::ivec2(0, 120));
			sprite->changeAnimation(STAND_LEFT);
			face_direction = false;
		}
		else{
			Scene::instance().setLevel("levels/palace-map2.txt", glm::vec2(6, 56), "levels/map2-col2.txt", false, glm::ivec2(0, 120));
			sprite->changeAnimation(STAND_RIGHT);
			face_direction = true;
		}
		Scene::instance().isEnemyVisible(false);
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_F11)){
		if (level == 1){
			if (!sword) Scene::instance().setLevel("levels/prince-map13.txt", glm::vec2(311, 56), "levels/col13.txt", false, glm::ivec2(0, 120));
			else Scene::instance().setLevel("levels/prince-map14.txt", glm::vec2(311, 56), "levels/col13.txt", false, glm::ivec2(0, 120));
			sprite->changeAnimation(STAND_LEFT);
			face_direction = false;
		}
		else{
			Scene::instance().setLevel("levels/palace-map11.txt", glm::vec2(311, -8), "levels/map2-col11.txt", false, glm::ivec2(0, 120));
			sprite->changeAnimation(STAND_LEFT);
			face_direction = false;
		}
		Scene::instance().isEnemyVisible(false);
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_HOME)){
		if (level == 1){
			Scene::instance().setLevel("levels/prince-map10.txt", glm::vec2(311, -8), "levels/col10.txt", false, glm::ivec2(0, 120));
			sprite->changeAnimation(STAND_LEFT);
			face_direction = false;
		}
		else{
			Scene::instance().setLevel("levels/palace-map10.txt", glm::vec2(6, 56), "levels/map2-col10.txt", false, glm::ivec2(0, 120));
			sprite->changeAnimation(STAND_RIGHT);
			face_direction = true;
		}
		Scene::instance().isEnemyVisible(false);
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_END)){
		if (level==2){ 
			Scene::instance().setLevel("levels/palace-map12.txt", glm::vec2(6, 56), "levels/map2-col12.txt", false, glm::ivec2(0, 120));
			sprite->changeAnimation(STAND_RIGHT);
			face_direction = true;
		}
		Scene::instance().isEnemyVisible(false);
	}
}

void Player::die(){
	if (face_direction) sprite->changeAnimation(DIED_RIGHT);
	else sprite->changeAnimation(START_FALL_LEFT);
}

glm::ivec2 Player::getPosition() {
	return posPlayer;
}

void Player::trapsManagement(ShaderProgram program) {
	vector<TileChange> traps = map->getTraps();
	for (int i = 0; i < traps.size(); ++i) {
		if (traps[i].tile != traps[i].status) {
			if (face_direction) {
				if (traps[i].x < (posPlayer.x + 40)/32) {
					map->changeTile(glm::ivec2(traps[i].x, traps[i].y), traps[i].tile, program);
				}
			}
			else {
				if (traps[i].x > (posPlayer.x - 20)/32) {
					map->changeTile(glm::ivec2(traps[i].x, traps[i].y), traps[i].tile, program);
				}
			}
		}
	}
}