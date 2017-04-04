#ifndef _TILE_MAP_INCLUDE
#define _TILE_MAP_INCLUDE


#include <glm/glm.hpp>
#include "Texture.h"
#include "ShaderProgram.h"
#include <windows.h> 
#include <conio.h>
#include <stdio.h>
#include "ShaderProgram.h"
#include <vector>
#include "TileChange.h"
using namespace std;

// Class Tilemap is capable of loading a tile map from a text file in a very
// simple format (see level01.txt for an example). With this information
// it builds a single VBO that contains all tiles. As a result the render
// method draws the whole map independently of what is visible.


class TileMap
{

public:
	// Tile maps can only be created inside an OpenGL context
	static TileMap *createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program);

	TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program);
	~TileMap();

	void render() const;
	void render_col() const;
	void free();
	
	int getTileSize() const { return tileSize; }

	bool collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size) const;
	bool collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size) const;
	bool collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY) const;
	bool collisionMoveUp(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY, bool right) const;
	bool collisionClimb(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY, bool right) const;
	bool collisionTrap(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY) const;
	bool collisionWith(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY, int tile, glm::ivec2 &posTile) const;
	void changeTile(const glm::ivec2 &pos, int tile, ShaderProgram &shaderProgram);
	bool collisionDoor(const glm::ivec2 &pos, const glm::ivec2 &size, bool right) const;
	vector <TileChange> getTraps();
	void changeTileTrap(int pos, int newTile);
	
private:
	bool loadLevel(const string &levelFile);
	void prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program, int x);

private:
	GLuint vao;
	GLuint vbo;
	GLint posLocation, texCoordLocation;
	glm::ivec2 position, mapSize, tilesheetSize;
	int tileSize, blockSize;
	Texture tilesheet;
	glm::vec2 tileTexSize;
	int *map;
	vector< TileChange> traps;
	vector< glm::ivec2> doors;

};


#endif // _TILE_MAP_INCLUDE


