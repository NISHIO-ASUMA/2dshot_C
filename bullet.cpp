//---------------------------
//
//弾の処理
//Author:NISHIO ASUMA
//
//---------------------------

#include "bullet.h"
#include "player.h"
#include "main.h"
#include "explosion.h"
#include "enemy.h"
#include "item.h"
#include "effect.h"
#include "particle.h"
#include "sound.h"


//マクロ定義
#define BULLET_TEX (2)				//テクスチャ数
#define MAX_BULLET (128)			//弾の最大数
#define MAX_BULLETSIZE_X (25.0f)	//弾のサイズ
#define MAX_BULLETSIZE_Y (25.0f)	//弾のサイズ

//弾構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;			//位置
	D3DXVECTOR3 move;			//移動量
	D3DXVECTOR3 rot;			//z軸
	int nLife;					//弾の寿命
	bool bUse;					//使用しているかどうか
	BULLETTYPE type;			//弾の種類
	float fAngle;				//角度
	float fLength;				//対角線の長さ
	int nBulletType;			//弾の種類
}Bullet;

//グローバル変数
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;        //頂点バッファのポインタ
LPDIRECT3DTEXTURE9 g_pTextureBullet[BULLET_TEX] = {};   //テクスチャのポインタ
Bullet g_aBullet[MAX_BULLET];							//弾の情報

//=========================
//弾の初期化処理
//=========================
void InitBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//デバイスへのポインタ
	
	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\bullet002.png",
		&g_pTextureBullet[0]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\bullet001.png",
		&g_pTextureBullet[1]);


	//弾情報の初期化
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//座標
		g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//移動量
		g_aBullet[nCntBullet].nLife = 100;								//体力
		g_aBullet[nCntBullet].bUse = false;								//使用してない状態にする
		BULLETTYPE type = {};											//タイプ
		g_aBullet[nCntBullet].rot = D3DXVECTOR3(0.0f,0.0f,0.0f);		//角度
		g_aBullet[nCntBullet].nBulletType = 0;							//弾の種類

		//対角線の長さを算出
		g_aBullet[nCntBullet].fLength = sqrtf((MAX_BULLETSIZE_X * MAX_BULLETSIZE_X) + (MAX_BULLETSIZE_Y * MAX_BULLETSIZE_Y)) / 2.0f;

		//対角線の角度を算出
		g_aBullet[nCntBullet].fAngle = atan2f(MAX_BULLETSIZE_X, MAX_BULLETSIZE_Y);

	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BULLET,
		D3DUSAGE_WRITEONLY, 
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBullet,
		NULL);

	VERTEX_2D* pVtx;//頂点情報のポインタ

	//頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f ,0.0f);	//1つ目の頂点情報
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
	g_pVtxBuffBullet->Unlock();
}
//========================
//弾の終了処理
//========================
void UninitBullet(void)
{
	StopSound();	//サウンドの停止

	//テクスチャの破棄
	for (int nCnt = 0; nCnt < BULLET_TEX; nCnt++)
	{
		if (g_pTextureBullet[nCnt] != NULL)
		{
			g_pTextureBullet[nCnt]->Release();
			g_pTextureBullet[nCnt] = NULL;
		}

	}

	//頂点バッファの破棄
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}

}
//=============================
//弾の更新処理
//=============================
void UpdateBullet(void)
{
	VERTEX_2D* pVtx ;//頂点情報のポインタ

	//プレイヤーの取得
	Player* pPlayer = GetPlayer();

	//頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{

		if (g_aBullet[nCntBullet].bUse == true)
		{
			Enemy* pEnemy = GetEnemy();//敵の取得

			//弾が使用されている
			if (g_aBullet[nCntBullet].type == BULLETTYPE_PLAYER)
			{

				for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
				{

 					if (pEnemy->bUse == true)
					{

						//プレイヤーが使用されている
						if (g_aBullet[nCntBullet].pos.x <= pEnemy->pos.x + MAX_ENEMY_SIZE &&
							g_aBullet[nCntBullet].pos.y >= pEnemy->pos.y - MAX_ENEMY_SIZE &&
							g_aBullet[nCntBullet].pos.x >= pEnemy->pos.x - MAX_ENEMY_SIZE &&
							g_aBullet[nCntBullet].pos.y <= pEnemy->pos.y + MAX_ENEMY_SIZE)
						{
							SetParticle(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 0.0f, 0.4f, 1.0f));
							//敵のヒット処理
							if (GetMode() == MODE_GAME)
							{
								HitEnemy(nCntEnemy, 1);
							}
							g_aBullet[nCntBullet].bUse = false;
						}
					}

				}
			}
			else if (g_aBullet[nCntBullet].type == BULLETTYPE_ENEMY)
			{
				if (g_aBullet[nCntBullet].nBulletType == 1)
				{
					//SetEffect(g_aBullet[nCntBullet].pos, g_aBullet[nCntBullet].move, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 20.0f, 7, 0);

				}

				if (pPlayer->bDisp == true &&pPlayer->state == PLAYERSTATE_NORMAL)
				{
					//プレイヤーの使用
					if (
						g_aBullet[nCntBullet].pos.x <= pPlayer->pos.x + MAX_ENEMY_SIZE&& 
						g_aBullet[nCntBullet].pos.x >= pPlayer->pos.x - MAX_ENEMY_SIZE&&
						g_aBullet[nCntBullet].pos.y <= pPlayer->pos.y + MAX_ENEMY_SIZE&&
						g_aBullet[nCntBullet].pos.y >= pPlayer->pos.y - MAX_ENEMY_SIZE
						)											
					{

						//ダメージ判定
						HitPlayer(1);

						PlaySound(SOUND_LABEL_EXPLOSION);

						SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
						g_aBullet[nCntBullet].bUse = false;//未使用状態


					}
				}
			}
				

			//弾の位置更新処理
			g_aBullet[nCntBullet].pos.x += g_aBullet[nCntBullet].move.x;
			g_aBullet[nCntBullet].pos.y += g_aBullet[nCntBullet].move.y;

			//頂点座標の更新
			pVtx[0].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot.z + (-D3DX_PI + g_aBullet[nCntBullet].fAngle)) * g_aBullet[nCntBullet].fLength;
			pVtx[0].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot.z + (-D3DX_PI + g_aBullet[nCntBullet].fAngle)) * g_aBullet[nCntBullet].fLength;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot.z + (D3DX_PI - g_aBullet[nCntBullet].fAngle)) * g_aBullet[nCntBullet].fLength;
			pVtx[1].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot.z + (D3DX_PI - g_aBullet[nCntBullet].fAngle)) * g_aBullet[nCntBullet].fLength;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot.z + (0.0f - g_aBullet[nCntBullet].fAngle)) * g_aBullet[nCntBullet].fLength;
			pVtx[2].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot.z + (0.0f - g_aBullet[nCntBullet].fAngle)) * g_aBullet[nCntBullet].fLength;
			pVtx[2].pos.z = 0.0f;																									  
																																	  
			pVtx[3].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot.z + (0.0f + g_aBullet[nCntBullet].fAngle)) * g_aBullet[nCntBullet].fLength;
			pVtx[3].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot.z + (0.0f + g_aBullet[nCntBullet].fAngle)) * g_aBullet[nCntBullet].fLength;
			pVtx[3].pos.z = 0.0f;


			g_aBullet[nCntBullet].nLife--;//体力を減らす

			if (g_aBullet[nCntBullet].nLife <= 0)//寿命尽きる
			{
				g_aBullet[nCntBullet].bUse = false;//未使用状態
			}
		}
		pVtx += 4; //4つ分進める
	}
	//頂点バッファをアンロック
	g_pVtxBuffBullet->Unlock();

}
//===========================
//弾の描画処理
//===========================
void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		//弾が使用されている
		if (g_aBullet[nCntBullet].bUse == true)
		{
			if (g_aBullet[nCntBullet].type == BULLETTYPE_PLAYER)
			{
				//テクスチャの設定
				pDevice->SetTexture(0, g_pTextureBullet[0]);

			}
			else if (g_aBullet[nCntBullet].type == BULLETTYPE_ENEMY)
			{
				//テクスチャの設定
				pDevice->SetTexture(0, g_pTextureBullet[1]);
			}

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBullet * 4, 2);//プレイヤーの始点(0番目～3番目)

		}
	}
}
//============================
//弾の設定処理
//============================
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, float fWidth, float fHeight, int nLife, BULLETTYPE type)
{
	VERTEX_2D* pVtx;//頂点情報のポインタ

	//頂点バッファをロックし,頂点情報へのポインタを取得
 	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == false)
		{
			//対角線の長さを算出
			g_aBullet[nCntBullet].fLength = sqrtf((fWidth * fWidth) + (fHeight * fHeight)) / 2.0f;

			//対角線の角度を算出
			g_aBullet[nCntBullet].fAngle = atan2f(fWidth, fHeight);

			//弾が使われていない
			g_aBullet[nCntBullet].pos = pos;		//座標
			g_aBullet[nCntBullet].rot = rot;		//角度
			g_aBullet[nCntBullet].move = move;		//移動量
			g_aBullet[nCntBullet].nLife = nLife;	//体力
			g_aBullet[nCntBullet].type = type;		//種類
			g_aBullet[nCntBullet].bUse = true;		//使用状態にする
			
			//頂点座標の設定
			pVtx[0].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot.z + (-D3DX_PI + g_aBullet[nCntBullet].fAngle)) * g_aBullet[nCntBullet].fLength;
			pVtx[0].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot.z + (-D3DX_PI + g_aBullet[nCntBullet].fAngle)) * g_aBullet[nCntBullet].fLength;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot.z + (D3DX_PI - g_aBullet[nCntBullet].fAngle)) * g_aBullet[nCntBullet].fLength;
			pVtx[1].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot.z + (D3DX_PI - g_aBullet[nCntBullet].fAngle)) * g_aBullet[nCntBullet].fLength;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot.z + (0.0f - g_aBullet[nCntBullet].fAngle)) * g_aBullet[nCntBullet].fLength;
			pVtx[2].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot.z + (0.0f - g_aBullet[nCntBullet].fAngle)) * g_aBullet[nCntBullet].fLength;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot.z + (0.0f + g_aBullet[nCntBullet].fAngle)) * g_aBullet[nCntBullet].fLength;
			pVtx[3].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot.z + (0.0f + g_aBullet[nCntBullet].fAngle)) * g_aBullet[nCntBullet].fLength;
			pVtx[3].pos.z = 0.0f;

			//rhwの設定(1.0fで固定)
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			break;
		}

		pVtx += 4;	//4つ分移動
	}

	//頂点バッファをアンロック
	g_pVtxBuffBullet->Unlock();

}

