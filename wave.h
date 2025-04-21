//---------------------------
//
//wave処理(読み込み)
//Author:NISHIO ASUMA
//
//---------------------------

#ifndef _WAVE_H_
#define _WAVE_H_

#include "main.h"
#include "enemy.h"

//マクロ定義
#define WAVEPATH_BIN    ("WAVE\\wave000.bin")	//ウェーブ1のパス
#define WAVEPATH_BIN_1  ("WAVE\\wave001.bin")	//ウェーブ2のパス
#define WAVEPATH_BIN_2  ("WAVE\\wave002.bin")	//ウェーブ3のパス
#define WAVEPATH_BIN_3  ("WAVE\\wave003.bin")	//ウェーブ4のパス
#define WAVEPATH_BIN_4  ("WAVE\\wave004.bin")	//ウェーブ5のパス
#define WAVEPATH_BIN_5  ("WAVE\\wave005.bin")	//ウェーブ6のパス
#define WAVEPATH_BIN_6  ("WAVE\\wave006.bin")	//ウェーブ7のパス
#define WAVEPATH_BIN_7  ("WAVE\\wave007.bin")	//ウェーブ8のパス
#define WAVEPATH_BIN_8  ("WAVE\\wave008.bin")	//ウェーブ9のパス
#define WAVEPATH_BIN_9  ("WAVE\\wave009.bin")	//ウェーブ10のパス
#define WAVEPATH_BIN_10 ("WAVE\\wave010.bin")	//ウェーブ11のパス


//プロトタイプ宣言
void InitWave(void);			//ファイルの初期化
void LoadWave(void);			//ファイルを読み込む
int GetWave(void);				//ウェーブ情報の取得
bool GetFinish(void);			//終了判定の取得

#endif

//#define WAVEPATH ("WAVE\\wave.txt")	//ウェーブのパス
