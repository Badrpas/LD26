#include "hButton.h"
#include <hge.h>
#include <hgefont.h>
#include <hgesprite.h>
#pragma warning(disable : 4006)

extern hButton *buttons[BUTTONS_COUNT];

extern float mouse_x, mouse_y;
extern HGE * hge;
extern hgeFont *font;
extern hgeSprite *blockSprite;
extern void restartLevel();
extern bool gameEnabled;
extern bool inputEnabled;
extern bool updateEnabled;
extern void bye();
extern bool canPunchBall;

hButton::hButton(float x_, char *caption_, ID id_){
	x = x_;
	caption  = caption_;
	id = id_;
	selected = false;
	enabled  = true;
}

void hButton::update(){
	if (enabled){
		if (mouse_x > x - font->GetStringWidth(caption)
		 && mouse_x < x + font->GetStringWidth(caption)
		 && mouse_y < 20 + font->GetHeight()
		 && mouse_y > 0){
			 selected = true;
			 if (hge->Input_KeyUp(HGEK_LBUTTON))
				switch(id){
				case idStart:
					gameEnabled   = true;
					updateEnabled = true;
					inputEnabled  = true;
					break;
				case idRestart:
					restartLevel();
					canPunchBall = true;
					break;
				case idExit:
					bye();
					exit(4);
					break;
				default:
					break;
				}
		}else 
			selected = false;
	}
}

void hButton::render(){
	if (enabled){
		DWORD colPre = blockSprite->GetColor();
		blockSprite->SetColor(selected?0xFF0000FF:0xFFFFFFFF);
		blockSprite->RenderEx(x, 15, 0, font->GetStringWidth(caption)/2+font->GetStringWidth("r"), font->GetHeight());
		blockSprite->SetColor(colPre);
		colPre = font->GetColor();
		font->SetColor(!selected?0xFF00FFFF:0xFFFFFFFF);
		font->printf(x, 15, HGETEXT_CENTER, caption);
		font->SetColor(colPre);
	}
}
