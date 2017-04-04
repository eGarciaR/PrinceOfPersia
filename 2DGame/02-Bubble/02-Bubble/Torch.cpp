#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Torch.h"
#include "Game.h"

enum PlayerAnims
{
	ANIMATE
};


void Torch::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	spritesheet.loadFromFile("images/torch-sprite.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(15, 20), glm::vec2(0.1, 0.1), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(ANIMATE, 8);
	sprite->addKeyframe(ANIMATE, glm::vec2(0.f, 0.0f));
	sprite->addKeyframe(ANIMATE, glm::vec2(0.1f, 0.0f));
	sprite->addKeyframe(ANIMATE, glm::vec2(0.2f, 0.0f));
	sprite->addKeyframe(ANIMATE, glm::vec2(0.3f, 0.0f));
	sprite->addKeyframe(ANIMATE, glm::vec2(0.4f, 0.0f));
	sprite->addKeyframe(ANIMATE, glm::vec2(0.5f, 0.0f));
	sprite->addKeyframe(ANIMATE, glm::vec2(0.6f, 0.0f));
	sprite->addKeyframe(ANIMATE, glm::vec2(0.7f, 0.0f));
	sprite->setSpeed(ANIMATE, glm::vec2(0, 0));
	sprite->setSpeed(ANIMATE, glm::vec2(0, 0));
	sprite->setSpeed(ANIMATE, glm::vec2(0, 0));
	sprite->setSpeed(ANIMATE, glm::vec2(0, 0));
	sprite->setSpeed(ANIMATE, glm::vec2(0, 0));
	sprite->setSpeed(ANIMATE, glm::vec2(0, 0));
	sprite->setSpeed(ANIMATE, glm::vec2(0, 0));
	sprite->setSpeed(ANIMATE, glm::vec2(0, 0));

	
	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;

}

void Torch::update(int deltaTime, int x, int y)
{
	sprite->update(deltaTime);
	//printf("%f ", x);
	sprite->setPosition(glm::vec2(x, y));
}

void Torch::render()
{
	sprite->render();
}

void Torch::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Torch::setPosition(const glm::vec2 &pos)
{
	posTorch = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posTorch.x), float(tileMapDispl.y + posTorch.y)));
}




