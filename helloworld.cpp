#include <stdio.h>
#include <string.h>



#include "sampgdk.h"
#include <iostream>
#include <vector>
#include <string>

#include "AttackBot.h"
#include "DamageObject.h"

using namespace std;

void onBotMoved(int botid);
void onBotsUpdate();
float processDamage(int weaponid);

vector<AttackBot> attackbots;


void SAMPGDK_CALL PrintTickCountTimer(int timerid, void *params) {

	//sampgdk::logprintf("Tick count: %d", GetTickCount());
}

// Run every 50 milliseconds.
void SAMPGDK_CALL ScriptSync(int timerid, void* params)
{
	onBotsUpdate();

	return;
}

PLUGIN_EXPORT bool PLUGIN_CALL OnGameModeInit() {
	SetGameModeText("Attacking Bot Test");
	AddPlayerClass(0, 1958.3783f, 1343.1572f, 15.3746f, 269.1425f,
					0, 0, 0, 0, 0, 0);
	//SetTimer(1000, true, PrintTickCountTimer, 0);
	SetTimer(50, true, ScriptSync, 0);
	
	
	
	// Create the UFO
	//CreateObject(18846, 4726.4478, 134.1717, 12.2423, 18.00000, 30.00000, 285.00000, 0.0);

	Area area;
	area.xMin = float(4720.3354);
	area.xMax = float(4766.1201);
	area.yMin = float(134.1717);
	area.yMax = float(231.4489);
	area.zMin = float(11.2423);
	area.zMax = float(22.2423);

	AttackBot bot(18846, area);

	attackbots.push_back(bot);
  
	return true;
}

PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerConnect(int playerid) {
  SendClientMessage(playerid, 0xFFFFFFFF, "Welcome to the HelloWorld server!");
  return true;
}

PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerRequestClass(int playerid,
                                                    int classid) {
  SetPlayerPos(playerid, 1958.3783f, 1343.1572f, 15.3746f);
  SetPlayerCameraPos(playerid, 1958.3783f, 1343.1572f, 15.3746f);
  SetPlayerCameraLookAt(playerid, 1958.3783f, 1343.1572f, 15.3746f, CAMERA_CUT);
  return true;
}

PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerCommandText(int playerid,
                                                   const char *cmdtext) {
  if (strcmp(cmdtext, "/hello") == 0) {
    char name[MAX_PLAYER_NAME];
    GetPlayerName(playerid, name, sizeof(name));
    char message[MAX_CLIENT_MESSAGE];
    //sprintf(message, "Hello, %s!", name);
    SendClientMessage(playerid, 0x00FF00FF, message);
    return true;
  }

  if (strcmp(cmdtext, "/create") == 0)
  {
	  Area area;
	  area.xMin = float(4720.3354);
	  area.xMax = float(4766.1201);
	  area.yMin = float(134.1717);
	  area.yMax = float(231.4489);
	  area.zMin = float(11.2423);
	  area.zMax = float(22.2423);

	  AttackBot bot(18846, area);

	  attackbots.push_back(bot);
	
	  string botidList = "Bot List: ";
	  for (size_t i = 0; i < attackbots.size(); i++)
	  {
		  botidList += to_string(attackbots[i].getBotID()) + "->hp:" + to_string(attackbots[i].getHealth()) + " | ";
	  }
	  SendClientMessage(playerid, 0x44AA77FF, botidList.c_str());

	  return true;
  }

  if (strcmp(cmdtext, "/list") == 0)
  {
	  string botidList = "Bot List: ";
	  for (size_t i = 0; i < attackbots.size(); i++)
	  {
		  botidList += to_string(attackbots[i].getBotID()) + "->hp:" + to_string(attackbots[i].getHealth()) + " | ";
	  }
	  SendClientMessage(playerid, 0x44AA77FF, botidList.c_str());
  }

  if (strcmp(cmdtext, "/fffast") == 0)
  {
	  for (size_t i = 0; i < attackbots.size(); i++)
	  {
		  attackbots[i].setSpeed(105);
	  }
	  return true;
  }

  if(strcmp(cmdtext, "/fast") == 0)
  {
	  for (size_t i = 0; i < attackbots.size(); i++)
	  {
		  attackbots[i].setSpeed(25);
	  }
	  return true;
  }


  if (strcmp(cmdtext, "/medium") == 0)
  {
	  for (size_t i = 0; i < attackbots.size(); i++)
	  {
		  attackbots[i].setSpeed(10);
	  }
	  return true;
  }


  if (strcmp(cmdtext, "/slow") == 0)
  {
	  for (size_t i = 0; i < attackbots.size(); i++)
	  {
		  attackbots[i].setSpeed(5);
	  }
	  return true;
  }

  return false;
}

PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerDeath(int playerid, int killerid, int reason) {
	

	return true;
}

PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerWeaponShot(int playerid, int weaponid, int hittype, int hitid, float fX, float fY, float fZ)
{
	if (hittype == BULLET_HIT_TYPE_OBJECT)
	{
		int botid = hitid;

		for (size_t i = 0; i < attackbots.size(); i++)
		{
			if (attackbots[i].getBotID() == botid)
			{
				attackbots[i].damage(playerid, processDamage(weaponid), attackbots);
			}
		}
	}

	return true;
}

PLUGIN_EXPORT bool PLUGIN_CALL OnObjectMoved(int objectid)
{
	//sampgdk::logprintf("OnObjecTMoved");
	onBotMoved(objectid);
	return true;
}



PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerUpdate(int playerid)
{
	
	return true;
}

void onBotsUpdate()
{
	for (size_t i = 0; i < attackbots.size(); i++)
	{
		attackbots[i].OnUpdate();
	}
}

void onBotMoved(int botid)
{
	for (size_t i = 0; i < attackbots.size(); i++)
	{
		if (botid == attackbots[i].getBotID())
		{
			attackbots[i].OnMoved();
			break;
		}
			
	}
}

PLUGIN_EXPORT unsigned int PLUGIN_CALL Supports() {
  return sampgdk::Supports() | SUPPORTS_PROCESS_TICK;
}

PLUGIN_EXPORT bool PLUGIN_CALL Load(void **ppData) {
  return sampgdk::Load(ppData);
}

PLUGIN_EXPORT void PLUGIN_CALL Unload() {
  sampgdk::Unload();
}

PLUGIN_EXPORT void PLUGIN_CALL ProcessTick() {
  sampgdk::ProcessTick();
}

static cell AMX_NATIVE_CALL OnSomethingHappens(AMX* amx, cell* params)
{
	SendClientMessageToAll(-1, "First native!");
	return 1;
}

//An array of the functions we wish to register with the abstract machine.
AMX_NATIVE_INFO PluginNatives[] =
{
	//Here we specify our native functions information and terminate the array with two null values.
	{ "OnSomethingHappens", OnSomethingHappens},
	{ 0, 0 }
};

PLUGIN_EXPORT int PLUGIN_CALL AmxLoad(AMX *amx)
{
	//Here we register our natives to the abstract machine. Note how we're using -1. Normally this would have to be the number of
	//functions we're registering, but since we terminated the array with two null values, we can specify -1.  
	return amx_Register(amx, PluginNatives, -1);
}

float processDamage(int weaponid)
{
	switch (weaponid)
	{
	case WEAPON_DEAGLE:
		return float(45.0);
		break;

	case WEAPON_SAWEDOFF:
		return float(27.0);
		break;

	case WEAPON_UZI:
		return float(5.0);
		break;

	case WEAPON_M4:
		return float(10.0);
		break;

	case WEAPON_SNIPER:
		return float(60.0);
		break;

	default:
		return float(15.0);
		break;
	}
}