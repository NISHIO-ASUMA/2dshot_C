//---------------------------
//
//タイトル処理
//Author:NISHIO ASUMA
//
//---------------------------

#ifndef  _TITLE_H_
#define  _TITLE_H_

#include "main.h"
#include "input.h"

//列挙型(タイトルの種類)
typedef enum
{
	TITLE_FIRST = 0,					//1つ目のタイトル
	TITLE_SECOND,						//2つ目のタイトル
	TITLE_MAX
}TITLE;

//列挙型(タイトルの状態)
typedef enum
{
	TITLE_NONE = 0,						//タイトル状態(通常状態)
	TITLE_FLASH,						//点滅状態
	STATE_MAX
}TITLESTATE;

//プロトタイプ宣言	
void InitTitle(void);					 //タイトルの初期化
void UninitTitle(void);					 //タイトルの終了
void UpdateTitle(void);					 //タイトルの更新
void DrawTitle(void);					 //タイトルの描画
void SetTitle(int nType,D3DXVECTOR3 pos);//タイトルの設定
void TitleFlash(int nType);				//タイトルの点滅

#endif
