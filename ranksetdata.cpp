//==============================
//
//ランキング処理(順位)
//Author:NISHIO ASUMA
//
//==============================

#include "ranksetdata.h"
#include "input.h"
#include "fade.h"
#include <stdio.h>
#include "score.h"

//グローバル
LPDIRECT3DTEXTURE9 g_pTextureRank =NULL;					//テクスチャのポインタ(順位)
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRank = NULL;				//頂点バッファのポインタ(順位)

LPDIRECT3DTEXTURE9 g_pTextureRankBG = NULL;					//テクスチャのポインタ(順位)
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRankBG = NULL;			//頂点バッファのポインタ(順位)


//=============================
//初期化処理
//=============================
void InitRankingData(void)
{
	LPDIRECT3DDEVICE9 pDevice;	 //デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	VERTEX_2D* pVtx;			 //頂点情報のポインタ

	//テクスチャの読み込み(順位)
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\ranking.png",
		&g_pTextureRank);

	//テクスチャの読み込み(順位の背景)
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\ranking111.png",
		&g_pTextureRankBG);

	//===================
	//順位の背景
	//===================

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY, FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRankBG,
		NULL);

	//頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffRankBG->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//1つ目の頂点情報
	pVtx[1].pos = D3DXVECTOR3(1280.0f, 0.0f, 0.0f);		//2つ目の頂点情報
	pVtx[2].pos = D3DXVECTOR3(0.0f, 720.0f, 0.0f);		//3つ目の頂点情報
	pVtx[3].pos = D3DXVECTOR3(1280.0f, 720.0f, 0.0f);	//4つ目の頂点情報

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

	//アンロック
	g_pVtxBuffRankBG->Unlock();

	//======================
	//順位
	//======================

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
	D3DUSAGE_WRITEONLY, FVF_VERTEX_2D,
	D3DPOOL_MANAGED,
	&g_pVtxBuffRank,
	NULL);

	//頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffRank->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標1の設定
	pVtx[0].pos = D3DXVECTOR3(260.0f,150.0f,0.0f);		//1つ目の頂点情報
	pVtx[1].pos = D3DXVECTOR3(410.0f,150.0f,0.0f);		//2つ目の頂点情報
	pVtx[2].pos = D3DXVECTOR3(260.0f,530.0f,0.0f);		//3つ目の頂点情報
	pVtx[3].pos = D3DXVECTOR3(410.0f,530.0f,0.0f);		//4つ目の頂点情報

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

	//アンロック
	g_pVtxBuffRank->Unlock();

}
//=============================
//終了処理
//=============================
void UninitRankingData(void)
{
	//===============
	//順位
	//===============

	//テクスチャの破棄
	if (g_pTextureRank != NULL)
	{
		g_pTextureRank->Release();
		g_pTextureRank = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffRank != NULL)
	{
		g_pVtxBuffRank->Release();
		g_pVtxBuffRank = NULL;
	}

	//===============
	//順位の背景
	//===============

	//テクスチャの破棄
	if (g_pTextureRankBG != NULL)
	{
		g_pTextureRankBG->Release();
		g_pTextureRankBG = NULL;
	}

	//頂点バッファの破棄
	if (g_pTextureRankBG != NULL)
	{
		g_pTextureRankBG->Release();
		g_pTextureRankBG = NULL;
	}

}
//=============================
//更新処理
//=============================
void UpdateRankingData(void)
{
	//無し(ポリゴン出すくらい)
}
//=============================
//描画処理
//=============================
void DrawRankingData(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//===============
	//順位の背景
	//===============

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffRankBG, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureRankBG);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//===============
	//順位
	//===============

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffRank, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureRank);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

}
