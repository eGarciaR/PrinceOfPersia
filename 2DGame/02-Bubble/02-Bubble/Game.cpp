#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"


void Game::init()
{
	bPlay = true;
	game_finished = game_over = false;
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	Scene::instance().init();
}

bool Game::update(int deltaTime)
{
	Scene::instance().update(deltaTime);
	
	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Scene::instance().render();
}

void Game::keyPressed(int key)
{
	if(key == 27) // Escape code
		bPlay = false;
	else if (key > 0 && Scene::instance().game_over){
		Scene::instance().restart_game();
	}
	else if (key > 0 && !Scene::instance().fin_intro){
		Scene::instance().stop_music();
		Scene::instance().fin_intro = true;
	}
	else if (key > 0 && Scene::instance().won()){
		Scene::instance().init();
	}
	keys[key] = true;
}

void Game::keyReleased(int key)
{
	keys[key] = false;
}

void Game::specialKeyPressed(int key)
{
	specialKeys[key] = true;
}

void Game::specialKeyReleased(int key)
{
	specialKeys[key] = false;
}

void Game::mouseMove(int x, int y)
{
}

void Game::mousePress(int button)
{
}

void Game::mouseRelease(int button)
{
}

bool Game::getKey(int key) const
{
	return keys[key];
}

bool Game::getSpecialKey(int key) const
{
	return specialKeys[key];
}





