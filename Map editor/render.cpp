#include "references.h"
#pragma warning(disable : 4006)

void renderBlocks();
void renderSensors();
int		startArrowShear = 0;
float	startArrowShearTime = 0, startArrowShearD = 1;
float	startArrowTransparency = 255.0f;
void updateStartArrow();


bool render(){
	updateStartArrow();
	hge->Gfx_BeginScene();
		hge->Gfx_Clear(0xFF00FFFF);
		b2Vec2 pos = ball->GetPosition();
		ballSprite->RenderEx(pos.x*100.0f, 600-pos.y*100.0f, 0.0f, 1/1.6f, 1/1.6f);
		renderBlocks();
		renderSensors();
		

		arrowSprite->SetColor(ARGB((int)startArrowTransparency, 0xFF, 0xFF, 0xFF) );
		arrowSprite->RenderEx(getBallX(), getBallY(), startArrowAngle);
		
		
		DWORD fntCol = font->GetColor();
		if(debug){
			font->SetColor(0xFF0000FF);
			font->printf(0,font->GetHeight(),HGETEXT_LEFT, "%.2f; %.2f", mouse_x/100, 6.0f - mouse_y/100);
		}

		if (editing){
			font->SetColor(0xFFFF0000);
			font->printf(0, 0, HGETEXT_LEFT, "editing");
		}
		if (isOverBall)
			font->printf(0, font->GetHeight()*2, HGETEXT_LEFT, "OVERBALL!!!");
		
		font->SetColor(fntCol);
		
		for (int i = 0; i < punchCount; i++){
			float _x, _y;
			arrowSprite->GetHotSpot(&_x, &_y);
			arrowSprite->SetHotSpot( 16,  32);
			arrowSprite->RenderEx((18*i+10), 600 - 20+3.0f*cos(hge->Timer_GetTime()*4+i), M_PI/6+M_PI/24*cos(hge->Timer_GetTime()*3+i), 0.5f);
			arrowSprite->SetHotSpot( _x,  _y);
		}

		for (int i = 0; i < BUTTONS_COUNT; i++)
			buttons[i]->render();
	hge->Gfx_EndScene();
	return false;
}

void renderBlocks(){
	for(unsigned int i = 0; i < blocks.size(); i++){
		blocks[i]->render();
	}
}
void renderSensors(){
	for(unsigned int i = 0; i < sensors.size(); i++){
		sensors[i]->render();
	}
}


void updateStartArrow(){
	startArrowShearTime+=dt;
	if (startArrowShearTime >= 0.05f){
		startArrowShearTime = 0;
		startArrowShear+=startArrowShearD;
		if (startArrowShear == 8.0f | startArrowShear == 0.0f)
			startArrowShearD = -startArrowShearD;
		arrowSprite -> SetHotSpot(16,64+32+startArrowShear);
	}
	const float limit = 100.0f;
	const float modifier = 400.0f;
	if (!canPunchBall && startArrowTransparency > limit)
		startArrowTransparency -= dt*modifier;
	if ( canPunchBall && startArrowTransparency < 255  ){
		startArrowTransparency += dt*modifier;
		if (startArrowTransparency > 255.0f) startArrowTransparency = 255.0f;
	}
	if (punchCount <= 0 && startArrowTransparency <= limit)
		startArrowTransparency = 0;
}