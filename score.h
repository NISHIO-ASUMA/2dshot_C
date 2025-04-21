//---------------------------
//
//スコア処理
//Author:NISHIO ASUMA
//
//---------------------------

#ifndef  _SCORE_H_
#define  _SCORE_H_

#include "main.h"

//マクロ定義
#define MAX_NUM_SCORE (8)			//桁の最大数

//プロト
void InitScore(void);				//スコアの初期化
void UninitScore(void);				//スコアの終了
void UpdateScore(void);				//スコアの更新
void DrawScore(void);				//スコアの描画
void SetScore(int nScore);			//スコアの設定
void AddScore(int nValue);			//スコアの加算
int ScoreCounter(int nScore);		//スコアの0をカウント
int GetScore(void);					//スコアを取得
void SaveScore(void);				//スコアの書き出し

#endif // ! _SCORE_H_
