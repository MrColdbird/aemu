#include <string.h>
#include <stdio.h>
#include <pspctrl.h>
#include "hud.h"

// Logical Constants
#define NICKNAME_TRUNCATE 8
#define KEYPHRASE_TRUNCATE 8
#define CHAT_TRUNCATE 63
#define CHAT_MAX_LINES 8
#define KEYPHRASE_COUNT (8*4)
#define OUTBOX_SIZE 5

// Graphical Constants
#define HORIZONTAL_PADDING 12
#define VERTICAL_PADDING 10
#define FONT_WIDTH 6
#define BUTTON_CLONE_SHIFT 279
#define BUTTON_MAPPINGS_ON_SCREEN 8
#define BOTTOM_LINE 256
#define EXIT_MESSAGE_X 257
#define MIDDLE_MESSAGE_Y 132
#define MIDDLE_MESSAGE_X 90
#define CLEAR_CHAT_X 359
#define INPUT_LINE_Y 140

// Graphical Macros
#define GET_MIDDLE_ALIGN_X(t) (240 - strlen(t) * FONT_WIDTH / 2)

// Position for Button Mapping (8-Character optimized)
// UP, LEFT, DOWN, RIGHT, TRIANGLE, SQUARE, CROSS, CIRCLE
int bpos_x[8] = {  76 + 25,  12 + 25,  76 + 25, 140 + 25 };
int bpos_y[8] = { 158, 195, 234, 195 };

// Chat Message Structure
typedef struct
{
	char sender[NICKNAME_TRUNCATE + 1];
	char message[CHAT_TRUNCATE + 1];
} ChatLog;

// Chat Messages
static ChatLog chatlog[CHAT_MAX_LINES];

// Outbox
static char outbox[OUTBOX_SIZE][CHAT_TRUNCATE + 1];

// Keyphrases
static char * keyphrases[KEYPHRASE_COUNT] = {
	"A", "B", "C", "D", "E", "F", "G", "H",
	"I", "J", "K", "L", "M", "N", "O", "P",
	"Q", "R", "S", "T", "U", "V", "W", "X",
	"Y", "Z", "!", "?", "CLEAR", "SPACE", "ENTER", "DELETE"
};

// Chat Line Buffer
static char chatline[CHAT_TRUNCATE + 1];

// Phrase Package Index (0 = Standard, 1 = L Button, 2 = R Button, 3 = L+R Button)
int phrasepackage = 0;

// Online Mode Switch
extern int onlinemode;

// Server Connectivity Status
int connected_to_prometheus = 0;

// User Count
int usercount = 1;

// Show Notification Flag
int shownotification = 0;

// Paint HUD Overlay
void drawInfo(CANVAS * canvas)
{
	// Online Mode
	if(onlinemode)
	{
		// Erase Notification Flag
		shownotification = 0;
		
		// Connected to Server
		if(connected_to_prometheus)
		{
			// Chat Headline
			drawSmallFont(canvas, "PROMETHEUS CHATROOM", HORIZONTAL_PADDING, VERTICAL_PADDING, RGB_8888(0xDE, 0xDE, 0xDE));
			
			// Chat Output
			int i = 0; for(; i < CHAT_MAX_LINES; i++)
			{
				// End of Chat Log
				if(chatlog[i].message[0] == 0) break;
				
				// Print 8-Character Truncated Nickname
				drawSmallFont(canvas, chatlog[i].sender, HORIZONTAL_PADDING, VERTICAL_PADDING + FONT_WIDTH * (i+1) + 5, RGB_8888(0xFF, 0xFF, 0xFF));
				
				// Print Delimiter
				drawSmallFont(canvas, ":", HORIZONTAL_PADDING + FONT_WIDTH * NICKNAME_TRUNCATE, VERTICAL_PADDING + FONT_WIDTH * (i+1) + 5, RGB_8888(0xFF, 0xFF, 0xFF));
				
				// Print 63-Character Text Message
				drawSmallFont(canvas, chatlog[i].message, HORIZONTAL_PADDING + FONT_WIDTH * NICKNAME_TRUNCATE + FONT_WIDTH, VERTICAL_PADDING + FONT_WIDTH * (i+1) + 5, RGB_8888(0xFF, 0xFF, 0xFF));
			}
			
			// Show Chat Line
			drawSmallFont(canvas, chatline, HORIZONTAL_PADDING, INPUT_LINE_Y, RGB_8888(0x00, 0xFF, 0x00));
			
			// Fix Keyphrase Index
			int phraseindex = BUTTON_MAPPINGS_ON_SCREEN * phrasepackage;
			
			// Button Mapping Example
			for(i = 0; i < BUTTON_MAPPINGS_ON_SCREEN; i++)
			{
				// End of Keyphrase List
				if(keyphrases[phraseindex + i][0] == 0) break;
				
				// Location Shift
				int shift = (i > 3) ? (BUTTON_CLONE_SHIFT) : (0);
				
				// Fix Location Index
				int index = (i > 3) ? (i - 4) : (i);
				
				// Truncated Keyphrase Buffer
				char truncphrase[KEYPHRASE_TRUNCATE + 1];
				memset(truncphrase, 0, sizeof(truncphrase));
				strncpy(truncphrase, keyphrases[phraseindex + i], KEYPHRASE_TRUNCATE + 1);
				if(truncphrase[KEYPHRASE_TRUNCATE] != 0)
				{
					truncphrase[KEYPHRASE_TRUNCATE - 1] = '+';
					truncphrase[KEYPHRASE_TRUNCATE] = 0;
				}
				
				// Print 8-Character Truncated Keyphrase
				drawSmallFont(canvas, truncphrase, bpos_x[index] + shift - strlen(truncphrase) * FONT_WIDTH / 2, bpos_y[index], RGB_8888(0xDE, 0xDE, 0xDE));
			}
			
			// User Counter Example
			char strusercount[32]; sprintf(strusercount, "GROUP USER COUNT: %d", usercount);
			drawSmallFont(canvas, strusercount, HORIZONTAL_PADDING, BOTTOM_LINE, RGB_8888(0x00, 0xFF, 0x00));
			
			// Chat Clear Button Binding
			drawSmallFont(canvas, "SELECT: CLEAR CHAT", CLEAR_CHAT_X, VERTICAL_PADDING, RGB_8888(0x00, 0xFF, 0x00));
		}
		
		// Currently Offline
		else
		{
			// Write Offline Headline
			drawSmallFont(canvas, "WAITING FOR SERVER CONNECTION...", HORIZONTAL_PADDING, VERTICAL_PADDING, RGB_8888(0xDE, 0xDE, 0xDE));
		}
	}
	
	// Offline Mode
	else
	{
		// Offline Mode Headline
		drawSmallFont(canvas, "YOU'RE IN OFFLINE MODE! CHATROOM HAS BEEN DISABLED!", MIDDLE_MESSAGE_X, MIDDLE_MESSAGE_Y, RGB_8888(0xDE, 0xDE, 0xDE));
	}
	
	// Draw Exit Example
	drawSmallFont(canvas, "HOME: CLOSE MENU   START: EXIT GAME", EXIT_MESSAGE_X, BOTTOM_LINE, RGB_8888(0x00, 0xFF, 0x00));
}

// Paint Notification Overlay
void drawNotification(CANVAS * canvas)
{
	// New Incoming Message
	if(shownotification)
	{
		// Notification Text
		char * text = "INCOMING MESSAGE";
		
		// Draw Text Centered
		drawSmallFont(canvas, text, GET_MIDDLE_ALIGN_X(text), BOTTOM_LINE, RGB_8888(0x00, 0xFF, 0x00));
	}
}

// Add Chat Log
void addChatLog(char * nickname, char * text)
{
	// Chat Log Reference
	ChatLog * storage = NULL;
	
	// Find free Chat Log Slot
	int i = 0; for(; i < CHAT_MAX_LINES; i++)
	{
		// Found free Chat Log Slot
		if(chatlog[i].message[0] == 0)
		{
			// Link Field
			storage = &chatlog[i];
			
			// Stop Search
			break;
		}
	}
	
	// Chat Log full
	if(storage == NULL)
	{
		// Move Chat Logs
		for(i = 0; i < CHAT_MAX_LINES - 1; i++)
		{
			// Move Chat Log
			chatlog[i] = chatlog[i+1];
		}
		
		// Clear Last Chat Log
		memset(&chatlog[i], 0, sizeof(chatlog[i]));
		
		// Link Field
		storage = &chatlog[i];
	}
	
	// Store Nickname
	strncpy(storage->sender, nickname, NICKNAME_TRUNCATE);
	
	// Store Message
	strncpy(storage->message, text, CHAT_TRUNCATE);
	
	// Fix Nickname
	for(i = 0; i < strlen(storage->sender); i++)
	{
		// Fix Character
		storage->sender[i] = convertChar((uint8_t)storage->sender[i]);
	}
	
	// Fix Message
	for(i = 0; i < strlen(storage->message); i++)
	{
		// Fix Character
		storage->message[i] = convertChar((uint8_t)storage->message[i]);
	}
	
	// Activate Notification
	if(strcmp(nickname, "ME") != 0) shownotification = 1;
}

// Add Message to Outbox
void addToOutbox(char * text)
{
	// Other Player online
	if(usercount > 1)
	{
		// Find free Outbox Slot
		int i = 0; for(; i < OUTBOX_SIZE; i++)
		{
			// Found free Outbox Slot
			if(outbox[i][0] == 0)
			{
				// Link Field
				strncpy(outbox[i], text, CHAT_TRUNCATE);
				
				// Stop Search
				break;
			}
		}
	}
}

// Pop Message from Outbox
int popFromOutbox(char * buffer)
{
	// Find filled Outbox Slot
	int i = 0; for(; i < OUTBOX_SIZE; i++)
	{
		// Found used Outbox Slot
		if(outbox[i][0] != 0)
		{
			// Copy String
			strcpy(buffer, outbox[i]);
			
			// Clear Slot
			memset(outbox[i], 0, sizeof(outbox[i]));
			
			// Return Success
			return 1;
		}
	}
	
	// Nothing found
	return 0;
}

// Set Connection Status
void setConnectionStatus(int status)
{
	// Save Status
	connected_to_prometheus = status;
	
	// Disconnected
	if(!connected_to_prometheus)
	{
		// Clear Outbox
		memset(outbox, 0, sizeof(outbox));
	}
}

// Set User Count
void setUserCount(int count)
{
	// Save User Count
	usercount = count;
}

// Handle Key Event
void handleKeyEvent(uint32_t prev, uint32_t curr)
{
	// Online Mode
	if(onlinemode)
	{
		// Clear Chat Log
		if((prev & PSP_CTRL_SELECT) == 0 && (curr & PSP_CTRL_SELECT) != 0)
		{
			// Clear Chat Log
			memset(&chatlog, 0, sizeof(chatlog));
		}
		
		// Connected to Server
		if(connected_to_prometheus)
		{
			// Keyphrase Page
			if((curr & PSP_CTRL_LTRIGGER) != 0 && (curr & PSP_CTRL_RTRIGGER) != 0) phrasepackage = 3;
			else if((curr & PSP_CTRL_RTRIGGER) != 0) phrasepackage = 2;
			else if((curr & PSP_CTRL_LTRIGGER) != 0) phrasepackage = 1;
			else phrasepackage = 0;
			
			// Key Phrase Index
			int keyphraseindex = -1;
			
			// Calculate Page-Relative Index
			// UP, LEFT, DOWN, RIGHT, TRIANGLE, SQUARE, CROSS, CIRCLE
			if((prev & PSP_CTRL_UP) == 0 && (curr & PSP_CTRL_UP) != 0) keyphraseindex = 0;
			else if((prev & PSP_CTRL_LEFT) == 0 && (curr & PSP_CTRL_LEFT) != 0) keyphraseindex = 1;
			else if((prev & PSP_CTRL_DOWN) == 0 && (curr & PSP_CTRL_DOWN) != 0) keyphraseindex = 2;
			else if((prev & PSP_CTRL_RIGHT) == 0 && (curr & PSP_CTRL_RIGHT) != 0) keyphraseindex = 3;
			else if((prev & PSP_CTRL_TRIANGLE) == 0 && (curr & PSP_CTRL_TRIANGLE) != 0) keyphraseindex = 4;
			else if((prev & PSP_CTRL_SQUARE) == 0 && (curr & PSP_CTRL_SQUARE) != 0) keyphraseindex = 5;
			else if((prev & PSP_CTRL_CROSS) == 0 && (curr & PSP_CTRL_CROSS) != 0) keyphraseindex = 6;
			else if((prev & PSP_CTRL_CIRCLE) == 0 && (curr & PSP_CTRL_CIRCLE) != 0) keyphraseindex = 7;
			
			// Valid Page-Relative Index
			if(keyphraseindex >= 0)
			{
				// Calculate Absolute Index
				keyphraseindex += BUTTON_MAPPINGS_ON_SCREEN * phrasepackage;
				
				// Add Letter to Text
				if(keyphraseindex < 28 && strlen(chatline) < CHAT_TRUNCATE) strcpy(chatline + strlen(chatline), keyphrases[keyphraseindex]);
				
				// Clear Input
				else if(keyphraseindex == 28 && strlen(chatline) > 0) chatline[0] = 0;
				
				// Put Space Symbol
				else if(keyphraseindex == 29 && strlen(chatline) < CHAT_TRUNCATE) strcpy(chatline + strlen(chatline), " ");
				
				// Delete Letter
				else if(keyphraseindex == 31 && strlen(chatline) > 0) chatline[strlen(chatline) - 1] = 0;
				
				// Send Text
				else if(keyphraseindex == 30 && strlen(chatline) > 0)
				{
					// Schedule Chat Message for Sending via Adhoc Control Emulator
					addToOutbox(chatline);
					
					// Add to own Chat Log
					addChatLog("ME", chatline);
					
					// Clear Chat Line
					chatline[0] = 0;
				}
			}
		}
	}
}
