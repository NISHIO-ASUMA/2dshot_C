//========================================
//
//チュートリアル処理
//Author:ASUMA NISHIO
//
//========================================

#include "tutorial2.h"


LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTutorial2 = NULL;	//頂点バッファのポインタ(背景)
LPDIRECT3DTEXTURE9 g_pTextureTutorial2 = NULL;		//テクスチャのポインタ(背景)

D3DXVECTOR3 g_pos = D3DXVECTOR3(0.0f,0.0f,0.0f);	//座標

//========================
//初期化
//========================
void InitTutorial2(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	VERTEX_2D* pVtx;		//頂点情報のポインタ

	//初期化
	g_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\tutorialbg2.png",
		&g_pTextureTutorial2);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY, FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTutorial2,
		NULL);

	//頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffTutorial2->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標1の設定
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

	//アンロック
	g_pVtxBuffTutorial2->Unlock();

}
//========================
//終了
//========================
void UninitTutorial2(void)
{
	//テクスチャの破棄
	if (g_pTextureTutorial2 != NULL)
	{
		g_pTextureTutorial2->Release();
		g_pTextureTutorial2 = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffTutorial2 != NULL)
	{
		g_pVtxBuffTutorial2->Release();
		g_pVtxBuffTutorial2 = NULL;
	}

}
//========================
//更新
//========================
void UpdateTutorial2(void)
{
	
	if (GetIsNext() == true)
	{

		if (g_pos.x > -1280.0f)
		{
			g_pos.x -= 40.0f;

			VERTEX_2D* pVtx = NULL;		//頂点情報のポインタ

			//頂点バッファをロックし,頂点情報へのポインタを取得
			g_pVtxBuffTutorial2->Lock(0, 0, (void**)&pVtx, 0);

			//頂点座標1の設定(画面外)
			pVtx[0].pos = D3DXVECTOR3(g_pos.x + SCREEN_WIDTH,		0.0f,		   0.0f);		//1つ目の頂点情報
			pVtx[1].pos = D3DXVECTOR3(g_pos.x + (SCREEN_WIDTH * 2), 0.0f,		   0.0f);		//2つ目の頂点情報
			pVtx[2].pos = D3DXVECTOR3(g_pos.x + SCREEN_WIDTH,		SCREEN_HEIGHT, 0.0f);		//3つ目の頂点情報
			pVtx[3].pos = D3DXVECTOR3(g_pos.x + (SCREEN_WIDTH * 2), SCREEN_HEIGHT, 0.0f);		//4つ目の頂点情報

			//アンロック
			g_pVtxBuffTutorial2->Unlock();
		}

	}
}
//========================
//描画
//========================
void DrawTutorial2(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTutorial2, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureTutorial2);

	//チュートリアルポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

}
//========================
//設定処理
//========================
void SetTutorial2(void)
{
	VERTEX_2D* pVtx = NULL;		//頂点情報のポインタ

	//頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffTutorial2->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標1の設定(画面外)
	pVtx[0].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);			//1つ目の頂点情報
	pVtx[1].pos = D3DXVECTOR3((SCREEN_WIDTH * 2), 0.0f, 0.0f);		//2つ目の頂点情報
	pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH, 720.0f, 0.0f);			//3つ目の頂点情報
	pVtx[3].pos = D3DXVECTOR3((SCREEN_WIDTH * 2), 720.0f, 0.0f);	//4つ目の頂点情報

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
	g_pVtxBuffTutorial2->Unlock();

}
