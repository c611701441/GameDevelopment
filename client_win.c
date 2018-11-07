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


static SDL_Surface *gMainWindow,*str;

static SDL_Rect gButtonRect[MAX_CLIENTS+2];
	SDL_Texture *texture;

        SDL_Renderer *renderer;
//extern�ؿ�

int clientID;


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
	SDL_Surface *image;

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



	/* ボタンの作成 */
	for(i=0;i<num+2;i++){
		gButtonRect[i].x = 50+200*i;
		gButtonRect[i].y=100;
		gButtonRect[i].w=100;
		gButtonRect[i].h=100;
                
		 if(i==0){
			s=endButton;
		}
		
		image = IMG_Load(s);
                texture = SDL_CreateTextureFromSurface(renderer,image);
		SDL_RenderCopy(renderer,texture,NULL,&(gButtonRect[i]));
		SDL_FreeSurface(image);
	}
	SDL_RenderPresent(renderer);

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
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer); // レンダラーの破棄
    SDL_DestroyWindow(gMainWindow); // ウィンドウの破棄
    SDL_Quit();
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
        case SDL_MOUSEBUTTONUP:
            mouse = (SDL_MouseButtonEvent*)&event;
            if(mouse->button == SDL_BUTTON_LEFT){
                buttonNO = CheckButtonNO(mouse->x,mouse->y,num);
#ifndef NDEBUG
                printf("#####\n");
                printf("WindowEvent()\n");
                printf("Button %d is pressed\n",buttonNO);
#endif
                
                
                 if(buttonNO==0){
                    /* 「End」と書かれたボタンが押された */
                    SendEndCommand();
                }
            }
         
            break;
        }
    }
}



/*****
static
*****/
/*****************************************************************
関数名	: CheckButtonNO
機能	: クリックされたボタンの番号を返す
引数	: int	   x		: マウスの押された x 座標
		  int	   y		: マウスの押された y 座標
		  char	   num		: 全クライアント数
出力	: 押されたボタンの番号を返す
		  ボタンが押されていない時は-1を返す
*****************************************************************/
static int CheckButtonNO(int x,int y,int num)
{
	int i;

 	for(i=0;i<num+2;i++){
		if(gButtonRect[i].x < x &&
			gButtonRect[i].y < y &&
      		gButtonRect[i].x + gButtonRect[i].w > x &&
			gButtonRect[i].y + gButtonRect[i].h > y){
			return i;
		}
	}
 	return -1;
}

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


