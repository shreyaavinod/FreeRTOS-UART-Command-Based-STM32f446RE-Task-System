
#include "main.h"
#include "taskhandler.h"
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"
#include "queue.h"

state_t current_state=smainmenu;
BaseType_t ret;




extern QueueHandle_t inputqueue;
extern QueueHandle_t printqueue;
extern TaskHandle_t menutask;
extern TaskHandle_t rtctask;
extern TaskHandle_t printtask;
extern TaskHandle_t ledtask;
extern TaskHandle_t cmdhandlertask;

//TASK FUNCTION IMPLEMNETATIONS

void menu_task(void * parameters)
{
	int option;
	uint32_t cmd_addr;
	BaseType_t status;
	cmd_format * cmd_ptr;
	const char * message= "|                                                    |\r\n"
						  "=====================MAIN MENU=======================\r\n"
			              "|                                                    |\r\n"
						  "Enter your choice to perform the following operations:\r\n"
						  "1. LED MANIPULATION: Press 0 \r\n"
						  "2. RTC CONFIGURATION:Press 1\r\n"
						  "3. EXIT:Press 2\r\n "
						  "Enter your choice:";
	const char *invalidmessage="INVALID COMMAND\r\n";
	while (1)
	{
		xQueueSend(printqueue,&message,portMAX_DELAY);

		//wait for notification:
		status = xTaskNotifyWait(0,0,&cmd_addr,portMAX_DELAY);
		cmd_ptr=(cmd_format *)cmd_addr;

		//extract payload :
		if(cmd_ptr->len==1)
		{
		option=cmd_ptr->payload[0]-48;
		}
		else {
			//invalid
			xQueueSend(printqueue,&invalidmessage,portMAX_DELAY);
		}

		switch(option)
		{
		case 0:
			current_state=sLEDMenu;
			xTaskNotify(ledtask,0,eNoAction);
			break;
		case 1:
			current_state=RTCMenu;
			xTaskNotify(rtctask,0,eNoAction);
			break;
		case 2:
			break;
		default:
			xQueueSend(printqueue,&invalidmessage,portMAX_DELAY);
			continue;


		}
		xTaskNotifyWait(0,0,NULL,portMAX_DELAY);
	}

}

void rtc_task(void * parameters)
{
	while (1)
	{

	}
}

void led_task(void * parameters)
{
	int option;
		uint32_t cmd_addr;
		BaseType_t status;
		cmd_format * cmd_ptr;
		const char * message= "|                                                    |\r\n"
							  "=====================LED TASK=======================\r\n"
				              "|                                                    |\r\n"
							  "Enter your choice to witness different LED effects:\r\n"
							  "1. Effect 1:  Press 0 \r\n"
							  "2. Effect 2:  Press 1\r\n"
							  "3. Effect 3:  Press 2\r\n "
				              "4. Effect 4:  Press 3\r\n "
				              "5. No effect: Press 4\r\n "
							  "Enter your choice:";
		const char *invalidmessage="INVALID COMMAND\r\n";
		while (1)
		{
			xQueueSend(printqueue,&message,portMAX_DELAY);

			//wait for notification:
			status = xTaskNotifyWait(0,0,&cmd_addr,portMAX_DELAY);
			cmd_ptr=(cmd_format *)cmd_addr;

			//extract payload :
			if(cmd_ptr->len==1)
			{
			option=cmd_ptr->payload[0]-48;
			if(option==0)
			{
				ledeffect(0);
			}else if(option==1)
			{
				ledeffect(1);
			}else if (option==2)
			{
				ledeffect(2);
			}else if (option==3)
			{
				ledeffect(3);
			}else if (option==4)
			{
				ledeffect(4);
			}
			else {
				//invalid command
				xQueueSend(printqueue,&invalidmessage,portMAX_DELAY);

			}
			}
			else {
				//invalid
				xQueueSend(printqueue,&invalidmessage,portMAX_DELAY);
				current_state=smainmenu;
			}

			xTaskNotifyWait(0,0,NULL,portMAX_DELAY);

			}

	}


void print_task(void * parameters)
{
	while (1)
	{

	}
}

void cmd_handler_task(void * parameters)
{
	cmd_format cmd;
	while (1)
	{
		//wait till notification on receiving the entire byte arrives;
		//only when a null character is sent by the user the program reaches here
		ret=xTaskNotifyWait(0,0,NULL,portMAX_DELAY);
		//decoding the command received
		if (ret==pdTRUE)// when the notification is received pdTTUE is returned
		{
			process_cmd(&cmd);
		}



	}
}

void process_cmd(cmd_format *command){


	extract_cmd(command);

	//decide which task to notify using the current state;
	switch(current_state)
	{
	case smainmenu:
		xTaskNotify(&menutask,(uint32_t)command,eSetValueWithOverwrite);
		break;
	case sLEDMenu:
		xTaskNotify(&ledtask,(uint32_t)command,eSetValueWithOverwrite);
		break;

	case RTCMenu:
	case RTCTimeConfig:
	case RTCDateConfig:
	case RTCReport:
		xTaskNotify(&rtctask,(uint32_t)command,eSetValueWithOverwrite);
		break;
	}

}


int8_t extract_cmd (cmd_format* command){
	BaseType_t status;
	uint8_t i=0;
	uint8_t queued_item;

	UBaseType_t count=0;
	count= uxQueueMessagesWaiting(inputqueue);
	if(count==0)
		return -1;

	do{
		status =xQueueReceive(inputqueue, &queued_item, 0);
		if(status==HAL_OK)
		{
			command->payload[i++]=queued_item;
		}

	}while (queued_item!='\n');

	command->payload[i-1]='\0';
	command->len=i-1;

	return 0;
	//simpley dequeue and put the data in the array in the command struct


}






















