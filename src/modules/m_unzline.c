/*
 *   Unreal Internet Relay Chat Daemon, src/modules/m_unzline.c
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
#include <io.h>
#endif
#include <fcntl.h>
#include "h.h"
#include "proto.h"
#ifdef STRIPBADWORDS
#include "badwords.h"
#endif
#ifdef _WIN32
#include "version.h"
#endif

DLLFUNC int m_unzline(aClient *cptr, aClient *sptr, int parc, char *parv[]);

/* Place includes here */
#define MSG_UNZLINE     "UNZLINE"       /* UNZLINE */
#define TOK_UNZLINE     "r"     /* 113 */ 


#ifndef DYNAMIC_LINKING
ModuleHeader m_unzline_Header
#else
#define m_unzline_Header Mod_Header
ModuleHeader Mod_Header
#endif
  = {
	"unzline",	/* Name of module */
	"$Id$", /* Version */
	"command /unzline", /* Short description of module */
	"3.2-b8-1",
	NULL 
    };


/* The purpose of these ifdefs, are that we can "static" link the ircd if we
 * want to
*/

/* This is called on module init, before Server Ready */
#ifdef DYNAMIC_LINKING
DLLFUNC int	Mod_Init(ModuleInfo *modinfo)
#else
int    m_unzline_Init(ModuleInfo *modinfo)
#endif
{
	/*
	 * We call our add_Command crap here
	*/
	add_Command(MSG_UNZLINE, TOK_UNZLINE, m_unzline, MAXPARA);
	return MOD_SUCCESS;
}

/* Is first run when server is 100% ready */
#ifdef DYNAMIC_LINKING
DLLFUNC int	Mod_Load(int module_load)
#else
int    m_unzline_Load(int module_load)
#endif
{
	return MOD_SUCCESS;
}


/* Called when module is unloaded */
#ifdef DYNAMIC_LINKING
DLLFUNC int	Mod_Unload(int module_unload)
#else
int	m_unzline_Unload(int module_unload)
#endif
{
	if (del_Command(MSG_UNZLINE, TOK_UNZLINE, m_unzline) < 0)
	{
		sendto_realops("Failed to delete commands when unloading %s",
				m_unzline_Header.name);
	}
	return MOD_SUCCESS;
}


/*
 *  m_unzline                        remove a temporary zap line
 *    parv[0] = sender prefix
 *    parv[1] = host
 */

DLLFUNC int m_unzline(aClient *cptr, aClient *sptr, int parc, char *parv[])
{
        if (!MyClient(sptr) || !OPCanZline(sptr))
        {
                sendto_one(sptr, err_str(ERR_NOPRIVILEGES), me.name, parv[0]);
                return 0;
        }

        sendto_one(sptr, ":%s NOTICE %s :Please use /zline -user@host", me.name, parv[0]);

	return 0;
}