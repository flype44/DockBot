/************************************
**
**  DockBot - A Dock For AmigaOS 3
**
**  � 2016 Andrew Kennan
**
************************************/

#ifndef __DOCK_GADGET_H__
#define __DOCK_GADGET_H__

#include <intuition/intuition.h>
#include <intuition/classes.h>

VOID dock_gadget_added(Object *obj, struct MsgPort *dockPort);

VOID dock_gadget_removed(Object *obj);

VOID dock_gadget_tick(Object *obj);

VOID dock_gadget_click(Object *obj, UWORD x, UWORD y);

VOID dock_gadget_drop(Object *obj, STRPTR *paths, UWORD count);

VOID dock_gadget_launched(Object *obj, STRPTR path, STRPTR args, STRPTR console, BOOL wb);

VOID dock_gadget_set_environment(Object *obj, struct GadgetEnvironment *env);

VOID dock_gadget_get_size(Object *obj
			, DockPosition position, DockAlign align
			, UWORD *w, UWORD *h);

VOID dock_gadget_draw(Object *obj, struct RastPort *rp);

VOID dock_gadget_remap(Object *obj);

BOOL dock_gadget_hit_test(Object *obj, UWORD x, UWORD y);

VOID dock_gadget_read_settings(Object *obj, struct DockSettings *settings);

BOOL dock_gadget_builtin(Object *obj);

VOID dock_gadget_get_info(Object *obj
            , STRPTR *name, STRPTR *version
            , STRPTR *description, STRPTR *copyright);

VOID dock_gadget_get_hotkey(Object *obj
			, STRPTR *hotKey);

VOID dock_gadget_hotkey(Object *obj);

VOID dock_gadget_get_label(Object *obj, STRPTR *label);

BOOL dock_gadget_can_edit(Object *obj);

struct TagItem * dock_gadget_get_editor(Object *obj);

VOID dock_gadget_editor_event(Object *obj, struct TR_Project *window, struct TR_Message *message);

VOID dock_gadget_editor_update(Object *obj, struct TR_Project *window);

VOID dock_gadget_init_edit(Object *obj, struct TR_App *app);

VOID dock_gadget_init_button(Object *obj, STRPTR name, STRPTR path);

VOID dock_gadget_message(Object *obj, struct MsgPort *port);

#endif 
