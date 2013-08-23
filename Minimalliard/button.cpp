#include "button.h"
#include <hge.h>
#include <hgefont.h>
#include <hgecolor.h>
#include <hgesprite.h>

extern hgeFont *font;
extern HGE * hge;


void RectangleButton::Render(){
	DWORD c = emptySprite->GetColor();
	emptySprite->SetColor(color);
	emptySprite->RenderEx(x + width/2, y + height/2, 0, width/2, height / 2);
	emptySprite->SetColor(c);
	c = font->GetColor();
	font->SetColor(0x30000000);
	font->printf(x + width/2 -captionDx/4, y + (height - font->GetHeight())/2 -captionDy/4, HGETEXT_CENTER, caption);
	font->SetColor(colorText);
	font->printf(x + width/2 +captionDx  , y + (height - font->GetHeight())/2 +captionDy  , HGETEXT_CENTER, caption);
	font->SetColor(c);
}

void RectangleButton::Update(){
	isPressed = wasPressed = false;
	if (mouseIn()){
		select();
		if (mouseX_pre != mouseX && mouseY_pre != mouseY && onMouseIn)
			onMouseIn();
		if (hge->Input_KeyUp(HGEK_LBUTTON)){
			if(onMouseClick)
			onMouseClick();
			wasPressed = true;
		}
		if (hge->Input_GetKeyState(HGEK_LBUTTON)){
			float mod = 0.5;
			captionDx *= mod;
			captionDy *= mod;
			isPressed = true;
		}
	}else{
		deselect();
		if (in(mouseX_pre, mouseY_pre) && onMouseOut)
			onMouseOut();
	}
}

DWORD RectangleButton::GetColor(){
	return color;
}

void RectangleButton::SetCaption(char *caption_){
	caption = caption_;
	width = font->GetStringWidth(caption) + font->GetStringWidth("   ");;
	height= font->GetHeight()*1.1;
}

void RectangleButton::SetFunctions(void (*OnMouseClick)(), void (*OnMouseIn)(), void (*OnMouseOut)()){
	if (OnMouseClick)
		onMouseClick = OnMouseClick;
	if (OnMouseIn)
		onMouseIn = OnMouseIn;
	if (OnMouseOut)
		onMouseOut = OnMouseOut;
}

void RectangleButton::select(){
	color = colorActive;
	colorText = colorTextActive;
	captionDx = -font->GetStringWidth(" ") / 2;
	captionDy = -font->GetHeight()/6;
}
void RectangleButton::deselect(){
	color = colorCommon;
	colorText = colorTextCommon;
	captionDx = -font->GetStringWidth(" ") / 2 * 0.5;
	captionDy = -font->GetHeight()/6 * 0.5;
}

void RectangleButton::init(){
		onMouseClick = NULL;
		onMouseIn	 = NULL;
		onMouseOut	 = NULL;
		captionDx = 0;
		captionDy = 0;
		
		colorCommon     = 0xFFFFFF00;
		colorActive     = 0xFF00FF00;
		
		colorTextCommon = 0xFF0000FF;
		colorTextActive = 0xFF0020FF;
		caption = " - ";
		isPressed = wasPressed = false;
}
