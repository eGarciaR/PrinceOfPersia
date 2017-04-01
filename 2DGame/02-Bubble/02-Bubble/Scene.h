#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Torch.h"
#include <windows.h> 


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

	static Scene &instance()
	{
		static Scene S;

		return S;
	}

	void init();
	void update(int deltaTime);
	void render();
	string getLevel();
	void setLevel(string s, glm::vec2 &pos);
	void setAntorcha(glm::ivec2 &pos);
	void clear_torchs();

private:
	void initShaders();

private:
	TileMap *map;
	Player *player;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	string level;
	vector<glm::ivec2> antorchas_pos;
	vector<Torch> torchs;
};


#endif // _SCENE_INCLUDE

