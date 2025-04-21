//======================
//
//エフェクト処理
//Author:ASUMA NISHIO
//
//======================
#include "effect.h"
#include "bullet.h"

//マクロ
#define MAX_EFFECT (4096)	//最大エフェクト数

//構造体
typedef struct
{
	D3DXVECTOR3 pos;			//位置
	D3DXCOLOR col;				//色
	D3DXVECTOR3 move;			//移動量
	D3DXVECTOR3 rot;			//z軸
	int nLife;					//弾の寿命
	int nType;					//タイプ
	bool bUse;					//使用しているかどうか
	float fRadius;				//半径
}Effect;

//グローバル
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;	//頂点バッファのポインタ
LPDIRECT3DTEXTURE9 g_pTextureEffect = NULL;			//テクスチャのポインタ
Effect g_aEffect[MAX_EFFECT];						//エフェクト情報

//======================
//初期化処理
//======================
void InitEffect(void)
{
	int nCntEffect;	//ローカル変数

	LPDIRECT3DDEVICE9 pDevice;	//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\effect000.jpg",
		&g_pTextureEffect);

	//弾情報の初期化
	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		g_aEffect[nCntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//座標
		g_aEffect[nCntEffect].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//移動量
		g_aEffect[nCntEffect].nLife = 100;									//体力
		g_aEffect[nCntEffect].bUse = false;									//使用してない状態にする
		g_aEffect[nCntEffect].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aEffect[nCntEffect].fRadius = 0.0f;								//半径
		g_aEffect[nCntEffect].nType = 0;									//エフェクトの種類
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_EFFECT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEffect,
		NULL);

	VERTEX_2D* pVtx;//頂点情報のポインタ

	//頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//1つ目の頂点情報
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//2つ目の頂点情報
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//3つ目の頂点情報
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//4つ目の頂点情報

		//rhwの設定(1.0fで固定)
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;	//頂点データのポインタを4つ分進める
	}
	//頂点バッファをアンロック
	g_pVtxBuffEffect->Unlock();

}
//==================
//終了処理
//==================
void UninitEffect(void)
{
	//テクスチャの破棄
	if (g_pTextureEffect != NULL)
	{
		g_pTextureEffect->Release();
		g_pTextureEffect = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffEffect != NULL)
	{
		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;
	}

}
//==================
//更新処理
//==================
void UpdateEffect(void)
{
	int nCntEffect;		//ローカル変数

	LPDIRECT3DDEVICE9 pDevice;	//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	VERTEX_2D* pVtx;//頂点情報のポインタ

	//頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{
 			g_aEffect[nCntEffect].pos += g_aEffect[nCntEffect].move;

			g_aEffect[nCntEffect].fRadius--;
			//頂点情報の設定
			pVtx[0].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x - g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].pos.y - g_aEffect[nCntEffect].fRadius, 0.0f);//1つ目の頂点情報
			pVtx[1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x + g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].pos.y - g_aEffect[nCntEffect].fRadius, 0.0f);//2つ目の頂点情報
			pVtx[2].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x - g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].pos.y + g_aEffect[nCntEffect].fRadius, 0.0f);//3つ目の頂点情報
			pVtx[3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x + g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].pos.y + g_aEffect[nCntEffect].fRadius, 0.0f);//4つ目の頂点情報

			

			if (g_aEffect[nCntEffect].fRadius <= 0.5f)
			{
				g_aEffect[nCntEffect].fRadius = 0.3f;
			}

			g_aEffect[nCntEffect].nLife--;

			if (g_aEffect[nCntEffect].nLife <= 0)
			{
				//寿命尽きた
				g_aEffect[nCntEffect].bUse = false;
			}

		}
		pVtx += 4;

	}

	//頂点バッファをアンロック
	g_pVtxBuffEffect->Unlock();

}
//==================
//描画処理
//==================
void DrawEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャ設定
	pDevice->SetTexture(0, g_pTextureEffect);

	//αブレンディングの加算合成
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{
			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEffect * 4, 2);//プレイヤーの始点(0番目～3番目)

		}
	}
	//αブレンディングを戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);

}
//==================
//設定処理
//==================
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, int nLife,int nType)
{
	int nCntEffect;	//ローカル変数

	VERTEX_2D* pVtx = {};	//頂点情報のポインタ

	//頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == false)
		{
			g_aEffect[nCntEffect].pos = pos;				//座標
			g_aEffect[nCntEffect].move = move;				//移動量
			g_aEffect[nCntEffect].nLife = nLife;			//体力
			g_aEffect[nCntEffect].nType = nType;			//種類
			g_aEffect[nCntEffect].bUse = true;				//使用状態にする
			g_aEffect[nCntEffect].col = col;				//色
			g_aEffect[nCntEffect].fRadius = fRadius;		//半径

			g_aEffect[nCntEffect].fRadius--;				//半径をデクリメント

			//頂点情報の設定
			pVtx[0].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x - g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].pos.y - g_aEffect[nCntEffect].fRadius, 0.0f);	//1つ目の頂点情報
			pVtx[1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x + g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].pos.y - g_aEffect[nCntEffect].fRadius, 0.0f);	//2つ目の頂点情報
			pVtx[2].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x - g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].pos.y + g_aEffect[nCntEffect].fRadius, 0.0f);	//3つ目の頂点情報
			pVtx[3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x + g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].pos.y + g_aEffect[nCntEffect].fRadius, 0.0f);	//4つ目の頂点情報

			//頂点カラーの設定
			pVtx[0].col = col;
			pVtx[1].col = col;
			pVtx[2].col = col;
			pVtx[3].col = col;

			break;

		}
		pVtx += 4;
	}


	//頂点バッファをアンロック
	g_pVtxBuffEffect->Unlock();
}