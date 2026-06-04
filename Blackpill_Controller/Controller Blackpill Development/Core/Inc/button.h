#ifndef __BUTTON_H_
#define __BUTTON_H_

#include <stdint.h>

/**
 * Button driver object class. Enables button object with built-in DB.
 */

class Button{
public:
	enum State{
		INACTIVE,
		DBHOLD,
		ACTIVE
	};

	Button(char pin, bool holdable);
	void update(bool rawPressed);
	void tick();

	bool pressed() const;
	State state() const;

private:
	bool isHoldable; // can hold button down
	bool isPressed; // set HI on first contact, set LO on first break
	char bPin; // pointer to GPIO pin, can be reassigned

	State curState;

	uint32_t delayDB;
	static const uint32_t dbTimeMS = 20;
};
