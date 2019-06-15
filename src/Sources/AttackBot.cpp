#include "AttackBot.h"
#include "DamageObject.h"
#include "streamer.hpp"







AttackBot::AttackBot()
{

	health = MAX_HEALTH;
}

AttackBot::AttackBot(int modelid, Area area)
{
	srand((unsigned int)time(NULL));

	this->modelid = modelid;
	this->area = area;

	health = MAX_HEALTH;

	Position randPos = randomPosInArea();
	
	//this->botid = CreateObject(modelid, randPos.x, randPos.y, randPos.z - SPAWN_DEPTH, float(0.0), float(0.0), float(0.0), float(0.0));
	this->botid = Plugins::Streamer::Object::Create(modelid, randPos.x, randPos.y, randPos.z - SPAWN_DEPTH, float(0.0), float(0.0), float(0.0), 0, 0, -1, 300.0, 0.0, -1, 0);
	//sampgdk::logprintf("Bot ID: %d has been created!", botid);
}

void AttackBot::setSpeed(int speed)
{
	this->speed = speed;
}

int AttackBot::getBotID()
{
	return botid;
}

int AttackBot::getHealth()
{
	return health;
}

void AttackBot::moveInArea()
{
	Position newpos = randomPosInArea();

	isMoving = true;

	Plugins::Streamer::Object::Move(botid, newpos.x, newpos.y, newpos.z, float(speed), float(0.0), float(0.0), float(0.0));
	//MoveObject(botid, newpos.x, newpos.y, newpos.z, float(speed), float(0.0), float(0.0), float(0.0));
	//sampgdk::logprintf("Bot ID: %d started moving. to %f %f %f", botid, newpos.x, newpos.y, newpos.z);
}


void AttackBot::damage(int playerid, float damage, std::vector<AttackBot>& botList)
{
	std::string message;
	health -= int(damage);

	if (health <= 0)
	{
		message = "Destroyed Bot: " + std::to_string(botid);
		
		SendClientMessage(playerid, -1, message.c_str());

		// Remove this object from the vector.
		destroy(botList);

	}
}

void AttackBot::destroy(std::vector<AttackBot>& botList)
{
	AttackBot temp;

	// On bot death, destroy its lasers.
	while(projectiles.size() > 0)
	{
		projectiles[0].destroy(projectiles);
	}

	Position pos = randomPosInArea();

	pos.z = area.zMin - SPAWN_DEPTH;

	// Instead of destorying the object, hide it.
	Plugins::Streamer::Object::SetPos(botid, pos.x, pos.y, pos.z);
	//DestroyObject(botid);
	isDead = true;
	isMoving = false;
	deathTime = getCurrentTick();

	/*
	for (size_t i = 0; i < botList.size(); i++)
	{
		if (botList[i].botid == botid)
		{
			DestroyObject(botid);
			
			temp = botList[i];
			botList[i] = botList[botList.size() - 1];
			botList[botList.size() - 1] = temp;

			botList.pop_back();

			break;
		}
	}
	*/

}

Position AttackBot::randomPosInArea()
{
	Position newpos;

	newpos.x = randomRange(area.xMin, area.xMax);
	newpos.y = randomRange(area.yMin, area.yMax);
	newpos.z = randomRange(area.zMin, area.zMax);

	return newpos;

}

void AttackBot::OnMoved()
{
	isMoving = false;
	startMoveTime = getCurrentTick();
	//sampgdk::logprintf("Bot ID: %d stopped moving.", botid);
}

void AttackBot::OnUpdate()
{
	/*
	if (isMoving && !IsObjectMoving(botid))
		OnMoved();
	*/ 

	if (isDead)
	{
		//SendClientMessageToAll(-1, "Dead");
		// If the bot has been dead for deathWait
		if (getCurrentTick() - deathTime > deathWait)
		{
			//SendClientMessageToAll(-1, "REVIVED");
			// Revive!!!
			Position randPos = randomPosInArea();

			//this->botid = CreateObject(modelid, randPos.x, randPos.y, randPos.z - SPAWN_DEPTH, float(0.0), float(0.0), float(0.0), float(0.0));
			isDead = false;
			health = MAX_HEALTH;
		}
		else
			return;
	}

	for (size_t i = 0; i < projectiles.size(); i++)
	{
		projectiles[i].onDamageObjectUpdate(projectiles);
	}
	
	if (!isMoving)
	{
		

		if (getCurrentTick() - startMoveTime > moveWait)
		{
			moveInArea();

			selectRandomTarget();

			attackTarget();
		}
	}

	
}

float AttackBot::randomRange(float min, float max)
{
	int intMin = int(min);
	int intMax = int(max);
	int randInt;

	

	randInt = rand() % (intMax - intMin + 1) + intMin;

	return float(randInt);
}



// Return whether the given player is in the bot's spawn area.
bool AttackBot::isPlayerInArea(int playerid)
{
	Position pos;

	
	GetPlayerPos(playerid, &pos.x, &pos.y, &pos.z);

	//std::string message = "(x, y, z) => (" + std::to_string(pos.x) + ", " + std::to_string(pos.y) + ", " + std::to_string(pos.z) + ")";
	//SendClientMessage(playerid, -1, message.c_str());

	if (pos.x >= area.xMin && pos.x <= area.xMax &&
		pos.y >= area.yMin && pos.y <= area.yMax &&
		pos.z >= area.zMin && pos.z <= area.zMax)
		return true;
	else
		return false;
}

void AttackBot::targetPlayer(int playerid)
{
	// Have the bot target this player.
	this->targetid = playerid;

	return;
}

void AttackBot::attackTarget()
{
	Position start;
	Position end;

	if (targetid == INVALID_PLAYER_ID)
		return;

	Plugins::Streamer::Object::GetPos(botid, &start.x, &start.y, &start.z);
	//GetObjectPos(botid, &start.x, &start.y, &start.z);

	GetPlayerPos(targetid, &end.x, &end.y, &end.z);

	DamageObject laser(18647, start, end, float(5.0), 3000, 1, float(1.5));

	projectiles.push_back(laser);


}

std::vector<int> AttackBot::scanArea()
{
	std::vector<int> playersInArea;

	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		if (!IsPlayerConnected(i))
			continue;
		//SendClientMessageToAll(-1, "Player online");
		if (isPlayerInArea(i))
		{
			//SendClientMessageToAll(-1, "Player found");
			playersInArea.push_back(i);
		}
	}

	return playersInArea;
}

void AttackBot::selectRandomTarget()
{
	std::vector<int> playerList;
	int index;

	playerList = scanArea();


	//SendClientMessageToAll(-1, std::to_string(playerList.size()).c_str());

	if (playerList.size() != 0)
	{
		index = rand() % playerList.size();

		targetPlayer(playerList[index]);
	}
	else
	{
		targetPlayer(INVALID_PLAYER_ID);
	}

	return;
}


