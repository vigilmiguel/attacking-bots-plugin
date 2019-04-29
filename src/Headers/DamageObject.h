#ifndef DAMAGE_OBJECT
#define DAMAGE_OBJECT
#include <math.h>
#include <vector>
#include <string>
#include "tick.h"

const float PI = float(3.14159265);
const int INF_DURABILITY = -1;

class AttackBot;
struct Position;


class DamageObject
{

private:
	int objectid;
	bool isAttackBot = false;
	int duration; // How long this object will be around before being destroyed.
	float damage;
	int durability; // Times that it will damage players before it is destroyed.
	float range; // Any player that is within this range from the object will take damage.
	long long createTime;
public:
	DamageObject();
	DamageObject(AttackBot bot, float damage, int duration, int durability, float range);
	DamageObject(int modelid, Position spawn, Position end, float damage, int duration, int durability, float range);
	float calculateZRotation(Position start, Position end);
	float calculateXRotation(Position start, Position end);
	void onDamageObjectUpdate(std::vector<DamageObject>& objectList);
	void damagePlayer(int playerid, std::vector<DamageObject>& objectList);
	void destroy(std::vector<DamageObject>& objectList);


};



#endif
