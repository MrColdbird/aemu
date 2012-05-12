#include <stdio.h>
#include <string.h>
#include <user.h>
#include <status.h>
#include <config.h>
#include <sqlite3.h>

/**
 * Update Status Logfile
 */
void update_status(void)
{
	// Open Logfile
	FILE * log = fopen(SERVER_STATUS_XMLOUT, "w");
	
	// Opened Logfile
	if(log != NULL)
	{
		// Write XML Header
		fprintf(log, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
		
		// Write XSL Processor Information
		fprintf(log, "<?xml-stylesheet type=\"text/xsl\" href=\"status.xsl\"?>\n");
		
		// Output Root Tag + User Count
		fprintf(log, "<prometheus usercount=\"%u\">\n", _db_user_count);
		
		// Database Handle
		sqlite3 * db = NULL;
		
		// Open Database
		if(sqlite3_open(SERVER_DATABASE, &db) == SQLITE_OK)
		{
			// Iterate Games
			SceNetAdhocctlGameNode * game = _db_game; for(; game != NULL; game = game->next)
			{
				// Safe Product ID
				char productid[PRODUCT_CODE_LENGTH + 1];
				strncpy(productid, game->game.data, PRODUCT_CODE_LENGTH);
				productid[PRODUCT_CODE_LENGTH] = 0;
				
				// Display Name
				char displayname[128];
				memset(displayname, 0, sizeof(displayname));
				
				// SQL Statement
				const char * sql = "SELECT name FROM productids WHERE id=?;";
				
				// Prepared SQL Statement
				sqlite3_stmt * statement = NULL;
				
				// Prepare SQL Statement
				if(sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &statement, NULL) == SQLITE_OK)
				{
					// Bind SQL Statement Data
					if(sqlite3_bind_text(statement, 1, productid, strlen(productid), SQLITE_STATIC) == SQLITE_OK)
					{
						// Found Matching Row
						if(sqlite3_step(statement) == SQLITE_ROW)
						{
							// Fetch Game Name from Database
							const char * gamename = (const char *)sqlite3_column_text(statement, 0);
							
							// Copy Game Name
							strncpy(displayname, gamename, sizeof(displayname) - 1);
						}
						
						// Game not in Database
						else
						{
							// Use Product Code as Name
							strcpy(displayname, productid);
						}
					}
					
					// Destroy Prepared SQL Statement
					sqlite3_finalize(statement);
				}
				
				// Output Game Tag + Game Name
				fprintf(log, "\t<game name=\"%s\" usercount=\"%u\">\n", displayname, game->playercount);
				
				// Activate User Count
				uint32_t activecount = 0;
				
				// Iterate Game Groups
				SceNetAdhocctlGroupNode * group = game->group; for(; group != NULL; group = group->next)
				{
					// Safe Group Name
					char groupname[ADHOCCTL_GROUPNAME_LEN + 1];
					strncpy(groupname, (const char *)group->group.data, ADHOCCTL_GROUPNAME_LEN);
					groupname[ADHOCCTL_GROUPNAME_LEN] = 0;
					
					// Output Group Tag + Group Name + User Count
					fprintf(log, "\t\t<group name=\"%s\" usercount=\"%u\">\n", groupname, group->playercount);
					
					// Iterate Users
					SceNetAdhocctlUserNode * user = group->player; for(; user != NULL; user = user->group_next)
					{
						// Output User Tag + Username
						fprintf(log, "\t\t\t<user>%s</user>\n", (const char *)user->resolver.name.data);
					}
					
					// Output Closing Group Tag
					fprintf(log, "\t\t</group>\n");
					
					// Increase Active Game User Count
					activecount += group->playercount;
				}
				
				// Output Idle Game Group
				if(game->playercount > activecount)
				{
					// Output Group Tag + Group Name + Idle User Count
					fprintf(log, "\t\t<group name=\"Groupless\" usercount=\"%u\" />\n", game->playercount - activecount);
				}
				
				// Output Closing Game Tag
				fprintf(log, "\t</game>\n");
			}
			
			// Close Database
			sqlite3_close(db);
		}
		
		// Output Closing Root Tag
		fprintf(log, "</prometheus>");
		
		// Close Logfile
		fclose(log);
	}
}

