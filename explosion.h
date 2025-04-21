//---------------------------
//
//爆発処理
//Author:NISHIO ASUMA
//
//---------------------------

#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

#include "main.h"


//プロトタイプ宣言
void InitExplosion(void);							//爆発の初期化
void UninitExplosion(void);							//爆発の終了
void UpdateExplosion(void);							//爆発の更新
void DrawExplosion(void);							//爆発の描画
void SetExplosion(D3DXVECTOR3 pos,D3DXCOLOR col);	//爆発の設定処理

#endif

