#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Torch.h"
#include "Game.h"

enum PlayerAnims
{
	INTRO1, INTRO2, INTRO3, INTRO4, INTRO5, INTRO6, INTRO7
};


void Intro::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	spritesheet.loadFromFile("images/intro.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(320, 200), glm::vec2(1, 0.125), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(8);

	sprite->setAnimationSpeed(INTRO1, 8);
	sprite->addKeyframe(INTRO1, glm::vec2(0.f, 0.0f));
	sprite->setSpeed(INTRO1, glm::vec2(0, 0));

	sprite->setAnimationSpeed(INTRO2, 8);
	sprite->addKeyframe(INTRO2, glm::vec2(0.f, 0.125f));
	sprite->setSpeed(INTRO2, glm::vec2(0, 0));

	sprite->setAnimationSpeed(INTRO3, 8);
	sprite->addKeyframe(INTRO3, glm::vec2(0.f, 0.250f));
	sprite->setSpeed(INTRO3, glm::vec2(0, 0));

	sprite->setAnimationSpeed(INTRO4, 8);
	sprite->addKeyframe(INTRO4, glm::vec2(0.f, 0.375f));
	sprite->setSpeed(INTRO4, glm::vec2(0, 0));

	sprite->setAnimationSpeed(INTRO5, 8);
	sprite->addKeyframe(INTRO5, glm::vec2(0.f, 0.500f));
	sprite->setSpeed(INTRO5, glm::vec2(0, 0));

	sprite->setAnimationSpeed(INTRO6, 8);
	sprite->addKeyframe(INTRO6, glm::vec2(0.f, 0.625f));
	sprite->setSpeed(INTRO6, glm::vec2(0, 0));

	sprite->setAnimationSpeed(INTRO7, 8);
	sprite->addKeyframe(INTRO7, glm::vec2(0.f, 0.750f));
	sprite->setSpeed(INTRO7, glm::vec2(0, 0));
	
	sprite->changeAnimation(0);

}

void Intro::update(int deltaTime, int x, int y)
{
	if (Scene::instance().getCurrentTime() >= 7000.0f && Scene::instance().getCurrentTime() <= 13017.0f){
		sprite->changeAnimation(INTRO2); // fondo autor
	}
	else if (Scene::instance().getCurrentTime() >= 13018.0f && Scene::instance().getCurrentTime() <= 18017.0f){
		sprite->changeAnimation(INTRO1); //fondo sin nada
	}
	else if (Scene::instance().getCurrentTime() >= 18018.0f && Scene::instance().getCurrentTime() <= 23017.0f){
		sprite->changeAnimation(INTRO3); //fondo prince
	}
	else if (Scene::instance().getCurrentTime() >= 23018.0f && Scene::instance().getCurrentTime() <= 28017.0f){
		sprite->changeAnimation(INTRO1); //fondo prince
	}
	else if (Scene::instance().getCurrentTime() >= 28018.0f && Scene::instance().getCurrentTime() <= 33017.0f){
		sprite->changeAnimation(INTRO4);
	}
	else if (Scene::instance().getCurrentTime() >= 33018.0f && Scene::instance().getCurrentTime() <= 38017.0f){
		sprite->changeAnimation(INTRO5); // primer mensaje
	}
	else if (Scene::instance().getCurrentTime() >= 38018.0f && Scene::instance().getCurrentTime() <= 43017.0f){
		sprite->changeAnimation(INTRO6); // segundo mensaje
	}
	else if (Scene::instance().getCurrentTime() >= 43018.0f && Scene::instance().getCurrentTime() <= 48017.0f){
		sprite->changeAnimation(INTRO7); // tercer mensaje
	}
	else if (Scene::instance().getCurrentTime() >= 48018.0f) Scene::instance().fin_intro = true;

	sprite->update(deltaTime);
	sprite->setPosition(glm::vec2(x, y));
}

void Intro::render()
{
	sprite->render();
}

void Intro::setPosition(const glm::vec2 &pos)
{
	posIntro = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posIntro.x), float(tileMapDispl.y + posIntro.y)));
}


