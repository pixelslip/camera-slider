#ifndef Menu_h
#define Menu_h

typedef struct 
{
	char* name;
	char** items;
	uint8_t nbItems;
	int current;
	void (*callbackFct)(uint8_t select);
} Menu;

typedef enum 
{
	SELECT,
	ENTER,
	ESC,
	NONE
} Button;

#endif

/*
Base on code idea:
https://skyduino.wordpress.com/2014/07/06/arduino-lcd-faire-un-menu-sous-forme-de-liste/
 */