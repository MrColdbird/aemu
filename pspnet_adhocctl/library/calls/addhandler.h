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

#ifndef _ADHOCCTL_ADDHANDLER_H_
#define _ADHOCCTL_ADDHANDLER_H_

/**
 * Register (up to 4) Adhocctl Event Handler
 * @param handler To-be-registered Adhocctl Handler
 * @param arg Optional to-be-passed Parameter (Get's passed to Handler in case of Event)
 * @return Handler ID > 0 on success or... ADHOCCTL_NOT_INITIALIZED, ADHOCCTL_INVALID_ARG, ADHOCCTL_HANDLER_MAX
 */
int proNetAdhocctlAddHandler(SceNetAdhocctlHandler handler, void * arg);

#endif
