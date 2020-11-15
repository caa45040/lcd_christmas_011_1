#include <Arduino.h>

//*          *****  *****  
//*         *       *    * 
//*         *       *    *
//*         *       *    *
//*         *       *    *
//********   *****  *****

////#define swdclk PA14
////#define swdio  PA13

#define DW   digitalWrite

#define swdclk PA8
#define swdio  PA12
#define en     PA0
#define in7    PB7

void setup() {
  delay(3000);

    pinMode(swdclk,OUTPUT);
    pinMode(swdio,OUTPUT);
    pinMode(en,OUTPUT);
    pinMode(in7,INPUT_PULLUP);

    DW(en,0);
    DW(swdio,0);
    for(int i=0;i<10;i++){
        DW(swdclk,1);
        delay(15);
        DW(swdclk,0);
        delay(15);
    }
}

int v;
int rs;
void seg1()
{
    DW(swdio,(v>>7)&1);DW(swdclk,1);DW(swdclk,0); //7
    DW(swdio,(v>>6)&1);DW(swdclk,1);DW(swdclk,0); //6
    DW(swdio,(v>>5)&1);DW(swdclk,1);DW(swdclk,0); //5
    DW(swdio,(v>>4)&1);DW(swdclk,1);DW(swdclk,0); //4
    DW(swdio,(v>>3)&1);DW(swdclk,1);DW(swdclk,0); //3
    DW(swdio,(v>>2)&1);DW(swdclk,1);DW(swdclk,0); //2
    DW(swdio,(v>>1)&1);DW(swdclk,1);DW(swdclk,0); //1
    DW(swdio,(v>>0)&1);DW(swdclk,1);DW(swdclk,0); //0
    DW(swdio,rs);   //delay(500);
    DW(en,1);       delayMicroseconds(40);
    DW(en,0);       //delay(500);
}

int a;
int b;
int c;
int d;
//int e;
int f;

int bl;
int bh;

int s;

int cursor1 = 0;

void loop() {

    //ファンクションセット1
    rs=0;
    v=0x30;seg1();
    delay(5);   //delay(1000); 

    //ファンクションセット2
    rs=0;
    v=0x30;seg1();
    delay(1);   //delay(1000); 

    //ファンクションセット3
    rs=0;
    v=0x30;seg1();   //delay(1000); 

    //ファンクションセット 2ラインモード
    rs=0;
    v=0x38;seg1();   //delay(1000); 

    //表示オフ
    rs=0;
    v=0x08;seg1();   //delay(1000); 

    //画面クリア
    rs=0;
    v=0x01;seg1();
    delay(2);        //delay(1000); 

    //エントリーモードセット
    rs=0;
    v=0x06;seg1();    //delay(1000); 

    //表示オン
    rs=0;
    v=0x08+0x04;seg1();//delay(1000); 


    //@文字の表示
    rs=1;
    v='@';seg1();
    delay(1000); 

    //1ライン目にカーソルを移動
    rs=0;
    v=0x80+0x00;seg1();
    delay(1000); 

    cursor1 = 0;
    while(1){

        while( digitalRead(in7) == 1 ) {}
        delay(35);
        a=!(digitalRead(in7));
        delay(35);
        b=!(digitalRead(in7));
        delay(35);
        c=!(digitalRead(in7));
        delay(35);
        d=!(digitalRead(in7));
        delay(35);

        //e=!(digitalRead(in7)); // 5 bit
        delay(35);
        f=!(digitalRead(in7));   // 6 bit
        delay(35);

        s=f*16+d*8+c*4+b*2+a;

        if( s==31 ) {
            //画面クリア
            rs=0;
            v=0x01;seg1();
            delay(2);
            //delay(1000); 
            //カーソルのクリア
            cursor1 = 0;
        } else {
            //文字の表示
            v=s+'@';rs=1;seg1();cursor1++;
            //delay(300); 
        }

        if(cursor1 == 8){
            //2ライン目にカーソルを移動
           rs=0;
           v=0x80+0x40;seg1();
           //delay(1000); 
        } else if(cursor1 >= 16) {
            //画面クリア
            rs=0;
            v=0x01;seg1();
            delay(2);
            //delay(1000); 
            //カーソルのクリア
            cursor1 = 0;
        } //endif
    } //while
} //end loop
