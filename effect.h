//======================
//
//エフェクト処理
//Author:ASUMA NISHIO
//
//======================

#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "main.h"

void InitEffect(void);				//エフェクトの初期化
void UninitEffect(void);			//エフェクトの終了
void UpdateEffect(void);			//エフェクトの更新
void DrawEffect(void);				//エフェクトの描画

void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col,float fRadius,int nLife,int nType);//エフェクトの設定処理

#endif
