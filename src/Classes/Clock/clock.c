

#include <intuition/intuition.h>
#include <intuition/classes.h>
#include <intuition/classusr.h>
#include <intuition/cghooks.h>
#include <exec/memory.h>
#include <dos/dos.h>

#include <proto/exec.h>
#include <proto/intuition.h>
#include <proto/dos.h>
#include <proto/locale.h>

#include <string.h>

#include "dockbot.h"
#include "dock_settings.h"

#include "dockbot_protos.h"
#include "dockbot_pragmas.h"

#include "class_def.h"

struct Library *LocaleBase;

#define S_FORMAT "format"
#define DEFAULT_FORMAT "%Q:%M"

ULONG __saveds clock_lib_init(struct ClockLibData* cld)
{
    if( cld->dosBase = OpenLibrary("dos.library", 37) ) {
        DOSBase = (struct DosLibrary *)cld->dosBase;
        if( cld->localeBase = OpenLibrary("locale.library", 37) ) {
            LocaleBase = cld->localeBase;
            return 1;
        }
        CloseLibrary(cld->dosBase);
    }
    cld->localeBase = NULL;
    cld->dosBase = NULL;
    return 0;
}

ULONG __saveds clock_lib_expunge(struct ClockLibData *cld)
{
    if( cld->localeBase ) {
        CloseLibrary(cld->localeBase);
    }

    if( cld->dosBase ) {
        CloseLibrary(cld->dosBase);
    }

    return 1;
}

ULONG __saveds clock_new(Class *c, Object *o, Msg m)
{
    struct ClockGadgetData *cd = INST_DATA(c, o);

    if( cd->locale = OpenLocale(NULL) ) {
        return 1;
    }
    return 0;
}

ULONG __saveds clock_dispose(Class *c, Object *o, Msg m)
{
    struct ClockGadgetData *cd = INST_DATA(c, o);

    if( cd->locale ) {
        CloseLocale(cd->locale);
    }
    
    FREE_STRING(cd->format);

    return 1;
}

VOID __saveds set_text_font(struct IntuiText *text)
{
    struct Screen *screen;
    struct DrawInfo *drawInfo;
    
    if( screen = LockPubScreen(NULL) ) {
    
        if( drawInfo = GetScreenDrawInfo(screen) ) {

            text->ITextFont->ta_Name = drawInfo->dri_Font->tf_Message.mn_Node.ln_Name;
            text->ITextFont->ta_YSize = drawInfo->dri_Font->tf_YSize;
            text->ITextFont->ta_Style = drawInfo->dri_Font->tf_Style;
            text->ITextFont->ta_Flags = drawInfo->dri_Font->tf_Flags;
            text->FrontPen = drawInfo->dri_Pens[TEXTPEN];
            text->BackPen = drawInfo->dri_Pens[BACKGROUNDPEN];
            text->DrawMode = JAM2;

            FreeScreenDrawInfo(screen, drawInfo);
        }

        UnlockPubScreen(NULL, screen);
    }     
}

ULONG __saveds clock_draw(Class *c, Object *o, Msg m)
{   
    struct Rect b;
    struct IntuiText text;
    struct TextAttr ta;
    struct ClockGadgetData *g = INST_DATA(c, o);
    struct DockMessageDraw *d = (struct DockMessageDraw *)m;
    UWORD w;

    DB_GetDockGadgetBounds(o, &b);

    DB_DrawOutsetFrame(d->rp, &b);

    text.ITextFont = &ta;
    set_text_font(&text);

    text.ITextFont = &ta;
    text.IText = g->time;
    text.NextText = NULL;
            
    w = IntuiTextLength(&text);

    text.LeftEdge = b.x + (b.w - w) / 2;
    text.TopEdge = b.y + (b.h - text.ITextFont->ta_YSize) / 2;

    PrintIText(d->rp, &text, 0, 0);

    return 1;    
}

VOID __saveds __asm write_char(
    register __a0 struct Hook *hook,
    register __a2 void *object,
    register __a1 char c)
{
    struct ClockGadgetData *cd = (struct ClockGadgetData *)hook->h_Data;

    if( cd->pos < MAX_DISPLAY_LENGTH ) {
        cd->time[cd->pos++] = c;
    }
}

VOID __saveds format_time(struct ClockGadgetData *cd, struct DateStamp *ds)
{
    struct Hook hook = {
        { NULL, NULL },
        (unsigned long (*)())write_char,
        NULL,
        NULL
    };
    hook.h_Data = cd;
    
    cd->pos = 0;
    FormatDate(cd->locale, cd->format, ds, &hook);
}

ULONG __saveds clock_tick(Class *c, Object *o, Msg m)
{
    struct DateStamp ds;
    struct ClockGadgetData *cd = INST_DATA(c, o);

    if( cd->counter > 0 ) {
        cd->counter--;
        return 1;
    }

    cd->counter = 20;

    DateStamp(&ds);
    
    if( ds.ds_Minute != cd->minutes ) {
    
        cd->minutes = ds.ds_Minute;

        format_time(cd, &ds);
    
        DB_RequestDockGadgetDraw(o);
    }

    return 1;
}

ULONG __saveds clock_get_size(Class *c, Object *o, Msg m)
{
    struct Screen *screen;
    struct DrawInfo *drawInfo;
    struct DockMessageGetSize *s = (struct DockMessageGetSize *)m;

    if( screen = LockPubScreen(NULL) ) {
    
        if( drawInfo = GetScreenDrawInfo(screen) ) {

            s->h = drawInfo->dri_Font->tf_YSize + 4;
            s->w = DEFAULT_SIZE;

            FreeScreenDrawInfo(screen, drawInfo);
        }

        UnlockPubScreen(NULL, screen);            
    }    

    return 1;
}


ULONG __saveds clock_read_config(Class *c, Object *o, Msg m)
{
    struct DockMessageReadSettings *r = (struct DockMessageReadSettings*)m;
    struct ClockGadgetData *cd = INST_DATA(c, o);
    struct DockSettingValue v;

    while( DB_ReadSetting(r->settings, &v) ) {
        if( IS_KEY(S_FORMAT, v) ) {
            GET_STRING(v, cd->format)
        }
    }

    if( ! cd->format ) {
        if( cd->format = (STRPTR)DB_AllocMem(strlen(DEFAULT_FORMAT) + 1, MEMF_CLEAR) ) {
            CopyMem(DEFAULT_FORMAT, cd->format, strlen(DEFAULT_FORMAT) + 1);
        }
    }

    return 1;
}
