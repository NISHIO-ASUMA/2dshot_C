//---------------------------
//
//パーティクル処理
//Author:NISHIO ASUMA
//
//---------------------------

#include "particle.h"
#include "effect.h"

//マクロ
#define MAX_PARTICLE (128)//最大数

//構造体
typedef struct
{
	D3DXVECTOR3 pos;		//座標
	int nLife;				//寿命
	bool bUse;				//使用判定
	D3DXCOLOR col;			//カラー
}Particle;

//グローバル
Particle g_aParticle[MAX_PARTICLE];		//パーティクル情報

//==========================
//パーティクルの初期化
//==========================
void InitParticle(void)
{
	//構造体の初期化
	for (int nCnt = 0; nCnt < MAX_PARTICLE; nCnt++)
	{
		g_aParticle[nCnt].bUse = false;							//未使用状態
		g_aParticle[nCnt].nLife = 10;							//体力
		g_aParticle[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//座標
		g_aParticle[nCnt].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f); //色

	}
}
//==========================
//パーティクルの終了
//==========================
void UninitParticle(void)
{
	//無し
}
//==========================
//パーティクルの更新
//==========================
void UpdateParticle(void)
{
	//ローカル変数
	int nCntParticle;
	D3DXVECTOR3 move;
	D3DXVECTOR3 pos;
	D3DXCOLOR col;

	for (nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse == true)
		{
			//パーティクルの生成
			for (int nCntApper = 0; nCntApper < 2; nCntApper++)
			{
				//位置の設定
				pos = g_aParticle[nCntParticle].pos;

				//移動量の設定
				float fAngel = (float)(rand() % 629 - 314) / 100.0f;
				float fLength = ((float)(rand() % 60) / 10.0f + 0.7f);

				move.x = sinf(fAngel) * fLength;
				move.y = cosf(fAngel) * fLength;
				move.z = 0.0f;

				//カラーの設定
				col = D3DXCOLOR(0.6f, 0.4f,1.0f, 0.5f);

				//半径の設定
				float fRadius = ((float)(rand() % 450) / 10.0f + 0.7f);

				//寿命の設定
				int nLife = ((float)(rand() % 2000) / 10);

				//エフェクトの設定
				SetEffect(pos, D3DXVECTOR3 (move.x,move.y,move.z), col, fRadius, nLife,0);

			}

			g_aParticle[nCntParticle].nLife--;

			if (g_aParticle[nCntParticle].nLife <= 0)
			{
				//未使用状態
				g_aParticle[nCntParticle].bUse = false;
			}
		}
	}
}
//==========================
//パーティクルの描画
//==========================
void DrawParticle(void)
{
	//無し
}
//==========================
//パーティクルの設定処理
//==========================
void SetParticle(D3DXVECTOR3 pos,D3DXCOLOR col)
{
	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse == false)
		{
			g_aParticle[nCntParticle].pos = pos;		//座標
			g_aParticle[nCntParticle].bUse = true;		//使用判定
			g_aParticle[nCntParticle].nLife = 15;		//体力
			g_aParticle[nCntParticle].col = col;		//色

			break;
		}
	}
}
