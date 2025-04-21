//---------------------------
//
//爆発処理
//Author:NISHIO ASUMA
//
//---------------------------

#include "explosion.h"
#include "player.h"
#include "main.h"

//マクロ定義
#define MAX_EXPLOSION (128)			//爆発の最大数
#define MAX_EXPLOSIONSIZE_X (30)	//爆発のサイズ
#define MAX_EXPLOSIONSIZE_Y (30)	//爆発のサイズ

//爆発構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;			//位置
	D3DXCOLOR col;				//色
	int nCounterAnimExplosion;	//アニメーションカウンター
	int nPatternAnimExplosion;	//アニメーションパターン
	bool bUse;					//使用しているか
}Explosion;

//グローバル変数
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffExplosion = NULL;	//頂点バッファのポインタ
LPDIRECT3DTEXTURE9 g_pTextureExplosion = NULL;		//テクスチャのポインタ
Explosion g_aExplosion[MAX_EXPLOSION];				//爆発の情報

//=========================
//爆発の初期化処理
//=========================
void InitExplosion(void)
{
	int nCntExplosion;//ローカル変数

	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ
		
	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\explosion001.png",
		&g_pTextureExplosion);

	//爆発情報の初期化
	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		g_aExplosion[nCntExplosion].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//座標
		g_aExplosion[nCntExplosion].col = D3DXCOLOR	 (1.0f,1.0f,1.0f,1.0f);	//色
		g_aExplosion[nCntExplosion].bUse = false;							//使用してない状態にする
		g_aExplosion[nCntExplosion].nCounterAnimExplosion = 0;				//カウンターの初期化
		g_aExplosion[nCntExplosion].nPatternAnimExplosion = 0;				//パターンの初期化
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_EXPLOSION,
		D3DUSAGE_WRITEONLY, 
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffExplosion,
		NULL);

	VERTEX_2D* pVtx;	//頂点情報のポインタ

	//頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x,		 g_aExplosion[nCntExplosion].pos.y - 50.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x + 50.0f, g_aExplosion[nCntExplosion].pos.y - 50.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x,		 g_aExplosion[nCntExplosion].pos.y + 50.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x + 50.0f, g_aExplosion[nCntExplosion].pos.y + 50.0f, 0.0f);

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
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		pVtx += 4;	//頂点データのポインタを4つ分進める
	}

	//頂点バッファをアンロック
	g_pVtxBuffExplosion->Unlock();
}
//========================
//爆発の終了処理
//========================
void UninitExplosion(void)
{
	//テクスチャの破棄
	if (g_pTextureExplosion != NULL)
	{
		g_pTextureExplosion->Release();
		g_pTextureExplosion = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffExplosion != NULL)
	{
		g_pVtxBuffExplosion->Release();
		g_pVtxBuffExplosion = NULL;
	}

}
//=============================
//爆発の更新処理
//=============================
void UpdateExplosion(void)
{
	int nCntExplosion;				//ローカル変数

	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	VERTEX_2D* pVtx ;//頂点情報のポインタ

	//頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		g_aExplosion[nCntExplosion].nCounterAnimExplosion++;

		if (g_aExplosion[nCntExplosion].bUse == true)
		{

				if (g_aExplosion[nCntExplosion].nCounterAnimExplosion >= 4)
				{
					g_aExplosion[nCntExplosion].nCounterAnimExplosion = NULL;		//カウンターを初期値に戻す

					g_aExplosion[nCntExplosion].nPatternAnimExplosion++;			//パターンナンバーを更新

					//頂点座標の更新
					pVtx[0].tex = D3DXVECTOR2(0.0f	 + g_aExplosion[nCntExplosion].nPatternAnimExplosion * 0.125f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(0.125f + g_aExplosion[nCntExplosion].nPatternAnimExplosion * 0.125f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(0.0f	 + g_aExplosion[nCntExplosion].nPatternAnimExplosion * 0.125f, SPLIT_V);
					pVtx[3].tex = D3DXVECTOR2(0.125f + g_aExplosion[nCntExplosion].nPatternAnimExplosion * 0.125f, SPLIT_V);

				}
				if (g_aExplosion[nCntExplosion].nPatternAnimExplosion > MAX_ANIMATION_PATTERN)
				{
					g_aExplosion[nCntExplosion].nPatternAnimExplosion = 0;			//パターンナンバーを初期値に戻す
					g_aExplosion[nCntExplosion].bUse = false;
				}
		}

		pVtx += 4;
	}
	//頂点バッファをアンロック
	g_pVtxBuffExplosion->Unlock();

}
//===========================
//爆発の描画処理
//===========================
void DrawExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffExplosion, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		//爆発が使用されている
		if (g_aExplosion[nCntExplosion].bUse == true)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureExplosion);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntExplosion * 4, 2);//プレイヤーの始点(0番目～3番目)

		}
	}
}
//============================
//爆発の設定処理
//============================
void SetExplosion(D3DXVECTOR3 pos, D3DXCOLOR col)
{
	VERTEX_2D* pVtx;	//頂点情報のポインタ

	//頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse == false)
		{
			//爆発が使われていない
			g_aExplosion[nCntExplosion].pos = pos;		//座標
			g_aExplosion[nCntExplosion].col = col;		//カラー
			g_aExplosion[nCntExplosion].bUse = true;	//使用してない状態にする

			//頂点座標
			pVtx[0].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x - 50.0f,g_aExplosion[nCntExplosion].pos.y - 50.0f,0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x + 50.0f,g_aExplosion[nCntExplosion].pos.y - 50.0f,0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x - 50.0f,g_aExplosion[nCntExplosion].pos.y + 50.0f,0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x + 50.0f,g_aExplosion[nCntExplosion].pos.y + 50.0f,0.0f);
																		 		
			//rhwの設定(1.0fで固定)
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			break;
		}

		pVtx += 4;		//4つ分移動
	}
	//頂点バッファをアンロック
	g_pVtxBuffExplosion->Unlock();

}