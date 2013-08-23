#include <hge.h> // editor
#include <hgefont.h>
#include <hgevector.h>
#include <hgesprite.h>
#include <stdio.h>
#include <math.h>
#include <vector>
#include <Box2D.h>
#include <vector>
#include <typeinfo>
#include <lua.hpp>


#include "hBlock.h"
#include "hSensor.h"
#include "hButton.h"

lua_State *l;

int lNewBlock  (lua_State  *l);
int lNewSensor (lua_State  *l);
int lSetBounds (lua_State  *l);
int lSetBallPos(lua_State  *l);
int lPunchCount(lua_State  *l);

HGE * hge;
float dt;
float mouse_x = 400, mouse_y = 300;
float mouse_x_pre = 400, mouse_y_pre = 300;
float mouse_x_shear = 0, mouse_y_shear = 0;


float startArrowAngle = 0;

hButton *buttons[BUTTONS_COUNT];

HTEXTURE	 ballTexture;
HTEXTURE	 slowTexture;
HTEXTURE	blockTexture;
HTEXTURE	arrowTexture;
HTEXTURE	sensorTexture;
hgeSprite	 *ballSprite;
hgeSprite	 *slowSprite;
hgeSprite	*blockSprite;
hgeSprite	*arrowSprite;
hgeSprite	*sensorSprite;
hgeFont		*font;

	
bool inputEnabled  = true;
bool updateEnabled = true;
bool gameEnabled   = true;
	
void saveLevel();

bool	editing = true;
bool	debug   = true;

int		currentLevel = 0;



bool	canPunchBall = true;
int		punchCount   = 5;
const	float TO_NEXT_PUNCH = 1.1f;
float	toNextPunch  = TO_NEXT_PUNCH;




bool isOverBall = false;



const	float PUNCH_POWER = 0.12f;

b2Vec2	gravity(0.0f, 0.0f);
b2World * world = new b2World(gravity);

b2BodyDef bodyDef;
b2Body*   ball;
b2PolygonShape	dynamicBox;
b2CircleShape	circleShape;
b2FixtureDef	fixtureDef;
	
std::vector <hBlock*>  blocks;
std::vector <hSensor*> sensors;

float	veilFade = 0xaa;
float	veilFadeMerge = 0;
int		veilFadeMergeMod = 1;

float getAngle(float X1, float Y1, float X2, float Y2);
float length(hgeVector v1, hgeVector v2);

bool render();
bool update();


void setBounds();
void setupLevel(int levelNumber);
void clearLevel();
void restartLevel();
void setBallPos(float x_, float y_);
bool goToNext = false;
float getBallX();
float getBallY();

void bye();

hSensor* findSensor(b2Body* body_){
	for (unsigned int i = 0; i < sensors.size();i++){
		if (body_ == sensors[i]->body) return sensors[i];
	}
	return NULL;
}



class MyContactListener : public b2ContactListener{
public:
     void BeginContact(b2Contact* contact) {
		b2Body* _body = contact->GetFixtureA()->GetBody();
		hSensor *sensor; 
		sensor = findSensor(_body);
		if (sensor){
			sensor->onContact();
		}else{
			_body = contact->GetFixtureB()->GetBody();
			sensor = findSensor(_body);
			if (sensor)
				sensor->onContact();
		}
    }
     void EndContact(b2Contact* contact) {
		b2Body* _body = contact->GetFixtureA()->GetBody();
		hSensor *sensor; 
		sensor = findSensor(_body);
		if (sensor)
			sensor->onContactEnd();
    }
} myContactListenerInstance;

