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
