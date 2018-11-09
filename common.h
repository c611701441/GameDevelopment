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
#include <SDL2/SDL.h>
#include <libcwiimote/wiimote.h>
#include <libcwiimote/wiimote_api.h>

#define PORT			(u_short)8888	/* ポート番号*/

#define MAX_CLIENTS		4				/* クライアントの最大数 */
#define MAX_NAME_SIZE	10 				/* ユーザー名の最大数*/

#define MAX_DATA		200				/* 送受信するデータの最大数*/

#define END_COMMAND		'E'		  		/* プログラム終了コマンド*/


extern int clientID;
extern wiimote_t wiimote;//wiiリモコンを用いるための構造体を宣言

/*キャラクターの構造体*/
typedef struct {
    int hp; /*体力*/
    int at; /*攻撃力*/
    int sp; /*スピード*/
    int item; /*所持アイテムの番号*/
    int key; /*鍵を持っているかどうか*/
    SDL_Rect rect; /*座標*/
}Character;

/*キャラクターの変数定義*/
extern Character player[4];//player[0]~[2]は逃走者、player[3]は鬼です
/*wiiリモコンの変数の定義*/
wiimote_t wiimote;
/*マップサイズ*/
enum {
    MAP_width = 4000,/*横幅*/
    MAP_high = 2800,/*高さ*/
};

#endif
