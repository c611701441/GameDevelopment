/*****************************************************************
ファイル名	: client_win.c
機能		: クライアントのユーザーインターフェイス処理
*****************************************************************/

#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include"common.h"
#include"client_func.h"

static int CheckButtonNO(int x,int y,int num);
static void SetCharData2DataBlock(void *data,char charData,int *dataSize);
static void SetIntData2DataBlock(void *data,int intData,int *dataSize);
static void blockset(void);
static void  BlockDrow( int blockname , SDL_Rect dst_rect);


static SDL_Surface *gMainWindow,*str;

static SDL_Rect gButtonRect[MAX_CLIENTS+2];
SDL_Texture *texture,*texture_player,*texture_others1,*texture_others2,*texture_others3,*texture_others4;
SDL_Texture *texture_wall , *texture_key;

SDL_Surface *image,*image_player;
SDL_Surface *image_wall , *image_key;

SDL_Renderer *renderer;
//extern�ؿ�

SDL_Rect charawin; //MapDrawで使用
Window window;

/*****************************************************************
関数名	: InitWindows
機能	: メインウインドウの表示、設定を行う
引数	: int	clientID		:　クライアント番号
		  int	num				: 全クライアント数
出力	: 正常に設定できたとき0,失敗したとき-1\
*****************************************************************/
int InitWindows(int clientID,int num,char name[][MAX_NAME_SIZE])
{
	int i;
	

	char endButton[]="END.jpg";
	char *s,title[10];

    /* 引数チェック */
    assert(0<num && num<=MAX_CLIENTS);
	
	/* SDLの初期化 */
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("failed to initialize SDL.\n");
		return -1;
	}
	
	/* メインウインドウを生成する */
	gMainWindow = SDL_CreateWindow("GAME",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,1000,700,0);
	renderer = SDL_CreateRenderer(gMainWindow,-1,SDL_RENDERER_SOFTWARE);
	/*  */
	SDL_RenderPresent(renderer);
        
        image = IMG_Load("testMap.png");
        texture = SDL_CreateTextureFromSurface(renderer,image);
        image_player = IMG_Load("test.png");
        image_wall = IMG_Load("wall.png");
        image_key = IMG_Load("key.png");
        texture_player = SDL_CreateTextureFromSurface(renderer,image_player);
	texture_others1 = SDL_CreateTextureFromSurface(renderer,image_player);
        texture_others2 = SDL_CreateTextureFromSurface(renderer,image_player);
        texture_others3 = SDL_CreateTextureFromSurface(renderer,image_player);
        texture_others4 = SDL_CreateTextureFromSurface(renderer,image_player);
        texture_wall = SDL_CreateTextureFromSurface(renderer,image_wall);
        texture_key = SDL_CreateTextureFromSurface(renderer,image_key);
	return 0;
}

/*****************************************************************
関数名	: DestroyWindow
機能	: SDLを終了する
引数	: なし
出力	: なし
*****************************************************************/
void DestroyWindow(void)
{
    SDL_FreeSurface(image);
    SDL_FreeSurface(image_player);
    SDL_DestroyTexture(texture);
    SDL_DestroyTexture(texture_player);
    SDL_DestroyTexture(texture_others1);
    SDL_DestroyTexture(texture_others2);
    SDL_DestroyTexture(texture_others3);
    SDL_DestroyTexture(texture_others4);
    SDL_DestroyRenderer(renderer); // レンダラーの破棄
    SDL_DestroyWindow(gMainWindow); // ウィンドウの破棄
    SDL_Quit();
}

void MapDisp(void)
{
    SDL_Rect src_rect_map = {player[clientID].rect.x - 450 ,player[clientID].rect.y - 300 ,1000, 700};
    SDL_Rect dst_rect_map = {0,0,1000,700};
    SDL_RenderCopy(renderer,texture,&src_rect_map,&dst_rect_map);
}

void PlayerDisp(void)
{
    SDL_Rect src_rect_player = {0,0,image_player->w,image_player->h};
    SDL_Rect dst_rect_player = {450 ,300 ,100,100};
    SDL_RenderCopy(renderer,texture_player,&src_rect_player,&dst_rect_player);
}

/*****************************************************************
関数名	: WindowEvent
機能	: メインウインドウに対するイベント処理を行う
引数	: int		num		: 全クライアント数
出力	: なし
*****************************************************************/
void WindowEvent(int num)
{
	SDL_Event event;
	SDL_MouseButtonEvent *mouse;
	int buttonNO;
	int dataSize;
	unsigned char data[MAX_DATA];
	

    /* 引き数チェック */
    assert(0<num && num<=MAX_CLIENTS);
    
    if(SDL_PollEvent(&event)){
        switch(event.type){
        case SDL_QUIT:
            SendEndCommand();
            break;
        
        }
    }
    MapDisp();
     blockset();/*障害物などを描画*/
    PlayerDisp();
    //MapDraw();
   
    MoveOthersPlayer(x1,y,angle1,sp1,id1);
    MoveOthersPlayer(x2,y2,angle2,sp2,id2);
    MoveOthersPlayer(x3,y3,angle3,sp3,id3);
    MoveOthersPlayer(x4,y4,angle4,sp4,id4);
        
    SDL_RenderPresent(renderer);
}


/******************************************
関数名　：MapDraw
機能　　：マップ全体でのスクロール
引数　　：なし
 *****************************************/
void MapDraw(void){
    //x軸方向の処理
    if(450 <= player[clientID].rect.x  &&  player[clientID].rect.x  <= 3450){
        window.rect.x = player[clientID].rect.x - 450;
    }else if(3450 < player[clientID].rect.x && player[clientID].rect.x <= 3900){
        charawin.x = player[clientID].rect.x - 3000;
    }else{
        charawin.x = player[clientID].rect.x; 
    }

    SDL_RenderPresent(renderer);
}




/**********************************************
関数名    :DrawOthersPlayer
機能        :メインウインドウに他のプレイヤーを表示する
引数        :int        x             :他のプレイヤーの x 座標
                 int        y             :他のプレイヤーの y 座標
出力        :なし
**********************************************/
void DrawOthersPlayer(int other_x,int other_y)
{
    #ifndef NDEBUG
    printf("#####\n");
    printf("DrawOthersPlayer()\n");
    printf("x=%d\ny=%d\n",other_x,other_y);
    #endif
    SDL_Rect src_rect_others1={0,0,image_player->w,image_player->h};
    SDL_Rect dst_rect_others1={other_x-player[clientID].rect.x+450,other_y-player[clientID].rect.y+300,100,100};
    SDL_RenderCopy(renderer,texture_others1,&src_rect_others1,&dst_rect_others1);
/*
    if(other_id==0)
    {
        SDL_Rect src_rect_others1={0,0,image_player->w,image_player->h};
        SDL_Rect dst_rect_others1={other_x-player[clientID].rect.x+450,other_y-player[clientID].rect.y+300,100,100};
        SDL_RenderCopy(renderer,texture_others1,&src_rect_others1,&dst_rect_others1);
    }
    if(other_id==1)
    {
        SDL_Rect src_rect_others2={0,0,image_player->w,image_player->h};
        SDL_Rect dst_rect_others2={other_x-player[clientID].rect.x+450,other_y-player[clientID].rect.y+300,100,100};
        SDL_RenderCopy(renderer,texture_others2,&src_rect_others2,&dst_rect_others2);
    }
    if(other_id==2)
    {
        SDL_Rect src_rect_others3={0,0,image_player->w,image_player->h};
        SDL_Rect dst_rect_others3={other_x-player[clientID].rect.x+450,other_y-player[clientID].rect.y+300,100,100};
        SDL_RenderCopy(renderer,texture_others3,&src_rect_others3,&dst_rect_others3);
    }
    if(other_id==3)
    {
        SDL_Rect src_rect_others4={0,0,image_player->w,image_player->h};
        SDL_Rect dst_rect_others4={other_x-player[clientID].rect.x+450,other_y-player[clientID].rect.y+300,100,100};
        SDL_RenderCopy(renderer,texture_others4,&src_rect_others4,&dst_rect_others4);
    }
                                                                                     */
    //SDL_RenderPresent(renderer);
}


/*****
static
*****/


/*****************************************************************
関数名	: SetIntData2DataBlock
機能	: int 型のデータを送信用データの最後にセットする
引数	: void		*data		: 送信用データ
		  int		intData		: セットするデータ
		  int		*dataSize	: 送信用データの現在のサイズ
出力	: なし
*****************************************************************/
static void SetIntData2DataBlock(void *data,int intData,int *dataSize)
{
    int tmp;
    
    /* 引き数チェック */
    assert(data!=NULL);
    assert(0<=(*dataSize));
     tmp = htonl(intData);
     /* int 型のデータを送信用データの最後にコピーする */
    memcpy(data + (*dataSize),&tmp,sizeof(int));
    /* データサイズを増やす */
    (*dataSize) += sizeof(int);
}
 /*****************************************************************
関数名	: SetCharData2DataBlock
機能	: char 型のデータを送信用データの最後にセットする
引数	: void		*data		: 送信用データ
		  int		intData		: セットするデータ
		  int		*dataSize	: 送信用データの現在のサイズ
出力	: なし
*****************************************************************/
static void SetCharData2DataBlock(void *data,char charData,int *dataSize)
{
    /* 引き数チェック */
    assert(data!=NULL);
    assert(0<=(*dataSize));
     /* char 型のデータを送信用データの最後にコピーする */
    *(char *)(data + (*dataSize)) = charData;
    /* データサイズを増やす */
    (*dataSize) += sizeof(char);
}


/*****************************************************************
関数名	: blockset
機能	: 障害物を表示する
引数	: なし
出力	: なし
*****************************************************************/
void blockset(void)
{
    int i,j;
    int dx, dy;
    SDL_Rect blockpoint;
    Digital(&dx, &dy);
    
    for( i = -3; i < 5 ; i++ )
    {
        for( j = -5 ; j < 7 ; j++)
        {
            if(dx + j >= 0 && dx + j <= 39 && dy + i  >= 0 && dy + i <= 27 )
            {
                if ( block[dx + j ][dy + i ] > 0) 
                {
                    blockpoint.x = player[clientID].rect.x - dx * 100 + ( j + 5 ) * 100;
                    blockpoint.y = player[clientID].rect.y - dy * 100 + ( i + 3 ) * 100;
                    printf("***%d,%d,%d,%d,%d , %d***\n",dx,dy,j,i, blockpoint.x,blockpoint.y);
                    BlockDrow(block[dx + j ][dy + i ] , blockpoint);
                }
                
            }
        }
    }
}


/*****************************************************************
関数名	: BlockDrow
機能	: 障害物を描画する
引数	: int 
出力	: なし
*****************************************************************/
void  BlockDrow( int blockname , SDL_Rect dst_rect)
{
    SDL_Rect src_rect = {0 , 0 ,100 , 100 };
    switch (blockname)
    {
    case 1:
        SDL_RenderCopy(renderer,texture_wall,&src_rect,&dst_rect);
        break;
    case 2:
         SDL_RenderCopy(renderer,texture_key,&src_rect,&dst_rect);
        break;
    }
}

