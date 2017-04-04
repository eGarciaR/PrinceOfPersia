#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"


#define SCREEN_X 0
#define SCREEN_Y 0

#define INIT_PLAYER_X_TILES 2
#define INIT_PLAYER_Y_TILES 0


Scene::Scene()
{
	map = NULL;
	col = NULL;
	player = NULL;
	clear_torchs();
	clear_doors();
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
	if(player != NULL)
		delete player;
	if(torchs.size() <1) clear_torchs();
}


void Scene::init()
{
	initShaders();
	enemyVisible = false;
	game_over = false;
	doorOpened = false;
	level = "levels/prince-map1.txt";
	map = TileMap::createTileMap("levels/prince-map1.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	col = TileMap::createTileMap("levels/col1.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	player = new Player();
	enemy = new Enemy();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);
	//play_music("PoP_music.wav", true);
	//Antorchas
	for (int i = 0; i < antorchas_pos.size(); ++i) {
		Torch *torch = new Torch();
		torch->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		if (antorchas_pos[i].y == 0) antorchas_pos[i].y = -1;
		else if (antorchas_pos[i].y == 2) antorchas_pos[i].y = 3;
		torch->setPosition(glm::vec2((((antorchas_pos[i].x + 1) * map->getTileSize()) + 9), ((antorchas_pos[i].y + 1) * map->getTileSize()) + 18));
		torch->setTileMap(map);
		torchs.push_back(*torch);
	}
	if (door_pos.size() >= 1) {
		Door *door = new Door();
		door->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		door->setPosition(glm::vec2((((door_pos[0].x) * map->getTileSize()) + 12), ((door_pos[0].y) * map->getTileSize()) + 0));
		door->setTileMap(map);
		doors.push_back(*door);
	}
	projection = glm::ortho(32.f, float(SCREEN_WIDTH+31), float(SCREEN_HEIGHT - 1), 0.f);
	currentTime = 0.0f;

}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	if (!game_over){
		player->update(deltaTime, texProgram);
		if (enemyVisible) enemy->update(deltaTime, player->getPosition());
		for (int i = 0; i < antorchas_pos.size(); ++i) {
			if (antorchas_pos[i].y == 0) antorchas_pos[i].y = -1;
			else if (antorchas_pos[i].y == 2) antorchas_pos[i].y = 3;
			(&torchs[i])->update(deltaTime, ((antorchas_pos[i].x + 1) * map->getTileSize()) + 9, ((antorchas_pos[i].y + 1) * map->getTileSize()) + 1);

		}
		if (door_pos.size() >= 1) (&doors[0])->update(deltaTime);
	}
}

void Scene::render()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	map->render();
	if (!game_over){
		for (int i = 0; i < antorchas_pos.size(); ++i) (&torchs[i])->render();
		if (door_pos.size() >= 1) (&doors[0])->render();
		player->render();
		if (enemyVisible) {
			enemy->render();
		}
		texProgram.use();
		texProgram.setUniformMatrix4f("projection", projection);
		texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
		modelview = glm::mat4(1.0f);
		texProgram.setUniformMatrix4f("modelview", modelview);
		texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
		col->render();
	}
}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if(!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}

string Scene::getLevel()
{
	return level;
}

void Scene::setLevel(string s, glm::vec2 &pos, string column, bool showEnemy, glm::ivec2 &posEnemy){
	initShaders();
	map = TileMap::createTileMap(s, glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	col = TileMap::createTileMap(column, glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(pos.x, pos.y));
	player->setTileMap(map);
	if (showEnemy) {
		enemy->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		enemy->setPosition(glm::ivec2(posEnemy.x, posEnemy.y));
		enemy->setTileMap(map);
		enemyVisible = true;
	}
	for (int i = 0; i < antorchas_pos.size(); ++i) {
		Torch *torch = new Torch();
		torch->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		torch->setPosition(glm::vec2(antorchas_pos[i].x * map->getTileSize(), antorchas_pos[i].y * map->getTileSize()));
		torch->setTileMap(map);
		torchs.push_back(*torch);
	}
	if (door_pos.size() >= 1) {
		Door *door = new Door();
		door->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		door->setPosition(glm::vec2(door_pos[0].x * map->getTileSize()+12, door_pos[0].y * map->getTileSize()));
		door->setTileMap(map);
		doors.push_back(*door);
	}
	projection = glm::ortho(32.f, float(SCREEN_WIDTH + 31), float(SCREEN_HEIGHT - 1), 0.f);
	currentTime = 0.0f;
	level = s;
}

void Scene::setAntorcha(glm::ivec2 &pos){
	antorchas_pos.push_back(pos);
}

void Scene::setDoor(glm::ivec2 &pos){
	door_pos.push_back(pos);
}

void Scene::clear_torchs(){
	torchs.clear();
	antorchas_pos.clear();
}

void Scene::clear_doors(){
	doors.clear();
	door_pos.clear();
	doorOpened = false;
}

void Scene::openDoor() {
	if (door_pos.size() >= 1 && !doorOpened) {
		(&doors[0])->changeAnimation();
		doorOpened = true;
	}
}

bool Scene::isDoorOpened() {
	return doorOpened;
}

void Scene::play_music(char *s, bool loop){
	if(loop) PlaySoundA(LPCSTR(s), NULL, SND_FILENAME | SND_LOOP | SND_ASYNC);
	else PlaySoundA(LPCSTR(s), NULL, SND_FILENAME | SND_ASYNC);
}

void Scene::stop_music(){
	PlaySoundA(NULL, 0, 0);
}

void Scene::set_game_over(){
	game_over = true;
	initShaders();
	map = TileMap::createTileMap("levels/game_over.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	projection = glm::ortho(32.f, float(SCREEN_WIDTH + 31), float(SCREEN_HEIGHT - 1), 0.f);
	currentTime = 0.0f;
	stop_music();
	play_music("game_over.wav", true);

}

void Scene::restart_game(){
	stop_music();
	play_music("PoP_music.wav", true);
	game_over = false;
	init();
}

void Scene::isEnemyVisible(bool visible) {
	enemyVisible = visible;
}