#ifndef _HUD_H_
#define _HUD_H_

#include "draw.h"

// Paint HUD Overlay
void drawInfo(CANVAS * canvas);

// Paint Notification Overlay
void drawNotification(CANVAS * canvas);

// Register Keyphrase
int registerKeyPhrase(char * phrase);

// Add Chat Log
void addChatLog(char * nickname, char * text);

// Add Message to Outbox
void addToOutbox(char * text);

// Pop Message from Outbox
int popFromOutbox(char * buffer);

// Set Connection Status
void setConnectionStatus(int status);

// Set User Count
void setUserCount(int count);

// Handle Key Event
void handleKeyEvent(uint32_t prev, uint32_t curr);

#endif
