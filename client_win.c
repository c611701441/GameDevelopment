/*****************************************************************
�ե�����̾	: client_win.c
��ǽ		: ���饤����ȤΥ桼�������󥿡��ե���������
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
�ؿ�̾	: InitWindows
��ǽ	: �ᥤ�󥦥���ɥ���ɽ���������Ԥ�
����	: int	clientID		: ���饤������ֹ�
		  int	num				: �����饤����ȿ�
����	: ���������Ǥ����Ȥ�0�����Ԥ����Ȥ�-1
*****************************************************************/
int InitWindows(int clientID,int num,char name[][MAX_NAME_SIZE])
{
	int i;
	SDL_Surface *image;

	char endButton[]="END.jpg";
	char *s,title[10];

    /* ����������å� */
    assert(0<num && num<=MAX_CLIENTS);
	
	/* SDL�ν���� */
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("failed to initialize SDL.\n");
		return -1;
	}
	
	/* �ᥤ��Υ�����ɥ���������� */
	gMainWindow = SDL_CreateWindow("GAME",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,1000,700,0);
	renderer = SDL_CreateRenderer(gMainWindow,-1,SDL_RENDERER_SOFTWARE);
	/* �طʤ���ˤ��� */
	SDL_RenderPresent(renderer);



	/* �ܥ���κ��� */
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
�ؿ�̾	: DestroyWindow
��ǽ	: SDL��λ����
����	: �ʤ�
����	: �ʤ�
*****************************************************************/
void DestroyWindow(void)
{
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer); // レンダラーの破棄
    SDL_DestroyWindow(gMainWindow); // ウィンドウの破棄
    SDL_Quit();
}

/*****************************************************************
�ؿ�̾	: WindowEvent
��ǽ	: �ᥤ�󥦥���ɥ����Ф��륤�٥�Ƚ�����Ԥ�
����	: int		num		: �����饤����ȿ�
����	: �ʤ�
*****************************************************************/
void WindowEvent(int num)
{
	SDL_Event event;
	SDL_MouseButtonEvent *mouse;
	int buttonNO;
	int dataSize;
	unsigned char data[MAX_DATA];
	

    /* ����������å� */
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
                    /* ��End�פȽ񤫤줿�ܥ��󤬲����줿 */
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
�ؿ�̾	: CheckButtonNO
��ǽ	: ����å����줿�ܥ�����ֹ���֤�
����	: int	   x		: �ޥ����β����줿 x ��ɸ
		  int	   y		: �ޥ����β����줿 y ��ɸ
		  char	   num		: �����饤����ȿ�
����	: �����줿�ܥ�����ֹ���֤�
		  �ܥ��󤬲�����Ƥ��ʤ�����-1���֤�
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
�ؿ�̾	: SetIntData2DataBlock
��ǽ	: int ���Υǡ����������ѥǡ����κǸ�˥��åȤ���
����	: void		*data		: �����ѥǡ���
		  int		intData		: ���åȤ���ǡ���
		  int		*dataSize	: �����ѥǡ����θ��ߤΥ�����
����	: �ʤ�
*****************************************************************/
static void SetIntData2DataBlock(void *data,int intData,int *dataSize)
{
    int tmp;
     /* ����������å� */
    assert(data!=NULL);
    assert(0<=(*dataSize));
     tmp = htonl(intData);
     /* int ���Υǡ����������ѥǡ����κǸ�˥��ԡ����� */
    memcpy(data + (*dataSize),&tmp,sizeof(int));
    /* �ǡ��������������䤹 */
    (*dataSize) += sizeof(int);
}
 /*****************************************************************
�ؿ�̾	: SetCharData2DataBlock
��ǽ	: char ���Υǡ����������ѥǡ����κǸ�˥��åȤ���
����	: void		*data		: �����ѥǡ���
		  int		intData		: ���åȤ���ǡ���
		  int		*dataSize	: �����ѥǡ����θ��ߤΥ�����
����	: �ʤ�
*****************************************************************/
static void SetCharData2DataBlock(void *data,char charData,int *dataSize)
{
    /* ����������å� */
    assert(data!=NULL);
    assert(0<=(*dataSize));
     /* char ���Υǡ����������ѥǡ����κǸ�˥��ԡ����� */
    *(char *)(data + (*dataSize)) = charData;
    /* �ǡ��������������䤹 */
    (*dataSize) += sizeof(char);
}


