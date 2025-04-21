//===============================
//
//アイテム処理
//Author:ASUMA NISHIO
//
//================================

#include "item.h"
#include "player.h"
#include "enemy.h"
#include "explosion.h"
#include "score.h"
#include "sound.h"

//グローバル
LPDIRECT3DTEXTURE9 g_apTextureItem[ITEMTYPE_MAX] = {};		//テクスチャ座標
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffItem = NULL;				//頂点バッファのポインタ
ITEM  g_aItem[ITEMTYPE_MAX];								//アイテムの情報
ITEMTYPE g_Item;											//アイテムの列挙型
int nCntItem;												//アイテム数をカウント

//==============================
//アイテムの初期化処理
//==============================
void InitItem(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	VERTEX_2D* pVtx;				//頂点情報のポインタ

	for (int nCnt = 0; nCnt < ITEMTYPE_MAX; nCnt++)
	{
		//アイテムのテクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			ITEM_TEXTURE[nCnt],
			&g_apTextureItem[nCnt]);
	}

	for (int nItem = 0; nItem < ITEMTYPE_MAX; nItem++)
	{
		g_aItem[nItem].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//座標
		g_aItem[nItem].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//移動量
		g_aItem[nItem].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	//色
		g_aItem[nItem].nLife = 1;								//体力
		g_aItem[nItem].nType = 0;								//種類
		g_aItem[nItem].bUse = false;							//未使用状態
		g_aItem[nItem].nItemCounter = 0;						//アイテムカウンターの初期化
		g_aItem[nItem].bRight = false;							//フラグ
		g_aItem[nItem].state = ITEMSTATE_POP;					//初期状態
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * ITEMTYPE_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffItem,
		NULL);

	//頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	for (int nItem1 = 0; nItem1 < ITEMTYPE_MAX; nItem1++)
	{

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//rhwの設定(1.0fで固定)
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}
	//アンロック
	g_pVtxBuffItem->Unlock();

}
//========================
//アイテムの終了
//========================
void UninitItem(void)
{
	StopSound();

	int nItem;		//ローカル変数

	//テクスチャ座標の破棄
	for (nItem = 0; nItem < ITEMTYPE_MAX; nItem++)
	{
		if (g_apTextureItem[nItem] != NULL)
		{			   
			g_apTextureItem[nItem]->Release();
			g_apTextureItem[nItem] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffItem != NULL)
	{			  
		g_pVtxBuffItem->Release();
		g_pVtxBuffItem = NULL;
	}
}
//============================
//アイテムの更新処理
//============================
void UpdateItem(void)
{
	VERTEX_2D* pVtx;		//頂点情報のポインタ

	//頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < ITEMTYPE_MAX; nCnt++)
	{
		//プレイヤーの取得
		Player* pPlayer = GetPlayer();

		switch (g_aItem[nCnt].state)
		{
		case ITEMSTATE_POP:
			if (g_aItem[nCnt].pos.x <= (SCREEN_WIDTH - 50.0f) * 0.5f || g_aItem[nCnt].pos.x >= (SCREEN_WIDTH + 50.0f) * 0.5f)
			{
				//状態変化
				g_aItem[nCnt].state = ITEMSTATE_MOVE;
			}
			else
			{
				//真ん中に出現
				g_aItem[nCnt].state = ITEMSTATE_STOP;

			}
			break;

		case ITEMSTATE_MOVE:
			if (g_aItem[nCnt].pos.x >= (SCREEN_WIDTH - 50.0f) * 0.5f && g_aItem[nCnt].pos.x <= (SCREEN_WIDTH + 50.0f) * 0.5f)
			{
				//状態変化
				g_aItem[nCnt].state = ITEMSTATE_STOP;
			}
			break;

		case ITEMSTATE_STOP:
			//動きを止める
			g_aItem[nCnt].move.x = 0.0f;
			
			break;

		default:
			break;
		}

		if (g_aItem[nCnt].bUse == true)
		{
			//使用状態なら
			if (g_aItem[nCnt].pos.x >= pPlayer->pos.x - MAX_SIZE_X * 0.4f
				&& g_aItem[nCnt].pos.x <= pPlayer->pos.x + MAX_SIZE_X * 0.4f
				&& g_aItem[nCnt].pos.y >= pPlayer->pos.y - MAX_SIZE_Y * 0.4f
				&& g_aItem[nCnt].pos.y <= pPlayer->pos.y + MAX_SIZE_Y * 0.4f)
			{
				//当たり判定
				HitItem(1);
				PlaySound(SOUND_LABEL_ITEM);
			}

			if (g_aItem[nCnt].state == ITEMSTATE_MOVE)
			{
				//stateがmoveの時
				g_aItem[nCnt].move.y = 0.0f;

				if (g_aItem[nCnt].nType == ITEMTYPE_ONE)
				{
					//1種類目
					if (g_aItem[nCnt].pos.x <= SCREEN_WIDTH * 0.5f)
					{
						g_aItem[nCnt].move.x = 4.0f;
					}
					else if (g_aItem[nCnt].pos.x >= SCREEN_WIDTH * 0.5f)
					{
						g_aItem[nCnt].move.x = -4.0f;

					}
				}
				if (g_aItem[nCnt].nType == ITEMTYPE_TWO)
				{
					//2種類目
					if (g_aItem[nCnt].pos.x <= SCREEN_WIDTH * 0.5f)
					{
						g_aItem[nCnt].move.x = 4.0f;

					}
					else if (g_aItem[nCnt].pos.x >= SCREEN_WIDTH * 0.5f)
					{
						g_aItem[nCnt].move.x = -4.0f;
					}
				}
				if (g_aItem[nCnt].nType == ITEMTYPE_THREE)
				{
					//3種類目
					if (g_aItem[nCnt].pos.x <= SCREEN_WIDTH * 0.5f)
					{
						g_aItem[nCnt].move.x = 4.0f;

					}
					else if (g_aItem[nCnt].pos.x >= SCREEN_WIDTH * 0.5f)
					{
						g_aItem[nCnt].move.x = -4.0f;
					}
				}
				if (g_aItem[nCnt].nType == ITEMTYPE_FOUR)
				{
					//4種類目
					if (g_aItem[nCnt].pos.x <= SCREEN_WIDTH * 0.5f)
					{
						g_aItem[nCnt].move.x = 4.0f;

					}
					else if (g_aItem[nCnt].pos.x >= SCREEN_WIDTH * 0.5f)
					{
						g_aItem[nCnt].move.x = -4.0f;
					}
				}
				if (g_aItem[nCnt].nType == ITEMTYPE_FIVE)
				{
					//5種類目
					if (g_aItem[nCnt].pos.x <= SCREEN_WIDTH * 0.5f)
					{
						g_aItem[nCnt].move.x = 3.0f;

					}
					else if (g_aItem[nCnt].pos.x >= SCREEN_WIDTH * 0.5f)
					{
						g_aItem[nCnt].move.x = -3.0f;
					}
				}
				if (g_aItem[nCnt].nType == ITEMTYPE_SIX)
				{
					//6種類目
					if (g_aItem[nCnt].pos.x <= SCREEN_WIDTH * 0.5f)
					{
						g_aItem[nCnt].move.x = 4.0f;

					}
					else if (g_aItem[nCnt].pos.x >= SCREEN_WIDTH * 0.5f)
					{
						g_aItem[nCnt].move.x = -4.0f;
					}
				}

			}

			if (g_aItem[nCnt].bUse == true)
			{
				if (g_aItem[nCnt].pos.y >= SCREEN_HEIGHT)
				{
					g_aItem[nCnt].bUse = false;  //画面外にいったらfalse
				}
			}

			//位置の更新
			g_aItem[nCnt].pos += g_aItem[nCnt].move;

		}

		//頂点座標の更新
		pVtx[0].pos = D3DXVECTOR3(g_aItem[nCnt].pos.x - 25.0f, g_aItem[nCnt].pos.y - 25.0f, 0.0f);		//1つ目の頂点情報
		pVtx[1].pos = D3DXVECTOR3(g_aItem[nCnt].pos.x + 25.0f, g_aItem[nCnt].pos.y - 25.0f, 0.0f);		//2つ目の頂点情報
		pVtx[2].pos = D3DXVECTOR3(g_aItem[nCnt].pos.x - 25.0f, g_aItem[nCnt].pos.y + 25.0f, 0.0f);		//3つ目の頂点情報
		pVtx[3].pos = D3DXVECTOR3(g_aItem[nCnt].pos.x + 25.0f, g_aItem[nCnt].pos.y + 25.0f, 0.0f);		//4つ目の頂点情報

		pVtx += 4;

	}

	//アンロック
	g_pVtxBuffItem->Unlock();

}
//=============================
//アイテムの描画
//=============================
void DrawItem(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffItem, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nItem = 0; nItem < ITEMTYPE_MAX; nItem++)
	{
		if (g_aItem[nItem].bUse == true)
		{
			//使用されてる
			int nType = g_aItem[nItem].nType;

			//テクスチャの設定
			pDevice->SetTexture(0, g_apTextureItem[nType]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nItem * 4, 2);//始点

		}
	}
}
//===========================
//アイテムの設定
//===========================
void SetItem(D3DXVECTOR3 pos, int nType)
{
	int nItem;			//ローカル変数

	VERTEX_2D* pVtx;	//頂点情報のポインタ

	//頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	for (nItem = 0; nItem < ITEMTYPE_MAX; nItem++)
	{
		if (g_aItem[nItem].bUse == false)
		{
			//使用されてない
			g_aItem[nItem].pos = pos;								//座標
			g_aItem[nItem].nType = nType;							//種類
			g_aItem[nItem].bUse = true;								//使用状態
			g_aItem[nItem].nLife = 1;								//体力
			g_aItem[nItem].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);	//色
			g_aItem[nItem].state = ITEMSTATE_POP;					//状態

			//頂点情報の設定
			pVtx[0].pos = D3DXVECTOR3(g_aItem[nItem].pos.x - 25.0f, g_aItem[nItem].pos.y - 25.0f, 0.0f);		//1つ目の頂点情報
			pVtx[1].pos = D3DXVECTOR3(g_aItem[nItem].pos.x + 25.0f, g_aItem[nItem].pos.y - 25.0f, 0.0f);		//2つ目の頂点情報
			pVtx[2].pos = D3DXVECTOR3(g_aItem[nItem].pos.x - 25.0f, g_aItem[nItem].pos.y + 25.0f, 0.0f);		//3つ目の頂点情報
			pVtx[3].pos = D3DXVECTOR3(g_aItem[nItem].pos.x + 25.0f, g_aItem[nItem].pos.y + 25.0f, 0.0f);		//4つ目の頂点情報

			//頂点カラー
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			break;

		}

		pVtx += 4;
	}
	//アンロック
	g_pVtxBuffItem->Unlock();
}
//=======================
//アイテム側の当たり判定
//=======================
void HitItem(int nCntItem)
{
	int nItem = 0;			//ローカル変数

	VERTEX_2D* pVtx;		//頂点情報のポインタ

	//頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	if (g_aItem[nItem].nLife <= 0)
	{
		if (g_aItem[nItem].nType == ITEMTYPE_ONE)
		{
			//スコアの加算
			AddScore(1000);
		}
		else if (g_aItem[nItem].nType == ITEMTYPE_TWO)
		{
			//スコアの加算
			AddScore(2000);
		}
		else if (g_aItem[nItem].nType == ITEMTYPE_THREE)
		{
			//スコアの加算
			AddScore(4000);
		}
		else if (g_aItem[nItem].nType == ITEMTYPE_FOUR)
		{
			//スコアの加算
			AddScore(5000);
		}
		else if (g_aItem[nItem].nType == ITEMTYPE_FIVE)
		{
			//スコアの加算
			AddScore(10000);
		}
		else if (g_aItem[nItem].nType == ITEMTYPE_SIX)
		{
			//スコアの加算
			AddScore(20000);
		}

		g_aItem[nItem].bUse = false;		//未使用状態

	}
	else
	{
		g_aItem[nItem].nLife--;		//ライフを減らす

		pVtx += 4 * nCntItem;		//アイテム分ずらす

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);

	}
	//アンロック
	g_pVtxBuffItem->Unlock();

}
//=======================
//アイテムの判定を取得
//=======================
ITEM* GetItem(void)
{
	return &g_aItem[0];
}