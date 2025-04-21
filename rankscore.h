//==============================
//
//ランキングスコア処理
//Author:NISHIO ASUMA
//
//==============================

#ifndef _RANKSCORE_H_
#define _RANKSCORE_H_

#include "main.h"
#include "ranking.h"

//プロトタイプ宣言
void InitRankingScore(void);			//ランキングスコアの初期化
void UninitRankingScore(void);			//ランキングスコアの終了
void UpdateRankingScore(void);			//ランキングスコアの更新
void DrawRankingScore(void);			//ランキングスコアの描画
void SetRankingTexture(void);			//ランキングのテクスチャの処理
void SetRanking(int nScore);			//スコアを設定(スコア情報を渡す)
void ResetRanking(void);				//ランキング情報取得
void WriteScore(void);					//書き出し

#endif