//---------------------------
//
//パーティクル処理
//Author:NISHIO ASUMA
//
//---------------------------

#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "main.h"

//プロト
void InitParticle(void);						//パーティクルの初期化
void UninitParticle(void);						//パーティクルの終了
void UpdateParticle(void);						//パーティクルの更新
void DrawParticle(void);						//パーティクルの描画
void SetParticle(D3DXVECTOR3 pos,D3DXCOLOR col);//パーティクルの設定

#endif
