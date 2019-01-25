/*****************************************************************
ファイル名	: client_func.h
機能		: クライアントの外部関数の定義
*****************************************************************/

#ifndef _CLIENT_FUNC_H_
#define _CLIENT_FUNC_H_

#include"common.h"

/* client_net.c */
extern int SetUpClient(char* hostName,int *clientID,int *num,char clientName[][MAX_NAME_SIZE]);
extern void CloseSoc(void);
extern int RecvIntData(int *intData);
extern void SendData(void *data,int dataSize);
extern int SendRecvManager(void);

/* client_win.c */
extern int InitWindows(int clientID,int num,char name[][MAX_NAME_SIZE]);
extern void DestroyWindow(void);
extern void WindowEvent(int num, int starttime);
extern int Wii_Event(void);
extern void DrawOthersPlayer(int other_x,int other_y,int other_angle,int other_id);
extern void MapDisp(void);
extern void title(void);
extern void GameClear(void);
extern void GameOver(void);

/* client_command.c */
extern void PlayerMove(void);
extern int ExecuteCommand(char command);
extern void SendEndCommand(void);
extern void SendRectCommand(void);
extern void MoveOthersPlayer(int x,int y,int angle,int sp,int id);
extern int RAND(int b , int seed );
extern void ChangeCenter(void);
extern void Digital(int *dx, int *dy ,int x ,int y);
extern void SendStartCommand(void);
extern void SendItemCommand(int dx,int dy);
extern void SendClearCommand(void);
extern void SendOverCommand(void);
extern void SendGoalCommand(void);
extern void SendDeadCommand(void);
extern void SendKeyCommand(int dx,int dy);
extern void SendSetItemCommand(int w,int h,int  itemnum);
#endif
