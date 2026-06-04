#include "button.h"

Button::Button(char bPin, bool holdable)
	: bPin(pin),
	isHoldable(holdable),
	isPressed(false),
	curState(INACTIVE),
	delayDB(0)
{
}

void Button::tick(){
	if(delayDB>0){
		delayDB--;
	}
}

void Button::update(bool rawPressed){
	switch(curState){
	case INACTIVE:
		isPressed = false;
		if(rarPressed){
			delayDB = dbTimeMS;
			curState = DBHOLD;
		}
	}
	break;

	case DBHOLD:
		if(delayDB == 0){
			if (rawPressed){
				isPressed = true;
				curState = ACTIVE;
			} else {
				isPressed = false;
				curState = INACTIVE;
			}
		}
		break;

	case ACTIVE:
		if (rawPressed) {
			isPressed = true;
		} else {
			isPressed = false;
			curState = INACTIVE;
		}

		if (!isHoldable){
			curState = INACTIVE;
		}
		break;
}

bool Button::pressed() const {
	return isPressed;
}

Button::State Button::state() const {
	return curState();
}
