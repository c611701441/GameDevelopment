/*****************************************************************
ファイル名	: common.h
機能		: サーバーとクライアントで使用する定数の宣言を行う
*****************************************************************/

#ifndef _COMMON_H_
#define _COMMON_H_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<assert.h>
#include<math.h>
#include<time.h>
#include<SDL2/SDL_mixer.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include<SDL2/SDL_mixer.h>
#include <libcwiimote/wiimote.h>
#include <libcwiimote/wiimote_api.h>

#define PORT			(u_short)8888	/* ポート番号*/

#define MAX_CLIENTS		4				/* クライアントの最大数 */
#define MAX_NAME_SIZE	10 				/* ユーザー名の最大数*/

#define MAX_DATA		200				/* 送受信するデータの最大数*/

#define END_COMMAND		'E'		  		/* プログラム終了コマンド*/
#define RECT_COMMAND            'R'   /*座標送信コマンド*/
#define START_COMMAND   'S' /*ゲーム開始コマンド*/
#define ITEM_COMMAND   'I' /*アイテム取得コマンド*/
#define CLEAR_COMMAND   'C'   /*ゲームクリアコマンド*/
#define OVER_COMMAND     'O'    /*ゲームオーバーコマンド*/
#define GOAL_COMMAND      'G'/*ゴール変数共有コマンド*/
#define DEAD_COMMAND  'D' /*死亡人数確認コマンド*/

extern int clientID;
extern wiimote_t wiimote;//wiiリモコンを用いるための構造体を宣言
extern SDL_Rect charawin;//全体マップでのキャラ座標 

/*キャラクターの構造体*/
typedef struct {
    int state;/*キャラクターの状態*/
    int hp; /*体力*/
    int at; /*攻撃力*/
    int sp; /*スピード*/
    int item; /*所持アイテムの番号*/
    int key; /*鍵を持っているかどうか*/
    SDL_Rect rect; /*座標*/
    SDL_Rect rect_center;/*キャタクターの中心の座標*/
    int r;/*キャタクターの半径*/
    int angle;/*キャラクタの方向角度*/
}Character;

typedef struct{
SDL_Rect rect; /*全体マップの座標*/
}Window;




/*ウィンドウの変数定義*/
extern Window window;

/*キャラクターの変数定義*/
extern Character player[4];//player[0]~[2]は逃走者、player[3]は鬼です

/*wiiリモコンの変数の定義*/
wiimote_t wiimote;

/*マップのブロックの変数定義*/
extern int block[41][29];

/*アイテムの座標の変数定義*/
extern SDL_Rect item_rect[4][3];




/*マップサイズ*/
enum {
    MAP_width = 4000,/*横幅*/
    MAP_high = 2800,/*高さ*/
};
extern int id;

extern int x1,y,angle1,sp1,id1,state1,item1,key1,r1;
extern int x2,y2,angle2,sp2,id2,state2,item2,key2,r2;
extern int x3,y3,angle3,sp3,id3,state3,item3,key3,r3;
extern int x4,y4,angle4,sp4,id4,state4,item4,key4,r4;

extern int recttime;
extern int phase;
extern int item_flag;
extern int key_flag;
extern int GOAL;
#endif
