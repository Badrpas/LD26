#include "hSensor.h"
#include <vector>
#pragma warning(disable : 4006)

extern float dt;
extern hgeSprite *sensorSprite;
extern b2World   *world;
extern std::vector <hSensor*> sensors;
extern hgeSprite	*arrowSprite;
extern b2Body		*ball;
extern int punchCount;
extern int currentLevel;
extern void setupLevel(int levelNumber);
extern void clearLevel();
extern void restartLevel();
extern void setBallPos(float x_, float y_);
extern bool goToNext;
extern float veilFadeMerge;



const float SENSOR_PUNCH_POWER = 0.24f;

void hSensor::onContact(){
	switch(type){
	case hSensorFinish:
		color = 0xFF0000FF;
		currentLevel++;
		goToNext = true;
		veilFadeMerge = 255.0f;
		break;
	case hSensorPunch:		
		ball->SetLinearVelocity(b2Vec2(0,0));
		ball->ApplyLinearImpulseCenter(
			b2Vec2(punchPower*sin(angle),
			punchPower*cos(angle)));
		break;
	case hSensorGetPunch:	
		punchCount++;
		fadeout -= dt;
		break;
	}
}
void hSensor::onContactEnd(){
	switch(type){
	case hSensorFinish:
		color = 0xFF00FF00;
		break;
	}
}

bool hSensor::update(){
	time += dt;
	switch(type){
	case hSensorPunch:
		time -= dt*2;
		break;
	case hSensorGetPunch:
		if (fadeout != 0xFF){
			fadeout -= dt*300;
			color = ARGB (fadeout, 0xFF, 0xFF, 00);
			if (fadeout <= 0)
				delete this;
		}
		break;
	}
	return false;
}

void hSensor::render(){
	float _angle = time;
	switch(type){
	case hSensorPunch:{
		float _x, _y;
		arrowSprite->GetHotSpot(&_x, &_y);
		arrowSprite->SetHotSpot( 16,  32);
		DWORD colPre = arrowSprite->GetColor();
		arrowSprite->SetColor(color);
		arrowSprite->RenderEx(x*100,600-y*100,angle,radius/3.6f*10.0f+0.2f*radius*(cos(time*6)-1.0f));
		arrowSprite->SetHotSpot( _x,  _y);
		arrowSprite->SetColor(colPre);
		_angle /= 3;
		}break;
	}
	sensorSprite->SetColor(color);
	sensorSprite->RenderEx(x*100,600-y*100,_angle,radius/3.2f*10.0f);
}

hSensor::hSensor(float x_, float y_, float radius_, hSensorType type_, float angle_){
	type = type_;
	x = x_;
	y = y_;
	time = 0;
	radius	= radius_;
	angle	= angle_;
	fadeout = 0xFF;
	punchPower = SENSOR_PUNCH_POWER;

	switch(type){
	case hSensorFinish:
		color = 0xFFFF0000;
		break;
	case hSensorGetPunch:
		color = 0xFFFFFF00;
		break;
	case hSensorPunch:
		color = 0xFFFFFFFF;
		break;
	}

	b2BodyDef		_bodyDef;
	b2PolygonShape	_bodyShape;
	b2FixtureDef	_bodyFixture;
	b2CircleShape	_circleShape;

	_bodyDef.angle  = angle;
	_bodyDef.position.Set(x_,y_);
	_bodyDef.userData = NULL;

	body = world->CreateBody(&_bodyDef);
	
	_circleShape.m_p.Set(0, 0);
	_circleShape.m_radius = radius_ ;

	_bodyFixture.shape = &_circleShape;
	_bodyFixture.density = 1.0f;
	_bodyFixture.friction= 0.0f;
	_bodyFixture.restitution = 1.0f;
	_bodyFixture.isSensor = true;
	body->CreateFixture(&_bodyFixture);
	
	sensors.push_back(this);
}