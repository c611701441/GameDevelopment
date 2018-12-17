/*****************************************************************
ファイル名	: client_win.c
機能		: クライアントのユーザーインターフェイス処理
*****************************************************************/
#include <stdio.h>
#include <unistd.h>
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include"common.h"
#include"client_func.h"


extern Character player[4];//player[0]~[2]は逃走者、player[3]は鬼です

static int CheckButtonNO(int x,int y,int num);
static void SetCharData2DataBlock(void *data,char charData,int *dataSize);
static void SetIntData2DataBlock(void *data,int intData,int *dataSize);
static void blockset(void);
static void  BlockDrow( int blockname , SDL_Rect dst_rect);

static SDL_Surface *gMainWindow,*str;

static SDL_Rect gButtonRect[MAX_CLIENTS+2];
SDL_Texture *texture,*texture_player,*texture_others1,*texture_others2,*texture_others3,*texture_others4;
SDL_Texture *texture_wall , *texture_key ,*texture_item_sp;

SDL_Surface *image,*image_player;

SDL_Surface *image_sight;
SDL_Surface *image_state;

SDL_Texture* textures[10];
SDL_Texture* texture_colon;
SDL_Texture* texture_sight;

SDL_Surface *texture_state;

// 画像描画処理
SDL_Surface* images[10];
SDL_Surface* image_colon;
SDL_Surface *image_wall , *image_key , *image_item_sp;

SDL_Renderer *renderer;
//extern�ؿ�

SDL_Rect charawin; //MapDrawで使用
Window window;

void colon();
void onedigit(int i);
void minute(int k);
void twodigit(int j);
void limitTime(int starttime);
void sight();
void CharaState(Character player[clientID]);

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
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); //生成したレンダラーに描画色として白を設定
	//SDL_RenderPresent(renderer);
        
        image = IMG_Load("testMap.png");
        texture = SDL_CreateTextureFromSurface(renderer,image);
        image_player = IMG_Load("test.png");
        image_wall = IMG_Load("wall.png");
        image_key = IMG_Load("key.png");
        image_item_sp = IMG_Load("item_sp.png");
        texture_player = SDL_CreateTextureFromSurface(renderer,image_player);
	texture_others1 = SDL_CreateTextureFromSurface(renderer,image_player);
        texture_others2 = SDL_CreateTextureFromSurface(renderer,image_player);
        texture_others3 = SDL_CreateTextureFromSurface(renderer,image_player);
        texture_others4 = SDL_CreateTextureFromSurface(renderer,image_player);

        // 画像を（サーフェイスに）読み込む
        images[0] = IMG_Load("number_digtal0.png");
        images[1] = IMG_Load("number_digtal1.png");
        images[2] = IMG_Load("number_digtal2.png");
        images[3] = IMG_Load("number_digtal3.png");
        images[4] = IMG_Load("number_digtal4.png");
        images[5] = IMG_Load("number_digtal5.png");
        images[6] = IMG_Load("number_digtal6.png");
        images[7] = IMG_Load("number_digtal7.png");
        images[8] = IMG_Load("number_digtal8.png");
        images[9] = IMG_Load("number_digtal9.png");
        image_colon = IMG_Load(":.png");
        image_sight = IMG_Load("kurayami.png");
        image_state = IMG_Load("fish_shark.png");
        
        textures[0] = SDL_CreateTextureFromSurface(renderer, images[0]); // 読み込んだ画像からテクスチャを作成
        textures[1] = SDL_CreateTextureFromSurface(renderer, images[1]); // 読み込んだ画像からテクスチャを作成
        textures[2] = SDL_CreateTextureFromSurface(renderer, images[2]); // 読み込んだ画像からテクスチャを作
        textures[3] = SDL_CreateTextureFromSurface(renderer, images[3]); // 読み込んだ画像からテクスチャを作成
        textures[4] = SDL_CreateTextureFromSurface(renderer, images[4]); // 読み込んだ画像からテクスチャを作成
        textures[5] = SDL_CreateTextureFromSurface(renderer, images[5]); // 読み込んだ画像からテクスチャを作成
        textures[6] = SDL_CreateTextureFromSurface(renderer, images[6]); // 読み込んだ画像からテクスチャを作成
        textures[7] = SDL_CreateTextureFromSurface(renderer, images[7]); // 読み込んだ画像からテクスチャを作成
        textures[8] = SDL_CreateTextureFromSurface(renderer, images[8]); // 読み込んだ画像からテクスチャを作成
        textures[9] = SDL_CreateTextureFromSurface(renderer, images[9]); // 読み込んだ画像からテクスチャを作成
        texture_colon = SDL_CreateTextureFromSurface(renderer, image_colon); // 読み込んだ画像からテクスチャを作
        texture_sight = SDL_CreateTextureFromSurface(renderer, image_sight); // 読み込んだ画像からテクスチャを作成
        texture_wall = SDL_CreateTextureFromSurface(renderer,image_wall);
        texture_key = SDL_CreateTextureFromSurface(renderer,image_key);
        texture_item_sp = SDL_CreateTextureFromSurface(renderer,image_item_sp);
        texture_state = SDL_CreateTextureFromSurface(renderer,image_state);

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
    SDL_FreeSurface(images[0]); // サーフェイス（画像）の解放
    SDL_FreeSurface(images[1]); // サーフェイス（画像）の解放
    SDL_FreeSurface(images[2]); // サーフェイス（画像）の解放
    SDL_FreeSurface(images[3]); // サーフェイス（画像）の解放
    SDL_FreeSurface(images[4]); // サーフェイス（画像）の解放
    SDL_FreeSurface(images[5]); // サーフェイス（画像）の解放
    SDL_FreeSurface(images[6]); // サーフェイス（画像）の解放
    SDL_FreeSurface(images[7]); // サーフェイス（画像）の解放
    SDL_FreeSurface(images[8]); // サーフェイス（画像）の解放
    SDL_FreeSurface(images[9]); // サーフェイス（画像）の解放
    SDL_FreeSurface(image_colon); // サーフェイス（画像）の解放
    SDL_FreeSurface(image_sight);
    SDL_FreeSurface(image_state);
    SDL_DestroyTexture(textures[0]);
    SDL_DestroyTexture(textures[1]);
    SDL_DestroyTexture(textures[2]);
    SDL_DestroyTexture(textures[3]);
    SDL_DestroyTexture(textures[4]);
    SDL_DestroyTexture(textures[5]);
    SDL_DestroyTexture(textures[6]);
    SDL_DestroyTexture(textures[7]);
    SDL_DestroyTexture(textures[8]);
    SDL_DestroyTexture(textures[9]);
    SDL_DestroyTexture(texture_colon);
    SDL_DestroyTexture(texture_sight);
    SDL_DestroyTexture(texture_state);
    SDL_DestroyRenderer(renderer); // レンダラーの破棄
    SDL_DestroyWindow(gMainWindow); // ウィンドウの破棄
    IMG_Quit(); // IMGライブラリの利用終了
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
void WindowEvent(int num, int starttime)
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
     //sight();
     PlayerDisp();
     // CharaState(Character player[clientID]);
    //MapDraw();
   
    MoveOthersPlayer(x1,y,angle1,sp1,id1);
    MoveOthersPlayer(x2,y2,angle2,sp2,id2);
    MoveOthersPlayer(x3,y3,angle3,sp3,id3);
    MoveOthersPlayer(x4,y4,angle4,sp4,id4);
    limitTime(starttime);
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
    SDL_Rect dst_rect_others1={other_x-player[clientID].rect.x+400,other_y-player[clientID].rect.y+250,100,100};
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
関数名	: colon
機能	: 制限時間表示に使う関数
引数	: void		*data		: 送信用データ
出力	: なし
*****************************************************************/
void colon(){
    SDL_Rect dst_rect  ={ 390, 100, 104, 152}; // 画像のコピー先の座標と領域（x, y, w, h）
    SDL_Rect src_rect = {0, 0, image_colon->w, image_colon->h}; // コピー元画像の領域（x, y, w, h）
    SDL_RenderCopy(renderer,  texture_colon, &src_rect , &dst_rect); // フレーム番号に対応する画像の一領域をウィンドウに貼り付ける
}


/*****************************************************************
関数名	: onedigit
機能	: 制限時間表示に使う関数
引数	: void		*data		: 送信用データ
出力	: なし
*****************************************************************/
void onedigit(int k)
{
    SDL_Rect dst_rect1    = {612, 100, 84, 132}; // 画像のコピー先の座標と領域（x, y, w, h）
    SDL_Rect src_rect1 = {0, 0, images[k]->w, images[k]->h}; // コピー元画像の領域（x, y, w, h）
    SDL_RenderCopy(renderer,  textures[k], &src_rect1 , &dst_rect1 ); // フレーム番号に対応する画像の一領域をウィンドウに貼り付ける
}


/*****************************************************************
関数名	: minute
機能	: 制限時間表示に使う関数
引数	: void		*data		: 送信用データ
出力	: なし
*****************************************************************/
void minute(int i)
{
    SDL_Rect dst_rect2   = {300, 100, 84, 132}; // 画像のコピー先の座標と領域（x, y, w, h）
    SDL_Rect src_rect2 = {0, 0, images[i]->w, images[i]->h}; // コピー元画像の領域（x, y, w, h）
    SDL_RenderCopy(renderer,  textures[i], &src_rect2 , &dst_rect2 ); // フレーム番号に対応する画像の一領域をウィンドウに貼り付ける
}

/*********************************
関数名	: blockset
機能	: 障害物を表示する
引数	: なし
出力	: なし
*****************************************************************/
void blockset(void)
{
    int i,j;
    int dx, dy;
    SDL_Rect blockpoint = { 0 , 0 , 100 ,100 };
    Digital(&dx, &dy);
    
    for( i = -3; i < 5 ; i++ )
    {
        for( j = -5 ; j < 7 ; j++)
        {
            if(dx + j >= 0 && dx + j <= 39 && dy + i  >= 0 && dy + i <= 27 )
            {
                if ( block[dx + j ][dy + i ] > 0) 
                {
                    blockpoint.x = -player[clientID].rect.x + (dx + 5 ) * 100 + ( j + 5 ) * 100 - 50;
                    blockpoint.y = -player[clientID].rect.y + dy * 100 + ( i + 3 ) * 100 + 350;
                    printf("***[%d][%d]%d,%d,%d , %d***\n",dx + j,dy + i , blockpoint.x,( j + 5 ) * 100 , -player[clientID].rect.x + (dx +  5) * 100 , blockpoint.y);
                    BlockDrow(block[dx + j ][dy + i ] , blockpoint);
                }
                
            }
        }
    }
}


/*****************************************************************
関数名	: twodigit
機能	: 制限時間表示に使う関数
引数	: void		*data		: 送信用データ
出力	: なし
*****************************************************************/
void twodigit(int j)
{
    SDL_Rect dst_rect3 = {490, 100, 84, 132}; // 画像のコピー先の座標と領域（x, y, w, h）
    SDL_Rect src_rect3 = {0, 0, images[j]->w, images[j]->h}; // コピー元画像の領域（x, y, w, h）
    SDL_RenderCopy(renderer, textures[j], &src_rect3, &dst_rect3); // フレーム番号に対応する画像の一領域をウィンドウに貼り付ける
}


/*****************************************************************
関数名	: limitTime
機能	: 制限時間を表示
引数	: void		*data		: 送信用データ
出力	: なし
*****************************************************************/
void limitTime(int starttime)
{
    int b = 10;
    int c = -1;
    int i, j, k;

    
    b = time(NULL);

    if((b-starttime) < 181)
        {
            i = (180-(b-starttime))/60;//分のとこ
            j = ((180-(b-starttime)) % 60)/10;//秒の2桁目
            k = (180-(b-starttime)) % 10;//秒の1桁目
            printf("%d", i);
            printf(":%d", j);
             printf("%d\n", k);
             
         }

    else if((b-starttime) >= 181)
    {
        i = 0;
        j =0;
        k = 0;
    }
    
         c = b;
         
         colon();
         minute(i);
         twodigit(j);
         onedigit(k);
         
     }


/*****************************************************************
関数名	: sight
機能	: キャラの視界を制限
引数	: void		*data		: 送信用データ
出力	: なし
*****************************************************************/
/*void sight(void)
{
    SDL_Rect dst_rect_sight = {0, 0, 1000, 700}; // 画像のコピー先の座標と領域（x, y, w, h）
    SDL_Rect src_rect_sight = {0, 0, image_sight->w, image_sight->h}; // コピー元画像の領域（x, y, w, h）
    SDL_RenderCopy(renderer,  texture_sight, &src_rect_sight , &dst_rect_sight ); // フレーム番号に対応する画像の一領域をウィンドウに貼り付ける
}
*/


/********************
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
    case 3:
        SDL_RenderCopy(renderer,texture_item_sp,&src_rect,&dst_rect);
        break;
    }
}


/*****************************************************************
関数名	: ImageState
機能	: キャラの状態表示
引数	: void		*data		: 送信用データ
出力	: なし
*****************************************************************/
void ImageState ()
{
    SDL_Rect dst_rect_state  ={0 , 0, 200, 115}; // 画像のコピー先の座標と領域（x, y, w, h）
    SDL_Rect src_rect_state = {100, 100, image_state->w, image_state->h}; // コピー元画像の領域（x, y, w, h）
    SDL_RenderCopy(renderer,  texture_state, &src_rect_state , &dst_rect_state); // フレーム番号に対応する画像の一領域をウィンドウに貼り付ける
}

/*****************************************************************
関数名	: CharaState
機能	: キャラの状態表示
引数	: void		*data		: 送信用データ
出力	: なし
*****************************************************************/
/*void CharaState(Character player[clientID])
{
    int i;
    if(player[i].state == 1)
    {
        //生きているときの画像表示
        ImageState();
    }
    
    else if (player[i].state == 0)
    {
        //死んでいるときの画像表示
        ImageState();
    }

}
*/
