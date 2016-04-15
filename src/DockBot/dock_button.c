
#include <intuition/intuition.h>
#include <intuition/classes.h>
#include <dos/dos.h>
#include <dos/dostags.h>
#include <workbench/workbench.h>

#include <clib/intuition_protos.h>
#include <clib/alib_protos.h>
#include <clib/graphics_protos.h>
#include <clib/dos_protos.h>

/****
** Icon Library v44+
*/
extern struct Library *IconBase;
#define CONST

#include "iconlib/icon.h"
#include "iconlib/icon_protos.h"
#include "iconlib/icon_pragmas.h"

/**
****/

#include <stdio.h>

#include "dockbot.h"
#include "dockbot_protos.h"
#include "dockbot_pragmas.h"

#include "dock_gadget.h"
#include "dock_button.h"
#include "dock_settings.h"

#define CLASS_NAME "dockbutton"

#define S_NAME "name"
#define S_PATH "path"
#define S_ICON "icon"
#define S_START "start"
#define S_ARGS "args"
#define S_CON "console"

#define ST_WB 0
#define ST_SH 1

#define DEFAULT_CONSOLE "NIL:"

#define WBSTART "C:WBRun"

struct DockButtonData
{
    STRPTR name;
    STRPTR path;
    STRPTR icon;
    STRPTR args;
    STRPTR con;
    UWORD imageW;
    UWORD imageH;
    struct DiskObject *diskObj;
    UWORD startType;
};

struct Values StartValues[] = {
    { "wb", ST_WB },
    { "sh", ST_SH },
    { NULL, 0 }
};

VOID dock_button_draw(Object *o, struct DockButtonData *dbd, struct DockMessageDraw *dmd)
{
    struct Rect bounds;
    
    GetDockGadgetBounds(o, &bounds);  

    if( dbd->diskObj ) {

        DrawIconStateA(dmd->rp, dbd->diskObj, 
                NULL, 
                bounds.x + (bounds.w - dbd->imageW) / 2, 
                bounds.y + (bounds.h - dbd->imageH) / 2,
                0, NULL); 
    }

    DrawOutsetFrame(dmd->rp, &bounds);

}

VOID dock_button_get_size(struct DockButtonData *dbd, struct DockMessageGetSize *gsm) {
    struct Rectangle r;

    gsm->w = DEFAULT_SIZE;
    gsm->h = DEFAULT_SIZE;

    if( dbd->diskObj ) {
        if( GetIconRectangleA(NULL, dbd->diskObj, NULL, &r, NULL) ) {
            gsm->w = r.MaxX - r.MinX + 1;
            if( gsm->w < DEFAULT_SIZE ) {
                gsm->w = DEFAULT_SIZE;
            }
            gsm->h = r.MaxY - r.MinY + 1;
            if( gsm->h < DEFAULT_SIZE ) {
                gsm->h = DEFAULT_SIZE;
            }
        }
    }

    dbd->imageW = gsm->w;
    dbd->imageH = gsm->h;
}

VOID read_button_settings(struct DockButtonData *dbd, struct DockMessageReadSettings *m)
{
    struct DockSettingValue v;
    struct Values *vals;
    struct Screen *screen;
    UWORD len;

    while( ReadSetting(m->settings, &v) ) {
        
        if( IS_KEY(S_NAME, v) ) {
            GET_STRING(v, dbd->name)     
        }
        else if( IS_KEY(S_PATH, v) ) {
            GET_STRING(v, dbd->path)
        }
        else if( IS_KEY(S_ICON, v) ) {
            GET_STRING(v, dbd->icon)
        }
        else if( IS_KEY(S_START, v) ) {
            GET_VALUE(v, StartValues, vals, len, dbd->startType)
        }
        else if( IS_KEY(S_ARGS, v) ) {
            GET_STRING(v, dbd->args)
        }
        else if( IS_KEY(S_CON, v) ) {
            GET_STRING(v, dbd->con)
        }
    }    

    if( ! dbd->icon ) {
        if( dbd->diskObj = GetDiskObjectNew(dbd->path) ) {
            if( screen = LockPubScreen(NULL) ) {

                LayoutIconA(dbd->diskObj, screen, NULL);

                UnlockPubScreen(NULL, screen);
            }
        }
    }
}

VOID dispose_button_data(struct DockButtonData *dbd) 
{
    FREE_STRING(dbd->name);
    FREE_STRING(dbd->path);
    FREE_STRING(dbd->icon);
    FREE_STRING(dbd->con);    

    if( dbd->diskObj ) {
        FreeDiskObject(dbd->diskObj);
    }
}

VOID dock_button_click(struct DockButtonData *dbd, Msg msg) 
{
    STRPTR cmd;
    STRPTR con;
    ULONG wbslen, plen, alen, clen;
    BPTR fhIn;
    BPTR fhOut;

    struct TagItem shellTags[] = {
        { SYS_UserShell, TRUE },
        { SYS_Asynch, TRUE },
        { SYS_Input, NULL },
        { SYS_Output, NULL },
        { TAG_DONE, 0 }
    };

    plen = strlen(dbd->path);
    alen = strlen(dbd->args);
    if( dbd->startType == ST_SH ) {
        clen = plen + alen + 2;
        if( cmd = (STRPTR)AllocMem(clen, MEMF_CLEAR) ) {
            if( fhIn = Open(DEFAULT_CONSOLE, MODE_OLDFILE) ) {
                con = dbd->con;
                if( con == NULL ) {
                    con = DEFAULT_CONSOLE;
                }
                if( fhOut = Open(con, MODE_OLDFILE) ) {

                    shellTags[2].ti_Data = fhIn;
                    shellTags[3].ti_Data = fhOut;

                    CopyMem(dbd->path, cmd, plen);
                    cmd[plen] = ' ';
                    CopyMem(dbd->args, cmd + plen + 1, alen);
                    cmd[plen + alen + 1] = '\0';

                    if( SystemTagList(cmd, (struct TagItem*)&shellTags) == -1 ) {
                        Close(fhIn);
                        Close(fhOut);
                    }
                } else {
                    Close(fhIn);
                }
            } 
            FreeMem(cmd, clen);
        }       
    } else {
        wbslen = strlen(WBSTART);
        clen = wbslen + plen + alen + 3;
        if( cmd = (STRPTR)AllocMem(clen, MEMF_CLEAR) ) {
            if( fhIn = Open(DEFAULT_CONSOLE, MODE_OLDFILE) ) {
                if( fhOut = Open(DEFAULT_CONSOLE, MODE_OLDFILE) ) {
                    shellTags[2].ti_Data = fhIn;
                    shellTags[3].ti_Data = fhOut;

                    CopyMem(WBSTART, cmd, wbslen);
                    cmd[wbslen] = ' ';
                    CopyMem(dbd->path, cmd + wbslen + 1, plen);
                    cmd[wbslen + plen + 1] = ' ';
                    CopyMem(dbd->args, cmd + wbslen + plen + 2, alen);
                    cmd[wbslen + plen + alen + 2] = '\0';

                    if( SystemTagList(cmd, (struct TagItem*)&shellTags) == -1 ) {
                        Close(fhIn);
                        Close(fhOut);
                    }
                } else {
                    Close(fhIn);
                }
            }
            FreeMem(cmd, clen);
        }
    }
}

ULONG __saveds dock_button_dispatch(Class *c, Object *o, Msg msg)
{

    switch( msg->MethodID ) 
    {
		case OM_NEW:	
			return DoSuperMethodA(c, o, msg);

        case OM_DISPOSE:
            dispose_button_data(INST_DATA(c,o));
            return DoSuperMethodA(c, o, msg);            

        case DM_CLICK:
            dock_button_click(INST_DATA(c,o), msg);
            break;

        case DM_DRAW:
            dock_button_draw(o, INST_DATA(c,o), (struct DockMessageDraw*)msg);
            break;

        case DM_GETSIZE:
            dock_button_get_size(INST_DATA(c,o), (struct DockMessageGetSize*)msg);
            break;

        case DM_READCONFIG:
            read_button_settings(INST_DATA(c, o), (struct DockMessageReadSettings*)msg);
            break;

        default:
            return DoSuperMethodA(c, o, msg);
    }

    return NULL;
}

Class *init_dock_button_class(VOID) 
{
    ULONG HookEntry();
    Class *c = NULL;
    if( c = MakeClass(CLASS_NAME, DB_ROOT_CLASS, NULL, sizeof(struct DockButtonData), 0) )
    {
        c->cl_Dispatcher.h_Entry = HookEntry;
        c->cl_Dispatcher.h_SubEntry = dock_button_dispatch;

        AddClass(c);
    }

    return c;
}

VOID free_dock_button_class(Class *c)
{
    RemoveClass(c);

    FreeClass(c);
}
