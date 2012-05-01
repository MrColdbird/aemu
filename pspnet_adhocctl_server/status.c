#include <stdio.h>
#include <string.h>
#include <user.h>
#include <status.h>
#include <config.h>

/**
 * Update Status Logfile
 */
void update_status(void)
{
	// Open Logfile
	FILE * log = fopen(SERVER_STATUS_LOGFILE, "w");
	
	// Opened Logfile
	if(log != NULL)
	{
		// Write Total User & Game Count (including idle users)
		fprintf(log, "%u\n", _db_user_count);
		
		// Iterate Games
		SceNetAdhocctlGameNode * game = _db_game; for(; game != NULL; game = game->next)
		{
			// Active Game User Count
			uint32_t game_user_count = 0;
			
			// Iterate Game Groups
			SceNetAdhocctlGroupNode * group = game->group; for(; group != NULL; group = group->next)
			{
				// Increase Active User Count
				game_user_count += group->playercount;
			}
			
			// Product Code Termination
			char product_code[10];
			strncpy(product_code, game->game.data, PRODUCT_CODE_LENGTH);
			product_code[9] = 0;
			
			// Output Game Product ID, Idle Player Count and Active Player Count
			fprintf(log, "%s:%u:%u\n", product_code, game->playercount - game_user_count, game_user_count);
		}
		
		// Close Logfile
		fclose(log);
	}
}

