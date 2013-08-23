#include "references.h"
#pragma warning(disable : 4006)
void getInputDirectional();
void updateSensors();


bool update(){
	if ((!buttons[0]->enabled )&& veilFade > 0.0f)
		veilFade -= dt*200;
	if (veilFadeMerge > 0)
		veilFadeMerge += veilFadeMergeMod*dt*200;
	if (veilFadeMerge >= 255.0f)
		veilFadeMergeMod = -veilFadeMergeMod;
	if (veilFadeMerge <= 0){
		veilFadeMerge = 0;
		veilFadeMergeMod = -veilFadeMergeMod;
	}

	dt = hge->Timer_GetDelta();	
	hge->Input_GetMousePos(&mouse_x, &mouse_y);
	for (int i = 0; i < BUTTONS_COUNT; i++){
		buttons[i]->update();
	}

	if (hge->Input_KeyDown(HGEK_F))
		hge->System_SetState(HGE_WINDOWED,	!hge->System_GetState(HGE_WINDOWED));

	if (updateEnabled){
		b2Vec2 ballPos = ball->GetPosition();
		if (goToNext){
			restartLevel();
			goToNext = false;
		}

		if (toNextPunch > 0.0f){
			toNextPunch -= dt;
		}
		if (toNextPunch <= 0.0f && punchCount > 0)
			canPunchBall = true;
		if (inputEnabled){		
			startArrowAngle = getAngle(getBallX(), getBallY(), mouse_x, mouse_y);
			if (hge->Input_KeyDown(HGEK_ESCAPE)){
				//return true;
				inputEnabled = false;
				updateEnabled = false;
				veilFade = 0xaa;
				for (int i = 0; i < BUTTONS_COUNT-1; i++)
					buttons[i]->enabled = true;
			}
			if ((hge->Input_KeyDown(HGEK_SPACE) || hge->Input_KeyDown(HGEK_LBUTTON)) && canPunchBall){
				ball->SetLinearVelocity(b2Vec2(0,0));
				ball->ApplyLinearImpulseCenter(b2Vec2(PUNCH_POWER*sin(startArrowAngle), PUNCH_POWER*cos(startArrowAngle)));
				canPunchBall = false;
				toNextPunch  = TO_NEXT_PUNCH;
				punchCount--;
		}

		} else 
			startArrowAngle = getAngle(getBallX(), getBallY(), 400, 300);

		world->Step(dt, 6, 2);
		updateSensors();
	}

	return false;
}

void getInputDirectional(){
	if (hge->Input_GetKeyState(HGEK_W))
		ball->ApplyLinearImpulse(b2Vec2(0, PUNCH_POWER), b2Vec2(0,0));
	if (hge->Input_GetKeyState(HGEK_S))
		ball->ApplyLinearImpulse(b2Vec2(0,-PUNCH_POWER), b2Vec2(0,0));
	if (hge->Input_GetKeyState(HGEK_D))
		ball->ApplyLinearImpulse(b2Vec2( PUNCH_POWER,0), b2Vec2(0,0));
	if (hge->Input_GetKeyState(HGEK_A))
		ball->ApplyLinearImpulse(b2Vec2(-PUNCH_POWER,0), b2Vec2(0,0));
}


void updateSensors(){
	for(unsigned int i = 0; i < sensors.size(); i++){
		sensors[i]->update();
	}
}

