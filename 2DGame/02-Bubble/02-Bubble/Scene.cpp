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
	fin_intro = false;
	magicdoor = false;
	winner = false;
	stop_music();
	play_music("intro.wav", true);
	level = "levels/prince-map1.txt";
	map = TileMap::createTileMap("levels/prince-map1.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	col = TileMap::createTileMap("levels/col1.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	player = new Player();
	enemy = new Enemy();
	intro = new Intro();
	Magicdoor = new MagicDoor();
	Magicdoor->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	enemy->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, "images/enemy-sprite.png");
	intro->init(glm::ivec2(SCREEN_X-1, SCREEN_Y), texProgram);
	intro->setPosition(glm::ivec2(1 * map->getTileSize(), 0));
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);
	
	life = new Life();
	life->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	lifeEnemy = new LifeEnemy();
	lifeEnemy->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
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
		door->setPosition(glm::vec2((((door_pos[0].x) * map->getTileSize()) + 12), ((door_pos[0].y) * map->getTileSize())-6));
		door->setTileMap(map);
		doors.push_back(*door);
	}
	projection = glm::ortho(32.f, float(SCREEN_WIDTH+31), float(SCREEN_HEIGHT - 1), 0.f);
	currentTime = 0.0f;

}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	if (fin_intro && !winner){
		player->update(deltaTime, texProgram);
		if (enemyVisible) {
			enemy->update(deltaTime, player->getPosition());
		}
		for (int i = 0; i < antorchas_pos.size(); ++i) {
			if (antorchas_pos[i].y == 0) antorchas_pos[i].y = -1;
			else if (antorchas_pos[i].y == 2) antorchas_pos[i].y = 3;
			(&torchs[i])->update(deltaTime, ((antorchas_pos[i].x + 1) * map->getTileSize()) + 9, ((antorchas_pos[i].y + 1) * map->getTileSize()) + 1);

		}
		if (door_pos.size() >= 1) (&doors[0])->update(deltaTime);
		if (magicdoor) Magicdoor->update(deltaTime);
	}
	else if(!fin_intro && !winner)intro->update(deltaTime, 1*map->getTileSize(), 0);
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
	if (fin_intro && !winner){
		map->render();
		for (int i = 0; i < antorchas_pos.size(); ++i) (&torchs[i])->render();
		if (door_pos.size() >= 1) (&doors[0])->render();
		if (magicdoor) Magicdoor->render();
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
		life->render();
		if (enemyVisible) lifeEnemy->render();
	}
	else if(!fin_intro)intro->render();
	else if (winner) map->render();
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
		if(door_pos[0].y != 0) door->setPosition(glm::vec2(door_pos[0].x * map->getTileSize()+12, door_pos[0].y * map->getTileSize()+24));
		else door->setPosition(glm::vec2(door_pos[0].x * map->getTileSize()+12, door_pos[0].y * map->getTileSize()-6));
		door->setTileMap(map);
		doors.push_back(*door);
	}
	if (s == "levels/prince-map6.txt") {
		magicdoor = true;
		Magicdoor->setPosition(glm::vec2(map->getTileSize()*8 +20, map->getTileSize()*2 -15));
	}
	else if (s == "levels/palace-map11.txt") {
		magicdoor = true;
		Magicdoor->setPosition(glm::vec2(map->getTileSize() * 2 + 20 , map->getTileSize() * 2 - 15));
	}
	else if (s == "levels/palace-map1.txt"){
		Magicdoor->closeMagicDoor();
		magicdoor = true;
		Magicdoor->setPosition(glm::vec2(map->getTileSize() * 3 + 10, map->getTileSize() * 2 - 15));
	}
	else magicdoor = false;
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
	}
}

bool Scene::isDoorOpened() {
	doorOpened = (&doors[0])->opened();
	return doorOpened;
}

void Scene::changeHealthAnimation(int hp){
	life->changeAnimation(hp);
}

void Scene::changeHealthEnemyAnimation(int hp){
	lifeEnemy->changeAnimation(hp);
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
	changeHealthAnimation(4);
	stop_music();
	play_music("game_over.wav", true);

}

void Scene::restart_game(){
	stop_music();
	//play_music("PoP_music.wav", true);
	game_over = false;
	init();
	fin_intro = true;
}

void Scene::isEnemyVisible(bool visible) {
	enemyVisible = visible;
}

bool Scene::getEnemyVisible(){
	return enemyVisible;
}
void Scene::setEnemyCreated(bool created){
	enemy->setEnemyCreated(created);
}

bool Scene::isEnemyCreated(){
	return enemy->isEnemyCreated();
}
float Scene::getCurrentTime(){
	return currentTime;
}

glm::ivec2 Scene::getEnemyPosition(){
	return enemy->getEnemyPosition();
}

glm::ivec2 Scene::getPlayerPosition(){
	return player->getPosition();
}
void Scene::setEnemyPosition(glm::ivec2 pos){
	enemy->setEnemyPositon(pos);
}

int Scene::getPlayerAnimation(){
	return player->getPlayerAnimation();
}

void Scene::PlayerDamaged(int count){
	player->PlayerDamaged(count);
}

bool Scene::PlayerDied(){
	if(0 == player->getPlayerHp()) return true;
	return false;
}

bool Scene::EnemyDied(){
	if (0 == enemy->getEnemyHp()) return true;
	return false;
}

void Scene::resetEnemy(){
	enemy->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, "images/enemy2-sprite.png");
}

bool Scene::hasSword(){
	return player->hasSword();
}

bool Scene::isMagicDoor(){
	return magicdoor;
}

void Scene::openMagicDoor(){
	Magicdoor->changeAnimation();
}

bool Scene::magicDoorOpened(){
	return Magicdoor->magicDoorOpened();
}

void Scene::win(){
	stop_music();
	winner = true;
	play_music("won.wav", false);
	map = TileMap::createTileMap("levels/win.txt", glm::vec2(SCREEN_X-1, SCREEN_Y), texProgram);
	projection = glm::ortho(32.f, float(SCREEN_WIDTH-1), float(SCREEN_HEIGHT -15), 0.f);
	

	
}

bool Scene::won(){
	return winner;
}