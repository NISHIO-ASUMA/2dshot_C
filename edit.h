//---------------------------
//
//wave処理(書き出し)
//Author:NISHIO ASUMA
//
//---------------------------

#ifndef _EDIT_H_
#define _EDIT_H_

#include "main.h"
#include "enemy.h"

//プロトタイプ宣言
void SaveEdit(void);			//ファイル書き出し
void InitEdit(void);			//エディット画面の初期化
void UninitEdit(void);			//エディット画面の終了
void UpdateEdit(void);			//エディット画面の更新
void DrawEdit(void);			//エディット画面の描画

#endif