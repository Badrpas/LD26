#include "references.h"
#pragma warning(disable : 4006)
void getInputDirectional();
void updateSensors();
void updateEdit();

bool update(){

	dt = hge->Timer_GetDelta();	
	hge->Input_GetMousePos(&mouse_x, &mouse_y);
	mouse_x_shear = mouse_x - mouse_x_pre;
	mouse_y_shear = mouse_y - mouse_y_pre;
	updateEdit();
	mouse_x_pre = mouse_x;
	mouse_y_pre = mouse_y;
	for (int i = 0; i < BUTTONS_COUNT; i++){
		buttons[i]->update();
	}

	if (hge->Input_KeyDown(HGEK_TAB)){
		editing = !editing;
	}

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
			if ( hge->Input_KeyDown(HGEK_LBUTTON) && canPunchBall && !editing){
				ball->SetLinearVelocity(b2Vec2(0,0));
				ball->ApplyLinearImpulseCenter(b2Vec2(PUNCH_POWER*sin(startArrowAngle), PUNCH_POWER*cos(startArrowAngle)));
				canPunchBall = false;
				toNextPunch  = TO_NEXT_PUNCH;
				punchCount--;
			}
		} else 
			startArrowAngle = getAngle(getBallX(), getBallY(), 400, 300);

		if (!editing) world->Step(dt, 6, 2);
		updateSensors();
	}

	if (hge->Input_KeyDown(HGEK_L)){
		printf("trying to search.\n");
		for (int i = 0; i < 800; i++)
			for (int j = 0; j < 600; j++){
				sensors[0]->body->GetFixtureList()->SetSensor(false);
				if (sensors[0]->body->GetFixtureList()->TestPoint(b2Vec2(i/100.0f, 6.0f - j/100.0f))){
					printf("(%i; %i)\n", i, 600-j);
				}
				sensors[0]->body->GetFixtureList()->SetSensor(true);
			}
	}


	return false;
}

void updateSensors(){
	for(unsigned int i = 0; i < sensors.size(); i++){
		sensors[i]->update();
	}
}

void updateEdit(){ if (editing){
	b2Vec2 m_pos = b2Vec2(mouse_x     / 100.0f, 6.0f - mouse_y     / 100.0f);
	b2Vec2 m_pre = b2Vec2(mouse_x_pre / 100.0f, 6.0f - mouse_y_pre / 100.0f);
	
	if (hge->Input_GetKeyState(HGEK_LBUTTON)){
		if (ball->GetFixtureList()->TestPoint(m_pre))
			ball->SetTransform(m_pos,0);
		for (int i = 0; i < blocks.size(); i++)
			if (blocks[i]->body->GetFixtureList()->TestPoint(m_pre)){
				b2Vec2 sh;
				sh.x = m_pos.x + (-m_pre.x + blocks[i]->body->GetPosition().x);
				sh.y = m_pos.y + (-m_pre.y + blocks[i]->body->GetPosition().y);
				blocks[i]->body->SetTransform( sh, blocks[i]->body->GetAngle() );
			}			
		for (int i = 0; i < sensors.size(); i++){
			sensors[i]->body->GetFixtureList()->SetSensor(false);
			if (sensors[i]->body->GetFixtureList()->TestPoint(m_pre)){
				b2Vec2 sh;
				sh.x = m_pos.x + (-m_pre.x + sensors[i]->body->GetPosition().x);
				sh.y = m_pos.y + (-m_pre.y + sensors[i]->body->GetPosition().y);
				sensors[i]->body->SetTransform( sh, sensors[i]->body->GetAngle() );
			}
			sensors[i]->body->GetFixtureList()->SetSensor(true);
		}
	}

	if (hge->Input_GetMouseWheel() != 0){
		for (int i = 0; i < blocks.size(); i++)
			if (blocks[i]->body->GetFixtureList()->TestPoint(m_pos)){
				blocks[i]->body->SetTransform(
					blocks[i]->body->GetPosition(),
					blocks[i]->body->GetAngle()+hge->Input_GetMouseWheel()*b2_pi/180.0f);
			}			
		for (int i = 0; i < sensors.size(); i++){
			sensors[i]->body->GetFixtureList()->SetSensor(false);
			if (sensors[i]->body->GetFixtureList()->TestPoint(m_pos)){
				sensors[i]->body->SetTransform(
					sensors[i]->body->GetPosition(),
					sensors[i]->body->GetAngle()+hge->Input_GetMouseWheel()*b2_pi/180.0f);
				sensors[i]->angle += hge->Input_GetMouseWheel()*b2_pi/180.0f;
			}
			sensors[i]->body->GetFixtureList()->SetSensor(true);
		}
	}

}}