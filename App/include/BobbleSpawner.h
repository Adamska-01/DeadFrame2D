#pragma once
#include "Stats.h"
#include <fstream>
#include <sdl.h>
#include <sstream>
#include <vector>


class Bobble;


class BobbleSpawner
{
private:
	std::vector<Bobble*> bobbles;

	SDL_TimerID timerID;
	
	SDL_TimerID timerID2;

	Stats* stats;
	
	float bobbleSpeed;
	
	float speedMultiplier;
	
	int startX;
	
	int Score;
	
	int PenaltyScore;


	void Blow(int index);


public:
	BobbleSpawner(int p_startX);

	~BobbleSpawner();


	void Update(float dt);
	
	void Draw();

	void Clean();

	void CollisionsWall(SDL_Rect wall, bool lavaWall = false);
	
	void CollisionsBobble(Bobble* bobble);

	void Spawn();
	
	void IncreaseSpeed();


public:
	void RemoveTimers();

	//Getters
	inline Stats* GetStats() { return stats; }
};