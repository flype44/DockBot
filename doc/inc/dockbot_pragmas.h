@DATABASE "dockbot_pragmas.h"
@MASTER   "Stuff:Andrew/DockBot/include/dockbot_pragmas.h"
@REMARK   This file was created by ADtoHT 2.1 on 05-May-20 21:03:26
@REMARK   Do not edit
@REMARK   ADtoHT is � 1993-1995 Christian Stieber

@NODE MAIN "dockbot_pragmas.h"

@{"dockbot_pragmas.h" LINK File}

@ENDNODE
@NODE File "dockbot_pragmas.h"
/************************************/
/*                                  */
/*  DockBot - A Dock For AmigaOS 3  */
/*                                  */
/*  � 2016 Andrew Kennan            */
/*                                  */
/************************************/
/* Dock gadget functions*/
#pragma libcall DockBotBase DB_GetDockGadgetEnvironment 1e 9802
#pragma libcall DockBotBase DB_RequestDockQuit 24 801
#pragma libcall DockBotBase DB_RequestDockGadgetDraw 2a 801
#pragma libcall DockBotBase DB_RequestLaunch 30 0BA9805
/* Drawing utilities*/
#pragma libcall DockBotBase DB_DrawOutsetFrame 36 9802
#pragma libcall DockBotBase DB_DrawInsetFrame 3c 9802
/* Settings functions*/
#pragma libcall DockBotBase DB_OpenSettingsRead 42 801
#pragma libcall DockBotBase DB_OpenSettingsWrite 48 801
#pragma libcall DockBotBase DB_CloseSettings 4e 801
#pragma libcall DockBotBase DB_ReadBeginBlock 54 801
#pragma libcall DockBotBase DB_ReadEndBlock 5a 801
#pragma libcall DockBotBase DB_ReadSetting 60 9802
#pragma libcall DockBotBase DB_WriteBeginBlock 66 801
#pragma libcall DockBotBase DB_WriteEndBlock 6c 801
#pragma libcall DockBotBase DB_WriteSetting 72 A9803
#pragma libcall DockBotBase DB_ReadConfig 78 9802
#pragma libcall DockBotBase DB_WriteConfig 7e 9802
/* Memory Management*/
#pragma libcall DockBotBase DB_AllocMem 84 1002
#pragma libcall DockBotBase DB_FreeMem 8a 0802
#pragma libcall DockBotBase DB_GetMemInfo 90 0
/* Class Management*/
#pragma libcall DockBotBase DB_CreateDockGadget 96 801
#pragma libcall DockBotBase DB_ListClasses 9c 801
/* User Messages*/
#pragma libcall DockBotBase DB_ShowError a2 801
/* Gadget Message Ports*/
#pragma libcall DockBotBase DB_RegisterPort a8 9802
#pragma libcall DockBotBase DB_UnregisterPort ae 9802
/* Config cleanup*/
#pragma libcall DockBotBase DB_DisposeConfig b4 801
/* More gadget stuff*/
#pragma libcall DockBotBase DB_DisposeDockGadget ba 801
#pragma libcall DockBotBase DB_FreeGadget c0 801
#pragma libcall DockBotBase DB_AllocGadget c6 801
/* Debug Logging*/
#pragma libcall DockBotBase DB_RegisterDebugStream cc 801
#pragma libcall DockBotBase DB_DebugLog d2 9802
/* Brushes*/
#pragma libcall DockBotBase DB_LoadBrush d8 0802
#pragma libcall DockBotBase DB_FreeBrush de 801
#pragma libcall DockBotBase DB_DrawBrush e4 5432109808
#pragma libcall DockBotBase DB_GetBrushSize ea A9803
/* File Requester*/
#pragma libcall DockBotBase DB_SelectFile f0 BA9804
@ENDNODE
