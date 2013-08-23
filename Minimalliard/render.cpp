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
		
		DWORD fntCol = font->GetColor();
		font->SetColor(0xAAffFFff);
		switch (currentLevel){
		case 0:
			if (!buttons[0]->enabled)
			font->printf(400, 280, HGETEXT_CENTER, "click");
			break;
		case 1:
			font->printf(400,  10, HGETEXT_CENTER, "forces");
			break;
		case 2:
			font->printf(400, 550, HGETEXT_CENTER, "bounce");
			break;
		case 3:
			font->printf(665, 330, HGETEXT_CENTER, "additional");
			break;
		case 4:
			font->printf(400, 280, HGETEXT_CENTER, "hm...");
			break;
		case 5:
			font->printf(400, 280, HGETEXT_CENTER, "end");
			font->printf(400, 50, HGETEXT_CENTER, "sorry, no time for other lvls :(");
			font->printf(400, 500, HGETEXT_CENTER, "anyway, I'll do post-compo");
			break;
		}

		
		arrowSprite->SetColor(ARGB((int)startArrowTransparency, 0xFF, 0xFF, 0xFF) );
		arrowSprite->RenderEx(getBallX(), getBallY(), startArrowAngle);
		if(debug){
			font->SetColor(0xFF0000FF);
			font->printf(0,100,HGETEXT_LEFT, "%.2f; %.2f", mouse_x/100, 6.0f - mouse_y/100);
		}
		font->SetColor(fntCol);

		
		for (int i = 0; i < punchCount; i++){
			float _x, _y;
			arrowSprite->GetHotSpot(&_x, &_y);
			arrowSprite->SetHotSpot( 16,  32);
			arrowSprite->RenderEx((18*i+10), 600 - 20+3.0f*cos(hge->Timer_GetTime()*4+i), M_PI/6+M_PI/24*cos(hge->Timer_GetTime()*3+i), 0.5f);
			arrowSprite->SetHotSpot( _x,  _y);
		}

		if (veilFade > 0){
			DWORD colPre = blockSprite->GetColor();
			blockSprite->SetColor(ARGB(veilFade,0xFF,0xFF,0xFF));
			blockSprite->RenderEx(400, 300, 0, 400, 300);
			blockSprite->SetColor(colPre);
		}
		if (veilFadeMerge > 0){
			DWORD colPre = blockSprite->GetColor();
			blockSprite->SetColor(ARGB(veilFadeMerge,0xFF,0xFF,0xFF));
			blockSprite->RenderEx(400, 300, 0, 400, 300);
			blockSprite->SetColor(colPre);
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