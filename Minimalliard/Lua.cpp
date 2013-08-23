#include "references.h"

int lNewBlock  (lua_State  *l){
	int paramCount = lua_gettop(l);
	if (paramCount < 4) return 0;
	if (paramCount == 4)
		new hBlock(
			lua_tonumber(l,1),
			lua_tonumber(l,2),
			lua_tonumber(l,3),
			lua_tonumber(l,4)); 
	else
		new hBlock(
			lua_tonumber(l,1),
			lua_tonumber(l,2),
			lua_tonumber(l,3),
			lua_tonumber(l,4),
			lua_tonumber(l,5));
	return 0; 
}

int lNewSensor (lua_State  *l){
	int paramCount = lua_gettop(l);
	if (paramCount < 4) return 0;
	if (paramCount == 4)
		new hSensor(
			lua_tonumber(l,1),
			lua_tonumber(l,2),
			lua_tonumber(l,3),
			(hSensorType)(int)lua_tonumber(l,4));
	else
		new hSensor(
			lua_tonumber(l,1),
			lua_tonumber(l,2),
			lua_tonumber(l,3),
			(hSensorType)(int)lua_tonumber(l,4),
			lua_tonumber(l,5));
	return 0;
}

int lSetBounds (lua_State  *l){
	setBounds();
	return 0;
}

int lSetBallPos(lua_State  *l){
	int paramCount = lua_gettop(l);
	if (paramCount < 2) return 0;
	setBallPos(
			lua_tonumber(l,1),
			lua_tonumber(l,2));
	return 0;
}

int lPunchCount(lua_State  *l){
	int paramCount = lua_gettop(l);
	if (paramCount < 1) return 0;
	punchCount = (int)lua_tonumber(l,1);
	return 0;
}
