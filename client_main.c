/*****************************************************************
ファイル名	: client_main.c
機能		: クライアントのメインルーチン
*****************************************************************/

#include"common.h"
#include"client_func.h"

/*キャラクターのグローバル変数*/
extern Character player[4];//player[0]~[2]は逃走者、player[3]は鬼です
extern wiimote_t wiimote;//wiiリモコンを用いるための構造体を宣言

/*マップのプロックの変数定義*/
int block[41][29];

/*アイテムの座標の変数定義*/
SDL_Rect item_rect[4][3];


int clientID;
// Wiiリモコンを用いるための構造体を宣言（初期化）
wiimote_t wiimote = WIIMOTE_INIT;	// Wiiリモコンの状態格納用

static void SetChara( void );
static void SetPoint( void );
static void setitem(int itemnum , int num);

int clientID;
int phase;
int item_flag;
int key_flag;
int GOAL;
static void SetMapdata(void);

int main(int argc,char *argv[])
{
    int		num;
    char	name[MAX_CLIENTS][MAX_NAME_SIZE];
    int		endFlag=1;
    char	localHostName[]="localhost";
    char	*serverName;
    SDL_Thread *wii_thread;//スレッドを用いる
    int stop,start;
    int counter=0;
    phase = 0;
    item_flag = 0;
    key_flag = 0;
    GOAL = 0;//ゴールの体力(３) 
   // ***** Wiiリモコン処理 *****
    if (argc < 3) {	// Wiiリモコン識別情報がコマンド引数で与えられなければ
        printf("Designate the wiimote ID to the application.\n");
        exit(1);
    }

    // Wiiリモコンの接続（１つのみ）
    
    if (wiimote_connect(&wiimote, argv[2]) < 0) {	// コマンド引数に指定したWiiリモコン識別情報を渡して接続
        printf("unable to open wiimote: %s\n", wiimote_get_error());
        exit(1);
    }
    
    wiimote.led.one  = 1;	// WiiリモコンのLEDの一番左を点灯させる（接続を知らせるために）
    
    /* 引数チェック */
    if(argc == 1){
    	serverName = localHostName;
    }
    else if(argc == 3){
    	serverName = argv[1];
    }
    else{
		fprintf(stderr, "Usage: %s, Cannot find a Server Name.\n", argv[0]);
		return -1;
    }

    /* サーバーとの接続 */
    if(SetUpClient(serverName,&clientID,&num,name)==-1){
		fprintf(stderr,"setup failed : SetUpClient\n");
		return -1;
	}
    /* ウインドウの初期化 */
	if(InitWindows(clientID,num,name)==-1){
		fprintf(stderr,"setup failed : InitWindows\n");
		return -1;
	}
    /*wiiをスレッド化する*/
    wii_thread = SDL_CreateThread(Wii_Event,NULL,NULL);


  
        
    /*map_dataの読み込み*/
    SetMapdata();
    
    /*鍵のセット*/
    setitem( 2 , 3 );/*個数を変更*/
    
    /*アイテムセット*/
    setitem( 3 , 3 );

    /*キャラクターの初期設定*/
    SetChara();
    

    
    int starttime = time(NULL);//制限時間の始まりの時間
    
    /*メインイベントループ*/
    while(endFlag){
        if(phase == 0){
            MapDisp();
            title();
            endFlag = SendRecvManager();
            if(wiimote.keys.one)
                SendStartCommand();
            stop = SDL_GetTicks();
            if(stop-start<16)
            {
                SDL_Delay(16-(stop-start));
            }
            start = SDL_GetTicks();
            
        }
        else if(phase == 1){
            WindowEvent(num, starttime);
            ChangeCenter();
            if(player[clientID].state==1){
                PlayerMove();
            }
            SendRectCommand();
            if(counter >= 5 && state1 == 0 && state2 == 0 && state3 == 0){
                SendDeadCommand();
            }
            endFlag = SendRecvManager();
            stop = SDL_GetTicks();
            if(stop-start<16)
            {
                SDL_Delay(16-(stop-start));
            }
            start = SDL_GetTicks();
            counter++;
        }
    };

    /* 終了処理*/
	DestroyWindow();
	CloseSoc();
        SDL_WaitThread(wii_thread,NULL);

    return 0;
}

/********************************************
関数名　: SetChara
機能　　: キャラクターの初期設定
鬼のクライアントIDを3と仮定しました。
 ********************************************/
void SetChara(void)
{
    int i;
    
    for( i = 0 ; i < 3 ; i++ )
    {
        player[i].state = 1;/*1は生きている0は死んでいる*/
        player[i].hp = 1;
        player[i].at = 1;
        player[i].sp = 5;
        player[i].item = 0;/*アイテムを所持していないとき0*/
        player[i].key = 0;/*鍵を持っていないとき0*/
        player[i].r = 50;/*キャラクターの半径。当たり判定などに使用する*/
        player[i].angle = 270;
    }

    player[3].state = 1;
    player[3].hp = 100;
    player[3].at = 300;
    player[3].sp = 8;
    player[3].item = 0;
    player[3].key = 0;
    player[3].r = 50;
    player[3].angle = 270;

    SetPoint();/*自分の座標を設定*/
}

/********************************************
関数名　: SetPoint
機能　　: 自分のキャラクターの初期座標を設定
********************************************/
void SetPoint(void)
{
    int x , y;
    int w, h;

    if(clientID < 3){
        do
        {
            w = RAND(40 , clientID );
            h = RAND(28 , clientID );
            
        }while(block[w][h] != 0);
        
        x =  w * 100 + 500 ;/*適当に書きました。あとでランダムにしましょう。*/
        y = h * 100 + 350 ;
    
        player[clientID].rect.x = x;
        player[clientID].rect.y = y;
    
    }else{
        player[3].rect.x = 2000;/*鬼の初期位置。今は適当に書きました*/
        player[3].rect.y = 1000;
    }
     
}

/********************************************
関数名　: SetMapdata
機能　　: マップのデータをを設定
********************************************/
void SetMapdata(void)
{
    FILE *fp;/*ファイルを扱うための変数*/
    int i , j ;

    fp = fopen("map_data", "r" );/*ファイルを開く*/
/*map_dateをblockに入れる*/
    for( i = 0; i < 29 ; i++)
    {
        for( j = 0 ; j < 41 ;  j++)
        {
            fscanf( fp,"%d", &block[j][i] );
        }
    }

    fclose(fp);/*ファイルを閉じる*/
}


/*****************************************************************
関数名	: setitem
機能	: itemをmapdataにセットする
引数	: int	itemnum             mapdata上のセットしたいアイテムの番号
　　　　:int        num                       生成するアイテムの数
出力	: なし
*****************************************************************/
void setitem(int itemnum , int num)
{
    int w,h;
    int i;
     printf("a\n");

    for( i = 0 ; i < num ; i++ )
    {
        printf("b\n");
        do
        {
            w = RAND(40 ,i );
            h = RAND(28 , i );
        
        }while(block[w][h] != 0);

        block[w][h] = itemnum;

        item_rect[itemnum][i].x = w * 100;
        item_rect[itemnum][i].y = h * 100 ;
       
    }
    
}
