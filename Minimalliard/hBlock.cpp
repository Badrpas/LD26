#include "hBlock.h"
#pragma warning(disable : 4006)

extern b2World *world;
extern hgeSprite *blockSprite;
extern std::vector <hBlock*> blocks;
extern hgeSprite	 *ballSprite;

hBlock::hBlock(
		float x_, 
		float y_, 
		float halfWidth_, 
		float halfHeight_, 
		float angle_)
{
	halfWidth  = halfWidth_;
	halfHeight = halfHeight_;

	b2BodyDef		_bodyDef;
	b2PolygonShape	_bodyShape;
	b2FixtureDef	_bodyFixture;

	_bodyDef.angle  = angle_;
	_bodyDef.position.Set(x_,y_);
	_bodyDef.userData = NULL;
	_bodyShape.SetAsBox(halfWidth_, halfHeight_);
	
	body = world->CreateBody(&_bodyDef);
	_bodyFixture.shape = &_bodyShape;
	_bodyFixture.density  = 1.0f;
	_bodyFixture.friction = 0.0f;
	_bodyFixture.restitution = 1.0f;
	body->CreateFixture(&_bodyFixture);
	blocks.push_back(this);
}


void hBlock::render(){
	blockSprite->RenderEx(
		body->GetPosition().x*100,
		600  - body->GetPosition().y*100,
		M_PI - body->GetAngle(),
		halfWidth*100, halfHeight*100
	);
}
