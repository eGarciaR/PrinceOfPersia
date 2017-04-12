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
	STAND_RIGHT, STAND_LEFT, MOVE_LEFT, MOVE_RIGHT, MOVE_TO_LEFT, MOVE_TO_RIGHT, DIED_RIGHT, DIED_LEFT, STAND_FIGHT_LEFT, ATACK_LEFT, BLOCK_LEFT,
	DIED_FALL_LEFT
};


void Enemy::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, string file)
{
	face_direction = true;
	created = false;
	atacking = false;
	playerAttacking = false;
	blocked = false;
	music_on = false;
	hp = 3;
	posEnemy.x = 310;
	posEnemy.y = 56;
	spritesheet.loadFromFile(file, TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(64, 64), glm::vec2(0.1, 0.2), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(13);

	sprite->setAnimationSpeed(STAND_LEFT, 8);
	sprite->addKeyframe(STAND_LEFT, glm::vec2(0.9f, 0.0f));
	sprite->setSpeed(STAND_LEFT, glm::vec2(0, 0));

	sprite->setAnimationSpeed(STAND_RIGHT, 8);
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.f, 0.f));
	sprite->setSpeed(STAND_RIGHT, glm::vec2(0, 0));


	sprite->setAnimationSpeed(MOVE_LEFT, 8);
	for (float i = 8; i > 3; --i) {
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.0f + (i / 10.0f), 0.0f));
		sprite->setSpeed(MOVE_LEFT, glm::vec2(-1, 0));
	}

	sprite->setAnimationSpeed(MOVE_RIGHT, 8);
	for (float i = 0; i <= 4; ++i) {
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.0f + (i / 10.0f), 0.0f));
		sprite->setSpeed(MOVE_RIGHT, glm::vec2(1, 0));
	}

	sprite->setAnimationSpeed(STAND_FIGHT_LEFT, 8);
	sprite->addKeyframe(STAND_FIGHT_LEFT, glm::vec2((8 / 10.0f), 0.20f));
	sprite->setSpeed(STAND_FIGHT_LEFT, glm::vec2(0, 0));

	sprite->setAnimationSpeed(ATACK_LEFT, 8);
	for (float i = 9; i >= 5; --i) {
		sprite->addKeyframe(ATACK_LEFT, glm::vec2(0.0f + (i / 10.0f), 0.40f));
		sprite->setSpeed(ATACK_LEFT, glm::vec2(0, 0));
	}

	sprite->setAnimationSpeed(DIED_FALL_LEFT, 8);
	for (float i = 9; i >= 7; --i) {
		sprite->addKeyframe(DIED_FALL_LEFT, glm::vec2((i / 10.0f), 0.80f));
		sprite->setSpeed(DIED_FALL_LEFT, glm::vec2(0, 0));
	}

	sprite->setAnimationSpeed(BLOCK_LEFT, 8);
	for (float i = 9; i >= 5; --i) {
		sprite->addKeyframe(BLOCK_LEFT, glm::vec2((i / 10.0f), 0.20f));
		sprite->setSpeed(BLOCK_LEFT, glm::vec2(0, 0));
	}

	sprite->setAnimationSpeed(DIED_LEFT, 8);
	sprite->addKeyframe(DIED_LEFT, glm::vec2((7 / 10.0f), 0.80f));
	sprite->setSpeed(DIED_LEFT, glm::vec2(0, 0));

	sprite->changeAnimation(1);
	tileMapDispl = tileMapPos;
}

void Enemy::update(int deltaTime, glm::ivec2 posPlayer)
{
	sprite->update(deltaTime);
	posEnemy.x += sprite->getSpeed().x;
	posEnemy.y += sprite->getSpeed().y;
	if (hp <= 0){
		Scene::instance().changeHealthEnemyAnimation(hp);
		if (sprite->animation() != DIED_FALL_LEFT && sprite->animation() != DIED_LEFT) sprite->changeAnimation(DIED_FALL_LEFT);
		if (sprite->checkChangeAnimation(DIED_FALL_LEFT)) sprite->changeAnimation(DIED_LEFT);
		if (!music_on){
			music_on = true;
			Scene::instance().play_music("guard_death_and_obtaining_the_sword.wav", false);
		}
	}
	else{
		if (!atacking){
			if (posEnemy.x >= posPlayer.x){
				face_direction = false;
			}
			else face_direction = true;
			if (!face_direction){
				if (posEnemy.x - posPlayer.x >= 16){
					if (sprite->animation() != MOVE_LEFT) sprite->changeAnimation(MOVE_LEFT);
					if (sprite->checkChangeAnimation(MOVE_LEFT)) sprite->changeAnimation(MOVE_LEFT);
				}
				else {
					if (sprite->checkChangeAnimation(ATACK_LEFT)) sprite->changeAnimation(STAND_FIGHT_LEFT);
					else{
						if (sprite->animation() != ATACK_LEFT)sprite->changeAnimation(STAND_FIGHT_LEFT);
						int r = rand() % 100;
						if (r == 1 && !Scene::instance().PlayerDied()) {
							sprite->changeAnimation(ATACK_LEFT);
							atacking = true;
						}
						if (Scene::instance().getPlayerAnimation() == 46 && !playerAttacking){
							playerAttacking = true;
							r = rand() % 20;
							if (r == 1) {
								sprite->changeAnimation(BLOCK_LEFT);
								blocked = true;
							}
							if (sprite->checkChangeAnimation(BLOCK_LEFT)) sprite->changeAnimation(STAND_FIGHT_LEFT);
						}
						else if (Scene::instance().getPlayerAnimation() != 46 && playerAttacking) {
							if (!blocked){
								--hp;
								if (hp > 0)Scene::instance().changeHealthEnemyAnimation(hp);
							}
							else blocked = false;
							playerAttacking = false;
						}
					}
				}
			}
			else{
				if (sprite->animation() != MOVE_RIGHT) sprite->changeAnimation(MOVE_RIGHT);
				if (sprite->checkChangeAnimation(MOVE_RIGHT)) sprite->changeAnimation(MOVE_RIGHT);
			}
		}
		else{
			if (Scene::instance().getPlayerAnimation() == 52) atacking = false;
			if (atacking && sprite->checkChangeAnimation(ATACK_LEFT)){
				atacking = false;
				if(!Scene::instance().hasSword())Scene::instance().PlayerDamaged(3);
				else{
					Scene::instance().PlayerDamaged(1);
				}
				
			}
		}
	}
	if(hp>0)sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
	
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
	if(hp>0)sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

void Enemy::die(){
	if (face_direction) sprite->changeAnimation(DIED_RIGHT);
	else sprite->changeAnimation(DIED_LEFT);
}

glm::ivec2 Enemy::getEnemyPosition(){
	return posEnemy;
}

void Enemy::setEnemyCreated(bool c){
	created = c;
}

bool Enemy::isEnemyCreated(){
	return created;
}

void Enemy::setEnemyPositon(glm::ivec2 pos){
	posEnemy = pos;
}

int Enemy::getEnemyHp(){
	return hp;
}