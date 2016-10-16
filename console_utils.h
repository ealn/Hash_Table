/*
 * Copyright (c) 2016 by Adrian Luna
 * All Rights Reserved
 *
 * Author: Adrian Luna
 *
 * Porpuse: Header that contains all utils for the console
 */

#include <stdint.h>
#include <stdbool.h>

int32_t getUInt32FromConsole(char *consoleStr);
int32_t getInt32FromConsole(char *consoleStr);
uint16_t getUInt16FromConsole(char *consoleStr);
int16_t getInt16FromConsole(char *consoleStr);
uint8_t getUint8FromConsole(char *consoleStr);
int8_t getInt8FromConsole(char *consoleStr);
void getStringFromConsole(char *consoleStr, char *outputStr, uint32_t maxSize);
char getFirstCharFromConsole(char *consoleStr);
bool validateIntInput(int32_t value, int32_t lowerLimit, int32_t upperLimit, bool needCleanScreen);
void cleanScreen(void);
bool repeatAction(char *str);

