#include "main.h"
#include "stdint.h"
#include "LEDeffect.h"

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "timers.h"



extern TimerHandle_t timerhandle[4];

void ledeffectnone()
{
	//stop all timers
	for (int i =0;i<4;i++)
	{
		xTimerStop(timerhandle[i],portMAX_DELAY);
	}

}

void ledeffect(int effectnum)
{
	ledeffectnone();
	//start timers based on effectnum passed

	/* 0  - start timer 1 id 1
	 * 1 - timer 2 id 2
	 * 2 - timer 3 id 3
	 * 3 - timer 4 id 4
	 */
	xTimerStart(timerhandle[effectnum],portMAX_DELAY);




}

void ledeffect1(void ){

};
void ledeffect2(void ){

};
void ledeffect3(void ){

};
void ledeffect4(void ){

};

