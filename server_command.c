

#include"server_common.h"
#include"server_func.h"

static void SetIntData2DataBlock(void *data,int intData,int *dataSize);
static void SetCharData2DataBlock(void *data,char charData,int *dataSize);



/********************************************************************
関数名	: ExecuteCommand
機能	: クライアントから送られてきたコマンドを元に，
		  引き数を受信し，実行する
引数	: char	command		: コマンド
		  int	pos			: コマンドを送ったクライアント番号
出力	: プログラム終了コマンドが送られてきた時には0を返す．
		  それ以外は1を返す
*****************************************************************/
int ExecuteCommand(char command,int pos)
{
    unsigned char	data[MAX_DATA];
    int			dataSize,intData;
    int			endFlag = 1;
    int i;
    int dx,dy;
    int X,Y,ANGLE,SP,ID,STATE,ITEM,KEY,R;
        /* 引き数チェック */
    assert(0<=pos && pos<MAX_CLIENTS);
    
#ifndef NDEBUG
    printf("#####\n");
    printf("ExecuteCommand()\n");
    printf("Get command %c\n",command);
#endif
    switch(command){
    case END_COMMAND:
        dataSize = 0;
        /* コマンドのセット */
        SetCharData2DataBlock(data,command,&dataSize);
        /* 全ユーザーに送る */
        SendData(ALL_CLIENTS,data,dataSize);
        
        endFlag = 0;
        break;
    case RECT_COMMAND:
        RecvIntData(pos,&ID);
        RecvIntData(pos,&X);
        RecvIntData(pos,&Y);
        RecvIntData(pos,&ANGLE);
        RecvIntData(pos,&SP);
        RecvIntData(pos,&ITEM);
        RecvIntData(pos,&KEY);
        RecvIntData(pos,&R);
        RecvIntData(pos,&STATE);
        dataSize = 0;
        /* コマンドのセット */
        SetCharData2DataBlock(data,command,&dataSize);
        /*自分以外のクライアントID*/
        SetIntData2DataBlock(data,ID,&dataSize);
        /*自分以外のプレイヤーの x 座標*/
        SetIntData2DataBlock(data,X,&dataSize);
        /*自分以外のプレイヤーの y 座標*/
        SetIntData2DataBlock(data,Y,&dataSize);
        /*自分以外のプレイヤーの方向角度*/
        SetIntData2DataBlock(data,ANGLE,&dataSize);
        /*自分以外のプレイヤーの移動速度*/
        SetIntData2DataBlock(data,SP,&dataSize);
        SetIntData2DataBlock(data,ITEM,&dataSize);
        SetIntData2DataBlock(data,KEY,&dataSize);
        SetIntData2DataBlock(data,R,&dataSize);
        SetIntData2DataBlock(data,STATE,&dataSize);
        /*クライアントに送る*/
        SendData(ALL_CLIENTS,data,dataSize);
        break;
    case START_COMMAND:
        dataSize = 0;
        /* コマンドのセット */
        SetCharData2DataBlock(data,command,&dataSize);
        /* 全ユーザーに送る */
        SendData(ALL_CLIENTS,data,dataSize);
        break;
    case ITEM_COMMAND:
        RecvIntData(pos,&dx);
        RecvIntData(pos,&dy);
        dataSize = 0;
         /* コマンドのセット */
        SetCharData2DataBlock(data,command,&dataSize);
        /*自分以外のクライアントID*/
        SetIntData2DataBlock(data,dx,&dataSize);
        /*自分以外のプレイヤーの x 座標*/
        SetIntData2DataBlock(data,dy,&dataSize);
        /* 全ユーザーに送る */
        SendData(ALL_CLIENTS,data,dataSize);
        break;
    case CLEAR_COMMAND:
        dataSize = 0;
        /* コマンドのセット */
        SetCharData2DataBlock(data,command,&dataSize);
        /* 全ユーザーに送る */
        SendData(ALL_CLIENTS,data,dataSize);
        break;
    case OVER_COMMAND:
        dataSize = 0;
        /* コマンドのセット */
        SetCharData2DataBlock(data,command,&dataSize);
        /* 全ユーザーに送る */
        SendData(ALL_CLIENTS,data,dataSize);
        break;
    default:
        /* 未知のコマンドが送られてきた */
        fprintf(stderr,"0x%02x is not command!\n",command);
    }
    return endFlag;
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

    /* int 型のデータを送信用データの最後にコピーする */
    *(char *)(data + (*dataSize)) = charData;
    /* データサイズを増やす */
    (*dataSize) += sizeof(char);
}


