/*
 * taskhandler.h
 *
 *  Created on: Jul 17, 2025
 *      Author: shreyaa_vinod
 */

#ifndef INC_TASKHANDLER_H_
#define INC_TASKHANDLER_H_





void menu_task(void * parameters);
void rtc_task(void * parameters);
void led_task(void * parameters);
void print_task(void * parameters);
void cmd_handler_task(void * parameters);

void process_cmd(cmd_format *command);
int8_t extract_cmd (cmd_format* command);



#endif /* INC_TASKHANDLER_H_ */
