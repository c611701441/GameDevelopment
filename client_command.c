/*****************************************************************
ファイル名	: client_command.c
機能		: クライアントのコマンド処理
*****************************************************************/

#include"common.h"
#include"client_func.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
static void SetIntData2DataBlock(void *data,int intData,int *dataSize);
static void SetCharData2DataBlock(void *data,char charData,int *dataSize);

static void RecvOthersPlayer(void);

static void Digital(int *dx, int *dy);
static int MakeMap(void);
    

Character player[4];//player[0]~[2]は逃走者、player[3]は鬼です

SDL_Surface *gMainWindow;

int id;
int x1,y,angle1,sp1,id1;
int x2,y2,angle2,sp2,id2;
int x3,y3,angle3,sp3,id3;
int x4,y4,angle4,sp4,id4;
int move_flag;
/*****************************************************************
関数名	: PlayMove
機能	: Wiiリモコンからの入力を受け取りプレイヤーの座標を変更する
引数	: なし
出力	: なし
*****************************************************************/
void PlayerMove(void)
{

    int W;//障害物に触れているとき１、
    
    // 以下に処理を記述していく
    if(wiimote.keys.left)//下方向
    {
        player[clientID].rect.y += player[clientID].sp;
        W = MakeMap();
        if(W){
            player[clientID].rect.y -= player[clientID].sp;
        }
        if( player[clientID].rect.y > 3050)
        {
            player[clientID].rect.y = 350;//調整あり
        }
        player[clientID].angle = 270;
        move_flag=1;
        // SendRectCommand();
    }
    else if(wiimote.keys.right)//上方向
    {
        player[clientID].rect.y -= player[clientID].sp;
        W = MakeMap();
        if(W){
            player[clientID].rect.y += player[clientID].sp;
        }
        if( player[clientID].rect.y < 350)
        {
            player[clientID].rect.y = 3050;//要調整
            
        }
        player[clientID].angle = 90;
        move_flag=1;
        // SendRectCommand();
    }
    else if(wiimote.keys.up)//左方向
    {
        player[clientID].rect.x -= player[clientID].sp;
        W = MakeMap();
        if(W){
            player[clientID].rect.x += player[clientID].sp;
        }
        if( player[clientID].rect.x < 500)
        {
            player[clientID].rect.x = 4400;//なんとなく
        }
        player[clientID].angle = 180;
        move_flag=1;
        // SendRectCommand();
    }
    else if(wiimote.keys.down)//右方向
    {
        player[clientID].rect.x += player[clientID].sp;
        W = MakeMap();
        if(W){
            player[clientID].rect.x -= player[clientID].sp;
        }
        if( player[clientID].rect.x > 4400)
        {
            player[clientID].rect.x = 500;//適当な値です
        }
        player[clientID].angle = 0;
        move_flag=1;
        // SendRectCommand();
    }
    else{
        move_flag=0;
    }
    // SendRectCommand();
   
}
/*****************************************************************
関数名	: MakeMap
機能	: マップ上の障害物の当たり判定を行う
引数	: なし		: コマンド
出力	: プログラム終了コマンドがおくられてきた時には0を返す．
		  それ以外は1を返す
*****************************************************************/
int MakeMap(void){
    int dx, dy;
    Digital(&dx, &dy);
    if(block[dx][dy] || block[dx + 1][dy] || block[dx][dy + 1] || block[dx + 1][dy + 1])//キャラの４頂点での当たり判定
    {
        return 1;
    }else{
        return 0;
    }
}

/*****************************************************************
関数名	: ExecuteCommand
機能	: サーバーから送られてきたコマンドを元に，
		  引き数を受信し，実行する
引数	: char	command		: コマンド
出力	: プログラム終了コマンドがおくられてきた時には0を返す．
		  それ以外は1を返す
*****************************************************************/
int ExecuteCommand(char command)
{
    int	endFlag = 1;
#ifndef NDEBUG
    printf("#####\n");
    printf("ExecuteCommand()\n");
    printf("command = %c\n",command);
#endif
    switch(command){
    case END_COMMAND:
        endFlag = 0;
        break;
    case RECT_COMMAND:
        RecvOthersPlayer();
        break;
    }
    return endFlag;
}

/**********************************
Wii_Event : wiiのボタン入力イベント

**********************************/
int Wii_Event(void)
{
    
// Wiiリモコンがオープン（接続状態）であればループ
    while (wiimote_is_open(&wiimote)) {
        
        // Wiiリモコンの状態を取得・更新する
        if (wiimote_update(&wiimote) < 0) {
            wiimote_disconnect(&wiimote);
            printf("disconnect\n");
            break;
        }
        
	// ***** Wiiのキー（ボタン）ごとに処理 *****
        // HOMEボタンが押された時
        if (wiimote.keys.home) {
            wiimote_speaker_free(&wiimote);	// Wiiリモコンのスピーカを解放
            wiimote_disconnect(&wiimote);	// Wiiリモコンとの接続を解除
            SendEndCommand();
        }
   
    }
    
    return 0;
}




/*****************************************************************
関数名	: SendEndCommand
機能	: プログラムの終了を知らせるために，
		  サーバーにデータを送る
引数	: なし
出力	: なし
*****************************************************************/
void SendEndCommand(void)
{
    unsigned char	data[MAX_DATA];
    int			dataSize;

#ifndef NDEBUG
    printf("#####\n");
    printf("SendEndCommand()\n");
#endif
    dataSize = 0;
    /* コマンドのセット */
    SetCharData2DataBlock(data,END_COMMAND,&dataSize);

    /* データの送信 */
    SendData(data,dataSize);
}

/****************************************************************
関数名　:
機能　　:プレイヤーの座標をサーバーに送る
引数　　:なし
出力　　:なし
 ****************************************************************/
void SendRectCommand(void)
{
    unsigned char data[MAX_DATA];
    int                     dataSize;

    #ifndef NDEBUG
    printf("SendRectCommand()\n");
    printf("player[%d].rect.x=%d\n",clientID,player[clientID].rect.x);
    #endif
    dataSize = 0;
    /*コマンドのセット*/
    SetCharData2DataBlock(data,RECT_COMMAND,&dataSize);
    /*プレイヤーのクライアントIDのセット*/
    SetIntData2DataBlock(data,clientID,&dataSize);
    /*プレイヤーの x 座標のセット*/
    SetIntData2DataBlock(data,player[clientID].rect.x,&dataSize);
    /*プレイヤーの y 座標のセット*/
    SetIntData2DataBlock(data,player[clientID].rect.y,&dataSize);
    /*プレイヤーの方向角度のセット*/
    SetIntData2DataBlock(data,player[clientID].angle,&dataSize);
    /*プレイヤーの速度のセット*/
    SetIntData2DataBlock(data,player[clientID].sp,&dataSize);
    
    SendData(data,dataSize);
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

/****************************************************
関数名    :RecvOthersPlayer
機能        :サーバから他プレイヤーの座標を受け取る
引数        :なし
出力        :なし
****************************************************/
static void RecvOthersPlayer(void)
{
    RecvIntData(&id);
    if(id==0){
        id1 = id;
        RecvIntData(&x1);
        RecvIntData(&y);
        RecvIntData(&angle1);
        RecvIntData(&sp1);
    }
    else  if(id==1){
        id2 = id;
        RecvIntData(&x2);
        RecvIntData(&y2);
        RecvIntData(&angle2);
        RecvIntData(&sp2);
        }
    else if(id==2){
        id3 = id;
        RecvIntData(&x3);
        RecvIntData(&y3);
        RecvIntData(&angle3);
        RecvIntData(&sp3);
    }
    else if(id==3){
        id4 = id;
        RecvIntData(&x4);
        RecvIntData(&y4);
        RecvIntData(&angle4);
        RecvIntData(&sp4);
    }
    
}

void MoveOthersPlayer(int x,int y,int angle,int sp,int id)
{    
 /*ここで移動方向を計算*/
   /*角度から移動用のベクトルを求めて描画座標に加算*/
    /*度数法を弧度法へ*/
    float Angle = angle*3.14/180;
    /*三角関数を使用し、位置を割り出す*/
    float add_x = cos(Angle)*sp;
    float add_y = sin(Angle)*sp;
    /*結果で出た位置を元座標に加算し、それを描画位置とする*/
    int pos_x = x+add_x;
    int pos_y = y+add_y;
    if(id != clientID){
        DrawOthersPlayer(pos_x,pos_y);
    }
}
/*****************************************************************
関数名	: ChageCenter
機能	: キャラクターの座標を左上から中心に変更
引数	: なし
出力	: なし
*****************************************************************/
void ChangeCenter(void)
{
    player[clientID].rect_center.x = player[clientID].rect.x - 50;
    player[clientID].rect_center.y = player[clientID].rect.y - 50;
}


/*****************************************************************
関数名	: Digital
機能	: キャラクターの座標を100で割った値にする
引数	: int *dx キャラクターのx座標/100 を代入
                  int *dy キャラクターのy座標/100 を代入
出力	: なし
*****************************************************************/
void Digital(int *dx, int *dy)
{
    *dx = ( player[clientID].rect.x - 500 )/100;
    *dy = ( player[clientID].rect.y - 350 )/100;
}


/*****************************************************************
関数名	: RAND
機能	: ランダムの数字を生成する
引数	: int	b　　　0~bまでの数字を生成する
出力	:なし
*****************************************************************/
int RAND(int b)
{
    int i;

    srand(time(NULL));/*rand関数に必要*/
    i = rand() % b;/*0~b-1の値をランダムに生成*/

    //連続で生成したとき同じ値をとることがあるためsleepなどを使うと良いかも
    
    return i;
}
