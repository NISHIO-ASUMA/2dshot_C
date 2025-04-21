//========================================
//
//チュートリアル処理
//Author:ASUMA NISHIO
//
//========================================

#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

#include "main.h"
#include "input.h"

//列挙型(種類)
typedef enum
{
	TUTORIAL_ONE = 0,		//1つ目のチュートリアル画像
	TUTORIAL_ENTER,			//2つ目のチュートリアル画像
	TUTORIAL_KEY_W,			//3つ目のチュートリアル画像
	TUTORIAL_KEY_A,			//4つ目のチュートリアル画像
	TUTORIAL_KEY_S,			//5つ目のチュートリアル画像
	TUTORIAL_KEY_D,			//6つ目のチュートリアル画像
	TUTORIAL_KEY_SPACE,		//7つ目のチュートリアル画像
	TUTORIAL_BUTTON_B,		//8つ目のチュートリアル画像
	TUTORIAL_BUTTON_UP,		//9つ目のチュートリアル画像
	TUTORIAL_BUTTON_DOWN,	//10個目のチュートリアル画像
	TUTORIAL_BUTTON_RIGHT,	//11個目のチュートリアル画像
	TUTORIAL_BUTTON_LEFT,	//12個目のチュートリアル画像
	TUTORIAL_KEY_P,         //Pキー
	TUTORIAL_BUTTON_ST,		//startボタン
	TUTORIAL_KEY_E,			//Eキー
	TUTORIAL_KEY_Q,			//Qキー
	TUTORIAL_BUTTON_RB,		//RBボタン
	TUTORIAL_BUTTON_LB,		//LBボタン
	TUTORIAL_LSTICK,		//Lスティック
	TUTORIAL_MAX
}TUTORIAL;

//列挙型(状態)
typedef enum
{
	TUTORIALSTATE_NONE = 0, //タイトル状態(通常状態)
	TUTORIALSTATE_FLASH,	//点滅状態
	TUTORIALSTATE_MAX
}TUTORIALSTATE;

static const char* TUTORIAL_TEXTURE[TUTORIAL_MAX] =   //チュートリアルのテクスチャの設定
{
	"data\\TEXTURE\\tutoriallife.png",		//1つ目
	"data\\TEXTURE\\tutorial.png",			//2つ目
	"data\\TEXTURE\\tutorial020.png",		//3つ目(w)
	"data\\TEXTURE\\tutorial030.png",		//4つ目(a)
	"data\\TEXTURE\\tutorial040.png",		//5つ目(s)
	"data\\TEXTURE\\tutorial050.png",		//6つ目(d)
	"data\\TEXTURE\\tutorial060.png",		//7つ目(スペースキー)
	"data\\TEXTURE\\tutorialbuttonB.png",	//8つ目(bボタン)
	"data\\TEXTURE\\tutorialup.png",		//9つ目(UPボタン)
	"data\\TEXTURE\\tutorialdown.png",		//10個目(downボタン)
	"data\\TEXTURE\\tutorialright.png",		//11個目(右ボタン)
	"data\\TEXTURE\\tutorialleft.png",		//12個目(左ボタン)
	"data\\TEXTURE\\tutorial070.png",		//Pキー
	"data\\TEXTURE\\tutorialbuttonS.png",	//statrボタン
	"data\\TEXTURE\\tutorial080.png",		//15つ目(e)
	"data\\TEXTURE\\tutorial090.png",		//16つ目(q)
	"data\\TEXTURE\\tutorial100.png",		//17つ目(RB)
	"data\\TEXTURE\\tutorial110.png",		//18つ目(LB)
	"data\\TEXTURE\\tutorialbuttonL.png"	//19つ目(Lスティック)
};

//プロトタイプ宣言
void InitTutorial(void);						//チュートリアルの初期化
void UninitTutorial(void);						//チュートリアルの終了
void UpdateTutorial(void);						//チュートリアルの更新
void DrawTutorial(void);						//チュートリアルの描画
void SetTutorial(int nType, D3DXVECTOR3 pos);   //チュートリアルの設定
void TutorialFlash(int nType);					//チュートリアルの点滅
bool GetIsNext(void);							//チュートリアルの次のページ
int GetTutorialCountMove(void);					//カウンターを取得

#endif