#include "references.h"
#pragma warning(disable : 4006)


void initialize(){
	hge = hgeCreate(HGE_VERSION);
	hge->System_SetState(HGE_LOGFILE,		 "this_is_editor_.log");
	hge->System_SetState(HGE_FRAMEFUNC,		 update				);
	hge->System_SetState(HGE_RENDERFUNC,		 render 			);
	hge->System_SetState(HGE_TITLE,			 "Map Editor"		);
	hge->System_SetState(HGE_FPS,			 HGEFPS_VSYNC		);
	hge->System_SetState(HGE_HIDEMOUSE,		 false				);
	hge->System_SetState(HGE_WINDOWED,		 true				);
	hge->System_SetState(HGE_SCREENWIDTH,	 800				);
	hge->System_SetState(HGE_SCREENHEIGHT,	 600				);
	hge->System_SetState(HGE_SCREENBPP,		 32					);
	hge->System_SetState(HGE_SHOWSPLASH,		 false				);
	hge->System_SetState(HGE_USESOUND,		 false				);
}


void setBounds(){
	new hBlock( 4.0f,-0.1f, 4.0f, 0.1f);
	new hBlock( 4.0f, 6.1f, 4.0f, 0.1f);
	new hBlock(-0.1f, 3.0f, 0.1f, 3.0f);
	new hBlock( 8.1f, 3.0f, 0.1f, 3.0f);
}

void createBall(){
	bodyDef.type = b2_dynamicBody;		// Объект, ска, динамичен!
	bodyDef.position.Set(4.0f, 0.1f);
	bodyDef.linearDamping = 0.45f;		// Торможение
	bodyDef.angularDamping= b2_maxFloat;
	ball = world->CreateBody(&bodyDef);
	ball->SetBullet(true);

	circleShape.m_p.Set(0, 0);
	circleShape.m_radius = 0.1f ;		//radius
	fixtureDef.shape = &circleShape;
	fixtureDef.density  = 1.0f;
	fixtureDef.friction = 0.0f;
	fixtureDef.restitution = 1.0f;
	ball->CreateFixture(&fixtureDef);
}

void initializeBox2d(){
	createBall();
	clearLevel();
	setupLevel(currentLevel);
}



void onStart(){
	ballTexture = hge->Texture_Load("images/ball.png");
	ballSprite  = new hgeSprite(ballTexture,0,0,32,32);
	ballSprite -> SetHotSpot(16,16);
	slowTexture = hge->Texture_Load("images/slow.png");
	slowSprite  = new hgeSprite(slowTexture,0,0,256,256);
	slowSprite -> SetHotSpot(128,128);
	blockTexture = hge->Texture_Load("images/1px.png");
	blockSprite  = new hgeSprite(blockTexture,0,0,2,2);
	blockSprite -> SetHotSpot(1,1);
	arrowTexture = hge->Texture_Load("images/arrow.png");
	arrowSprite  = new hgeSprite(arrowTexture,0,0,32,64);
	arrowSprite -> SetHotSpot(16,64+32);
	sensorTexture = hge->Texture_Load("images/sensor.png");
	sensorSprite  = new hgeSprite(sensorTexture,0,0,64,64);
	sensorSprite -> SetHotSpot(32,32);

	font = new hgeFont("font1.fnt");
	
	buttons[0] = new hButton( 50, "go", idStart  );
	buttons[1] = new hButton(650,"bye", idExit   );
	buttons[BUTTONS_COUNT-1] = new hButton(750, "re", idRestart);
	for (int i = 0; i < BUTTONS_COUNT-1; i++)
		buttons[i]->enabled = false;
}


void loadLevel(char * filename){
	lua_State *l = lua_open();
	luaL_openlibs(l);
	lua_register(l, "block", lNewBlock);
	lua_register(l, "sensor", lNewSensor);
	lua_register(l, "setBounds", lSetBounds);
	lua_register(l, "setBallPos", lSetBallPos);
	lua_register(l, "setPunchCount", lPunchCount);
	luaL_dostring(l,"FINISH = 0 PUNCH = 1 FORCE = 2");
	luaL_dostring(l,"PI = 3.14159265359");
	luaL_dofile(l,filename);
	lua_close(l);
}



int main(){
	initialize();
	initializeBox2d();
	if(hge->System_Initiate()){
		onStart();
		printf("Я родился!\n");
		hge->System_Log("Я родился!");
		hge->System_Start();
	}else return 7;
	
	bye();
	return 0x128;
}

void bye(){
	//lua_close(l); // This generating "Access violation" (?_?)
	delete world;
	hge->System_SetState(HGE_WINDOWED, true);
	hge->System_Shutdown();
	hge->Release();
}

float getBallX(){
	return ball->GetPosition().x*100;
}
float getBallY(){
	return 600 - ball->GetPosition().y*100;
}

float flength(float x1, float y1, float x2, float y2);

void setupLevel(int levelNumber){
	clearLevel();
	createBall();
    world->SetContactListener(&myContactListenerInstance);
	loadLevel("levels/s1/3.lua");
}

// ehm... this must be leaking, 
// but there is no time to explore :(
void clearLevel(){ 	
	delete world;
	sensors.resize(0);
	blocks .resize(0);
	world = new b2World(gravity);
}

void restartLevel(){
	clearLevel();
	setupLevel(currentLevel);
}
void setBallPos(float x_, float y_){
	ball->SetTransform(b2Vec2(x_,y_),0);
}


float flength(float x1, float y1, float x2, float y2){
	return length(hgeVector(x1,y1),hgeVector(x2,y2));
}

float length(hgeVector v1, hgeVector v2){
	return sqrt((v1.x-v2.x)*(v1.x-v2.x) + (v1.y-v2.y)*(v1.y-v2.y));
}
float getAngle(float X1, float Y1, float X2, float Y2){
	float X;
	float Y;
	X=X2-X1;
	Y=Y2-Y1;
	if(X==0){
		if(Y>=0) return M_PI;
		else return 0;
	}
	else{
		if(X>0)
			return atan(Y/X) + M_PI_2;	
		else
			if(Y>=0)
				return atan(Y/X) + M_PI + M_PI_2; 
			else
				return atan(Y/X) - M_PI_2; 
	}
}