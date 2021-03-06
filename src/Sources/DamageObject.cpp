#include "DamageObject.h"
#include "AttackBot.h"
#include "streamer.hpp"
#include "sampgdk.h"
#include "amx_var.h"

int callWC_DamagePlayer(int playerid, float amount, int issuerid, int weaponid, int bodypart);

DamageObject::DamageObject()
{
	
}

DamageObject::DamageObject(AttackBot bot, float damage, int duration, int durability, float range)
{
	isAttackBot = true;

	objectid = bot.getBotID();
	this->damage = damage;
	this->duration = duration;
	this->durability = durability;
	this->range = range;
	createTime = getCurrentTick();

}

DamageObject::DamageObject(int modelid, Position spawn, Position end, float damage, int duration, int durability, float range)
{
	float rotX = calculateXRotation(spawn, end);
	float rotY = 0.0;
	float rotZ = calculateZRotation(spawn, end);

	float speed = 155.0;

	//std::string message = "rotX: " + std::to_string(rotX) + " | rotY: " + std::to_string(rotY) + " | rotZ: " + std::to_string(rotZ);
	//int size = message.length();

	//SendClientMessageToAll(-1, std::to_string(size).c_str());

	//SendClientMessageToAll(-1, message.c_str());

	if (abs(rotX) > 16.00)
	{
		modelid = 18728;
		speed = 10.0;
		durability = 3;
		duration = 10000;
		damage = 12.0;
	}
	// 18647
	objectid = Plugins::Streamer::Object::Create(modelid, spawn.x, spawn.y, spawn.z, rotX, rotY, rotZ, 0, 0, -1, 300.0, 0.0, -1, 0);
	//objectid = CreateObject(modelid, spawn.x, spawn.y, spawn.z, rotX, rotY, rotZ, float(0.0));
	this->damage = damage;
	this->duration = duration;
	this->durability = durability;
	this->range = range;
	createTime = getCurrentTick();

	Plugins::Streamer::Object::Move(objectid, end.x, end.y, end.z, speed, rotX, rotY, rotZ);
}

float DamageObject::calculateXRotation(Position start, Position end)
{
	float rotX;

	float xDiff = end.x - start.x;
	float yDiff = end.y - start.y;
	float zDiff = end.z - start.z;
	float distanceXY = sqrt(pow(xDiff, 2.0) + pow(yDiff, 2.0));

	float degrees;

	if (distanceXY == 0)
	{
		if (zDiff > 0)
			rotX = 90.0;
		else
			rotX = -90.0;

		return rotX;
	}
	else
	{
		degrees = float(atan(zDiff / distanceXY)) * (float(180.0) / PI);
	}

	if (zDiff > 0)
		rotX = degrees;
	else
		rotX = degrees;


	return rotX;
}

float DamageObject::calculateZRotation(Position start, Position end) 
{
	float rotZ;

	float xDiff = end.x - start.x;
	float yDiff = end.y - start.y;
	float zDiff = end.z - start.z;

	float degrees;

	if (yDiff == 0)
	{
		if (xDiff > 0)
			rotZ = 270.0;
		else
			rotZ = 90.0;

		return rotZ;
	}
	else
	{
		degrees = float(atan(xDiff / yDiff)) * (float(180) / PI);
	}

	if (xDiff > 0 && yDiff > 0)
		rotZ = float(360.0) - degrees;
	else if (xDiff > 0 && yDiff < 0)
		rotZ = float(180.0) - degrees;
	else if (xDiff < 0 && yDiff < 0)
		rotZ = float(180.0) - degrees;
	else if (xDiff < 0 && yDiff > 0)
		rotZ = -1 * degrees;
	else if (xDiff == 0 && yDiff > 0)
		rotZ = float(0.0);
	else // xDiff == 0 && yDiff < 0
		rotZ = float(180.0);

	return rotZ;
}

void DamageObject::onDamageObjectUpdate(std::vector<DamageObject>& objectList)
{
	Position objectPos;
	Position playerPos;

	Plugins::Streamer::Object::GetPos(objectid, &objectPos.x, &objectPos.y, &objectPos.z);
	

	//std::string message = "laser (x, y, z) = (" + std::to_string(objectPos.x) + ", " + std::to_string(objectPos.y) + ", " + std::to_string(objectPos.z);
	//SendClientMessageToAll(-1, message.c_str());

	// Find players within their range to damage them.
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		if (!IsPlayerConnected(i))
			continue;
		//SendClientMessageToAll(-1, "Going thorugh players...");
		
		if (IsPlayerInRangeOfPoint(i, range, objectPos.x, objectPos.y, objectPos.z))
			damagePlayer(i, objectList);
	}

	// If this object has been around for longer than its duration...
	if (getCurrentTick() - createTime > duration)
	{
		// Destroy the object...
		destroy(objectList);
	}

	return;

}

// public WC_DamagePlayer(playerid, Float:amount, issuerid, weaponid, bodypart);
void DamageObject::damagePlayer(int playerid, std::vector<DamageObject>& objectList)
{
	//SendClientMessage(playerid, -1, "Damaged");
	
	//callWC_DamagePlayer(playerid, (float)damage, INVALID_PLAYER_ID, 21, 0);
	float health;
	float armor;

	GetPlayerHealth(playerid, &health);
	GetPlayerArmour(playerid, &armor);

	if (health <= 0.0)
	{
		//SetPlayerHealth(playerid, 0.0);	
		return;
	}	
	else if (armor > 0.0)
		if(armor - (float)damage <= 0.0)
			SetPlayerArmour(playerid, 0.0);
		else
			SetPlayerArmour(playerid, armor - (float)damage);
	else
	{
		
		if(health - (float)damage <= 0.0)
			SetPlayerHealth(playerid, 0.0);
		else
			SetPlayerHealth(playerid, health - (float)damage);
	}

	durability--;

	// If the object's durability has depleated...
	if (durability == 0)
		// Destroy the object..
		destroy(objectList);

	return;

}

void DamageObject::destroy(std::vector<DamageObject>& objectList)
{
	DamageObject temp;

	for (size_t i = 0; i < objectList.size(); i++)
	{
		if (objectList[i].objectid == objectid)
		{
			Plugins::Streamer::Object::Destroy(objectid);

			temp = objectList[i];
			objectList[i] = objectList[objectList.size() - 1];
			objectList[objectList.size() - 1] = temp;

			objectList.pop_back();

			break;
		}
	}

	return;
}


int callWC_DamagePlayer(int playerid, float amount, int issuerid, int weaponid, int bodypart)
{
	//std::string params = std::to_string(playerid) + " " + std::to_string(amount) + " " + std::to_string(issuerid) + " " + std::to_string(weaponid) + " " + std::to_string(bodypart);
	//SendClientMessageToAll(-1, params.c_str());
	/*
	SendClientMessageToAll(-1, "WC+Called.");
	if (playerid == NULL && playerid != 0)
		return 0;
	SendClientMessageToAll(-1, "Valid playerid");
	if (amount == NULL)
		return 0;
	SendClientMessageToAll(-1, "Valid damage");
	if (issuerid == NULL)
		return 0;
	SendClientMessageToAll(-1, "Valid issuerid");
	if (weaponid == NULL)
		return 0;
	SendClientMessageToAll(-1, "Valid weaponid");
	if (bodypart == NULL)
		return 0;
	SendClientMessageToAll(-1, "Valid bodypart");
	*/
	int idx;
	cell ret;
	//cell amx_Address;
	//cell* phys_addr;
	//SendClientMessageToAll(-1, "Calling....");

	//std::string mess = "Size: " + std::to_string(vAMX.size());

	//sampgdk::logprintf(mess.c_str());

	int amxerr = amx_FindPublic(vAMX[LWLVDM_AMX], "WC_DamagePlayer", &idx);

	if (amxerr == AMX_ERR_NONE)
	{
		//SendClientMessageToAll(-1, "Success.");
		amx_Push(vAMX[LWLVDM_AMX], bodypart);
		amx_Push(vAMX[LWLVDM_AMX], weaponid);
		amx_Push(vAMX[LWLVDM_AMX], issuerid);
		amx_Push(vAMX[LWLVDM_AMX], amx_ftoc(amount));
		amx_Push(vAMX[LWLVDM_AMX], playerid);
		amx_Exec(vAMX[LWLVDM_AMX], &ret, idx);

		return 1;
	}

	//SendClientMessageToAll(-1, "Error.");

	return 0;
}


