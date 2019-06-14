#ifndef ATTACKING_BOT_H
#define ATTACKING_BOT_H

#include <stdlib.h>     /* srand, rand */
#include <time.h>

#include <vector>
#include <string>

#include "sampgdk.h"
#include "tick.h"

class DamageObject;



const float SPAWN_DEPTH = 5.0;
const int MAX_HEALTH = 250;

struct Area
{
	float xMin;
	float xMax;
	float yMin;
	float yMax;
	float zMin;
	float zMax;
};

struct Position
{
	float x;
	float y;
	float z;

	Position()
	{
		x = float(0.0);
		y = float(0.0);
		z = float(0.0);
	}
};

class AttackBot
{
private:
	int botid;
	int modelid;
	Area area;
	bool isMoving = false;
	bool isDead = false;
	long long deathTime = 0;
	int deathWait = 12000;
	long long startMoveTime = 0;
	int moveWait = 1500;
	int speed = 10;
	int health;
	int targetid;
	std::vector<DamageObject> projectiles;
	
public:
	

	AttackBot();
	AttackBot(int modelid, Area area);
	void setSpeed(int speed);
	int getBotID();
	int getHealth();
	void moveInArea();
	void destroy(std::vector<AttackBot>& botList);
	void damage(int playerid, float damage, std::vector<AttackBot>& botList);
	Position randomPosInArea();
	void OnMoved();
	void OnUpdate();
	float randomRange(float min, float max);
	bool isPlayerInArea(int playerid);
	void targetPlayer(int playerid);
	void attackTarget();
	std::vector<int> scanArea();
	void selectRandomTarget();
};

#endif