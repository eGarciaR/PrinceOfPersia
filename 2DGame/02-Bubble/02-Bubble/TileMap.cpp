#include <iostream>
#include <fstream>
#include <sstream>
#include "TileMap.h"
#include "Scene.h"
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>



TileMap *TileMap::createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{
	TileMap *map = new TileMap(levelFile, minCoords, program);
	
	return map;
}


TileMap::TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{
	loadLevel(levelFile);
	if (strcmp(levelFile.c_str(), "levels/game_over.txt") == 0) prepareArrays(minCoords, program,4);
	else prepareArrays(minCoords, program, 5);
}

TileMap::~TileMap()
{
	if(map != NULL)
		delete map;
}


void TileMap::render() const
{
	glEnable(GL_TEXTURE_2D);
	tilesheet.use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6 * mapSize.x * mapSize.y);
	glDisable(GL_TEXTURE_2D);
}


void TileMap::free()
{
	glDeleteBuffers(1, &vbo);
}

bool TileMap::loadLevel(const string &levelFile)
{
	ifstream fin;
	string line, tilesheetFile;
	stringstream sstream;
	char tile;
	bool previousNumber = false;
	
	fin.open(levelFile.c_str());
	if(!fin.is_open())
		return false;
	getline(fin, line);
	if(line.compare(0, 7, "TILEMAP") != 0)
		return false;
	getline(fin, line);
	sstream.str(line);
	sstream >> mapSize.x >> mapSize.y;
	getline(fin, line);
	sstream.str(line);
	sstream >> tileSize >> blockSize;
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetFile;
	tilesheet.loadFromFile(tilesheetFile, TEXTURE_PIXEL_FORMAT_RGBA);
	tilesheet.setWrapS(GL_CLAMP_TO_EDGE);
	tilesheet.setWrapT(GL_CLAMP_TO_EDGE);
	tilesheet.setMinFilter(GL_NEAREST);
	tilesheet.setMagFilter(GL_NEAREST);
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetSize.x >> tilesheetSize.y;
	tileTexSize = glm::vec2(1.f / tilesheetSize.x, 1.f / tilesheetSize.y);
	
	if (levelFile[7] == 'p') {
		Scene::instance().clear_torchs();
		Scene::instance().clear_doors();
	}
	map = new int[mapSize.x * mapSize.y];
	for(int j=0; j<mapSize.y; j++)
	{
		for(int i=0; i<(mapSize.x*2); i++)
		{
			fin.get(tile);

			if (tile == ' ') {
				previousNumber = false;
			}
			else {
				if (previousNumber) {
					int previous = map[j*mapSize.x + (i-1)/2];
					map[j*mapSize.x + (i/2)] = (previous * 10) + (tile - int('0'));
					previousNumber = false;
					if (map[j*mapSize.x + (i / 2)] == 21) {
						TileChange t;
						t.x = i / 2; t.y = j; t.tile = 21; t.status = 21;
						traps.push_back(t);
					}
					else if (map[j*mapSize.x + (i / 2)] == 14 || map[j*mapSize.x + (i / 2)] == 16 && (i / 2) < 10) {
						Scene::instance().setDoor(glm::ivec2(i / 2, j));
					}
				}
				else  {
					if ((tile - int('0')) == 6 && strcmp(levelFile.c_str(),"levels/game_over.txt") != 0) {
						Scene::instance().setAntorcha(glm::ivec2(i / 2, j));
					}
					map[j*mapSize.x + (i / 2)] = tile - int('0');

					previousNumber = true;
				}
			}
		}
		fin.get(tile);
#ifndef _WIN32
		fin.get(tile);
#endif
	}
	fin.close();
	
	return true;
}

void TileMap::prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program, int x)
{
	int tile, nTiles = 0;
	glm::vec2 posTile, texCoordTile[2], halfTexel;
	vector<float> vertices;
	
	halfTexel = glm::vec2(0.5f / tilesheet.width(), 0.5f / tilesheet.height());
	for(int j=0; j<mapSize.y; j++)
	{
		for(int i=0; i<mapSize.x; i++)
		{
			tile = map[j * mapSize.x + i];
			if(tile != 0)
			{
				// Non-empty tile
				nTiles++;
				posTile = glm::vec2(minCoords.x + i * tileSize, minCoords.y + j * 64);
				texCoordTile[0] = glm::vec2(float((tile-1)%x) / tilesheetSize.x, float((tile-1)/x) / tilesheetSize.y);//o /5 o /2
				texCoordTile[1] = texCoordTile[0] + tileTexSize;
				//texCoordTile[0] += halfTexel;
				texCoordTile[1] -= halfTexel;
				// First triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				// Second triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				vertices.push_back(posTile.x); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[1].y);
			}
		}
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * nTiles * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	posLocation = program.bindVertexAttribute("position", 2, 4*sizeof(float), 0);
	texCoordLocation = program.bindVertexAttribute("texCoord", 2, 4*sizeof(float), (void *)(2*sizeof(float)));
}

void TileMap::changeTile(const glm::ivec2 &pos, int tile, ShaderProgram &shaderProgram) {
	map[pos.y*mapSize.x + pos.x] = tile;
	prepareArrays(glm::vec2(0, 0), shaderProgram, 5);
}

void TileMap::changeTileTrap(int pos, int newTile){
	traps[pos].status = newTile;
}
// Collision tests for axis aligned bounding boxes.
// Method collisionMoveDown also corrects Y coordinate if the box is
// already intersecting a tile below.

bool TileMap::collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size) const
{
	int x, y0, y1;
	
	x = pos.x / tileSize;
	y0 = (pos.y + 8) / 64;
	y1 = ((pos.y + 8) + size.y - 1) / 64;
	for(int y=y0; y<=y1; y++)
	{
		if (map[y*mapSize.x + x] == 2 || map[y*mapSize.x + x] == 3 || map[y*mapSize.x + x] == 13)
				return true;
	}
	
	return false;
}

bool TileMap::collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size) const
{
	int x, y0, y1;
	
	x = (pos.x + size.x - 1) / tileSize;
	y0 = (pos.y+8) / 64;
	y1 = ((pos.y+8) + size.y+ - 1) / 64;
	
	for(int y=y0; y<=y1; y++)
	{
		if (map[y*mapSize.x + x] == 2 || map[y*mapSize.x + x] == 13 || map[y*mapSize.x + x] == 3)
			return true;
	}
	
	return false;
}

bool TileMap::collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY, int level) const
{
	int x0, x1, y;
	x0 = (pos.x+16) / tileSize;
	x1 = ((pos.x) + size.x - 1) / tileSize;
	y = (pos.y + size.y - 1) / 64;
	
	for(int x=x0; x<=x1; x++)
	{
		if (level == 1){
			if (map[y*mapSize.x + x] != 4 && map[y*mapSize.x + x] != 9 && map[y*mapSize.x + x] != 11 && map[y*mapSize.x + x] != 19 && map[y*mapSize.x + x] != 12 && map[y*mapSize.x + x] != 20 && map[y*mapSize.x + x] != 25)
			{
				if (*posY - 64 * y + size.y <= 4)
				{
					*posY = 64 * y - size.y;
					return true;
				}
			}
		}
		else{
			if (map[y*mapSize.x + x] != 4 && map[y*mapSize.x + x] != 9 && map[y*mapSize.x + x] != 11 && map[y*mapSize.x + x] != 19 && map[y*mapSize.x + x] != 12 && map[y*mapSize.x + x] != 20 && map[y*mapSize.x + x] != 23 && map[y*mapSize.x + x] != 24 && map[y*mapSize.x + x] != 25)
			{
				if (*posY - 64 * y + size.y <= 4)
				{
					*posY = 64 * y - size.y;
					return true;
				}
			}
		}
	}
	return false;
}

bool TileMap::collisionMoveUp(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY, bool right) const
{
	int x0, x1, y;
	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y = pos.y / 64;
	if (right){
		if (map[y*mapSize.x + x1] != 4 && map[y*mapSize.x + x1] != 9 && map[y*mapSize.x + x1] != 11 && map[y*mapSize.x + x1] != 12 && map[y*mapSize.x + x1] != 19 && map[y*mapSize.x + x1] != 20)
		{

			if (*posY - 64 * y <= 56) return true;
		}
	}
	else{
		if (map[y*mapSize.x + x0] != 4 && map[y*mapSize.x + x0] != 9 && map[y*mapSize.x + x0] != 11 && map[y*mapSize.x + x0] != 12 && map[y*mapSize.x + x0] != 19 && map[y*mapSize.x + x0] != 20)
		{

			if (*posY - 64 * y <= 56)return true;
		}
	}
	return false;
}

bool TileMap::collisionClimb(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY, bool right) const
{
	int x0, x1, y;
	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y = pos.y / 64;
	if (right){
		if ((map[y*mapSize.x + x0] == 9 || map[y*mapSize.x + x0] == 11 || map[y*mapSize.x + x0] == 12 || map[y*mapSize.x + x0] == 4 || map[y*mapSize.x + x0] == 19 || map[y*mapSize.x + x0] == 20) && (map[y*mapSize.x + x1] == 5 || map[y*mapSize.x + x1] == 7 || map[y*mapSize.x + x1] == 6 || map[y*mapSize.x + x1] == 1 || map[y*mapSize.x + x1] == 17 || map[y*mapSize.x + x1] == 16 || map[y*mapSize.x + x1] == 14))
		{
			if (*posY - 64 * y >= 56)
			{
				return true;
			}
		}
	}
	else{
		if ((map[y*mapSize.x + x1] == 9 || map[y*mapSize.x + x1] == 11 || map[y*mapSize.x + x1] == 4 || map[y*mapSize.x + x1] == 12 || map[y*mapSize.x + x1] == 19 || map[y*mapSize.x + x1] == 20) && (map[y*mapSize.x + x0] == 7 || map[y*mapSize.x + x0] == 6 || map[y*mapSize.x + x0] == 1 || map[y*mapSize.x + x0] == 17 || map[y*mapSize.x + x0] == 5 || map[y*mapSize.x + x0] == 16 || map[y*mapSize.x + x0] == 14))
		{
			if (*posY - 64 * y >= 56)
			{
				return true;
			}
		}
	}
	return false;
}


bool TileMap::collisionTrap(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY) const
{
	int x0, x1, y;
	x0 = (pos.x + 16) / tileSize;
	x1 = ((pos.x) + size.x - 1) / tileSize;
	y = (pos.y+64 + size.y - 1) / 64;
	for (int x = x0; x <= x1; x++)
	{
		if (map[y*mapSize.x + x] == 15)
		{
			if (*posY - 64 * y + size.y <= 4)
			{
				*posY = 64 * y - size.y;
				return true;
			}
		}
	}
	return false;
}

bool TileMap::collisionWith(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY, int tile, glm::ivec2 &posTile) const
{
	int x0, x1, y;
	x0 = (pos.x + 16) / tileSize;
	x1 = ((pos.x) + size.x - 1) / tileSize;
	y = (pos.y + 64 + size.y - 1) / 64;
	for (int x = x0; x <= x1; x++)
	{
		if (map[y*mapSize.x + x] == tile)
		{
			if (*posY - 64 * y + size.y <= 4)
			{
				*posY = 64 * y - size.y;
				posTile.x = x;
				posTile.y = y;
				return true;
			}
		}
	}
	return false;
}

bool TileMap::collisionDoor(const glm::ivec2 &pos, const glm::ivec2 &size, bool right) const
{
	int x, y0, y1;
	if (right){
		x = (pos.x + size.x - 1) / tileSize;
		y0 = (pos.y + 8) / 64;
		y1 = ((pos.y + 8) + size.y + -1) / 64;
	}
	else{
		x = pos.x / tileSize;
		y0 = (pos.y + 8) / 64;
		y1 = ((pos.y + 8) + size.y - 1) / 64;
	}
	for (int y = y0; y <= y1; y++)
	{
		if (map[y*mapSize.x + x] == 14 || map[y*mapSize.x + x] == 16)
			return true;
	}

	return false;
}
vector <TileChange> TileMap::getTraps(){
	return traps;
}






























