/*
 *   Unreal Internet Relay Chat Daemon, src/modules/m_guest.c
 *   (C) 2000-2001 Carsten V. Munk and the UnrealIRCd Team
 *   Moved to modules by Fish (Justin Hammond)
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 1, or (at your option)
 *   any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "config.h"
#include "struct.h"
#include "common.h"
#include "sys.h"
#include "numeric.h"
#include "msg.h"
#include "channel.h"
#include <time.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#define snprintf _snprintf
#include <io.h>
#endif
#include <fcntl.h>
#include "h.h"
#ifdef STRIPBADWORDS
#include "badwords.h"
#endif
#ifdef _WIN32
#include "version.h"
#endif

DLLFUNC int m_guest(aClient *cptr, aClient *sptr, int parc, char *parv[]);
#ifdef GUEST
static Hook *GuestHook = NULL;
#endif
/* Place includes here */
#ifndef DYNAMIC_LINKING
ModuleHeader m_guest_Header
#else
#define m_guest_Header Mod_Header
ModuleHeader Mod_Header
#endif
  = {
	"guest",	/* Name of module */
	"$Id$", /* Version */
	"command /guest", /* Short description of module */
	"3.2-b8-1",
	NULL 
    };

ModuleInfo *ModGuestInfo;
/* The purpose of these ifdefs, are that we can "static" link the ircd if we
 * want to
*/

/* This is called on module init, before Server Ready */
#ifdef DYNAMIC_LINKING
DLLFUNC int	Mod_Init(ModuleInfo *modinfo)
#else
int    m_guest_Init(ModuleInfo *modinfo)
#endif
{
	/*
	 * We call our add_Command crap here
	*/
#ifdef GUEST
	ModGuestInfo = modinfo;
	GuestHook = HookAddEx(ModGuestInfo->handle, HOOKTYPE_GUEST, m_guest);
#endif
	return MOD_SUCCESS;
	
}

/* Is first run when server is 100% ready */
#ifdef DYNAMIC_LINKING
DLLFUNC int	Mod_Load(int module_load)
#else
int    m_guest_Load(int module_load)
#endif
{
	return MOD_SUCCESS;
	
}


/* Called when module is unloaded */
#ifdef DYNAMIC_LINKING
DLLFUNC int	Mod_Unload(int module_unload)
#else
int	m_guest_Unload(int module_unload)
#endif
{
#ifdef GUEST
	HookDel(GuestHook);
#endif
	return MOD_SUCCESS;
}



DLLFUNC int m_guest(aClient *cptr, aClient *sptr, int parc, char *parv[])
{
	int randnum;
	char guestnick[NICKLEN];
	char *param[2];

	randnum = 1+(int) (99999.0*rand()/(RAND_MAX+10000.0));
	snprintf(guestnick, NICKLEN, "Guest%d", randnum);

	while(find_client(guestnick, (aClient *)NULL))
	{ 
		randnum = 1+(int) (99999.0*rand()/(RAND_MAX+10000.0));
		snprintf(guestnick, NICKLEN, "Guest%d", randnum);
	}
	param[0] = sptr->name;
	param[1] = guestnick;
	m_nick(sptr,cptr,2,param);
	return 0;
}