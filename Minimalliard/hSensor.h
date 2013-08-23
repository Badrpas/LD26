#include <hge.h>
#include <hgesprite.h>
#include <Box2D.h>
#include <cmath>

#pragma warning(disable : 4006)

enum hSensorType{
	hSensorFinish,
	hSensorGetPunch,
	hSensorPunch
};

class hSensor{
public:
	float angle;
	float time;
	float punchPower;
	float fadeout;
	b2Body*   body;
	hSensorType type;
	hSensor(
		float x_,
		float y_,
		float radius_,
		hSensorType type_ = hSensorFinish,
		float angle_ = 0);
	DWORD color;
	bool update();
	void render();
	void onContact();
	void onContactEnd();
protected:
	float x, y, radius;
};

