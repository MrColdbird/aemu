/*
 * This file is part of PRO ONLINE.

 * PRO ONLINE is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * PRO ONLINE is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with PRO ONLINE. If not, see <http://www.gnu.org/licenses/ .
 */

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
