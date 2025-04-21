//---------------------------
//
//ポーズ処理
//Author:NISHIO ASUMA
//
//---------------------------

#ifndef _PAUSE_H_
#define _PAUSE_H_

#include "main.h"

#define MAX_PAUSE_TEX (4)  //ポーズ画面のテクスチャ

//ポーズメニュー
typedef enum
{
	PAUSE_MENU_CONTINUE = 0,//ゲームに戻る
	PAUSE_MENU_RETRY,       //ゲームをやり直す
	PAUSE_MENU_QUIT,        //タイトルに戻る
	PAUSE_MENU_RANKING,     //ランキングに行く
	PAUSE_MENU_MAX
}PAUSE_MENU;


//プロトタイプ宣言
void InitPause(void);		//ポーズ画面の初期化
void UninitPause(void);		//ポーズ画面の終了
void UpdatePause(void);		//ポーズ画面の更新
void DrawPause(void);		//ポーズ画面の描画

#endif