#include <hge.h>
#include <hgesprite.h>
#include <Box2D.h>
#include <vector>

#pragma warning(disable : 4006)

class hBlock{
public:
	hBlock(
		float x_, 
		float y_, 
		float halfWidth_, 
		float halfHeight_, 
		float angle_ = 0.0f);
	void render();
	b2Body*   body;
	float halfWidth, halfHeight;
};

