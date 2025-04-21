//---------------------------
//
//敵の処理
//Author:NISHIO ASUMA
//
//---------------------------

#include "enemy.h"
#include "bullet.h"
#include "explosion.h"
#include "player.h"
#include "main.h"
#include "score.h"
#include "item.h"
#include "particle.h"
#include "effect.h"
#include "sound.h"

//グローバル
LPDIRECT3DTEXTURE9 g_apTextureEnemy[ENEMYTYPE_MAX] = {};	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEnemy = NULL;			//頂点バッファのポインタ
Enemy  g_aEnemy[MAX_ENEMY];								//敵の情報
ENEMYTYPE g_pEnemy;										//敵のタイプの列挙型情報
int g_nNumEnemy;										//カウント
int g_nEnemyTimer;										//タイマー


//=========================
//敵の初期化処理
//=========================
void InitEnemy(void)
{
	int nCntEnemy;		//ローカル変数

	LPDIRECT3DDEVICE9 pDevice;	//デバイスへのポインタ

	//グローバル変数の初期化
	g_nNumEnemy = 0;
	g_nEnemyTimer = 0;

	//デバイスの取得
	pDevice = GetDevice();

	VERTEX_2D* pVtx;//頂点情報のポインタ

	for (int nCnt = 0; nCnt < ENEMYTYPE_MAX; nCnt++)
	{
		//敵のテクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			ENEMY_TEXTURE[nCnt],
			&g_apTextureEnemy[nCnt]);
	}

	//構造体の初期化
	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);		//座標
		g_aEnemy[nCntEnemy].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//角度
		g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//移動量
		g_aEnemy[nCntEnemy].nCounterState = 0;						//状態カウンター
		g_aEnemy[nCntEnemy].nLife = 0;								//体力
		g_aEnemy[nCntEnemy].state = {};								//状態
		g_aEnemy[nCntEnemy].fAngle = 0.0f;							//角度
		g_aEnemy[nCntEnemy].fLength = 0.0f;							//対角線
		g_aEnemy[nCntEnemy].nType = 0;								//種類
		g_aEnemy[nCntEnemy].bUse = false;							//使用しているかどうか
		g_aEnemy[nCntEnemy].nBulletCounter = 0;						//敵の弾の数の初期化
		g_aEnemy[nCntEnemy].bRight = false;							//敵の移動判定
		g_aEnemy[nCntEnemy].bTop = false;							//敵の移動判定
		g_aEnemy[nCntEnemy].Threestate = {};						//敵の3種類の移動情報
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_ENEMY,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEnemy,
		NULL);

	//頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
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
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}
	//アンロック
	g_pVtxBuffEnemy->Unlock();

}
//========================
//敵の終了処理
//========================
void UninitEnemy(void)
{
	StopSound();

	int nCntEnemy;		//ローカル変数

	//テクスチャの破棄
	for (nCntEnemy = 0; nCntEnemy < ENEMYTYPE_MAX; nCntEnemy++)
	{
		if (g_apTextureEnemy[nCntEnemy] != NULL)
		{
			g_apTextureEnemy[nCntEnemy]->Release();
			g_apTextureEnemy[nCntEnemy] = NULL;
		}
	}

	for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++)
	{
		if (g_aEnemy[nCnt].bUse == true)
		{
			continue;
		}

		g_aEnemy[nCnt].bUse = false;							//使用しているかどうか

	}

	//頂点バッファの破棄
	if (g_pVtxBuffEnemy != NULL)
	{
		g_pVtxBuffEnemy->Release();
		g_pVtxBuffEnemy = NULL;
	}

}
//=============================
//敵の更新処理
//=============================
void UpdateEnemy(void)
{
	int nCntEn = 0;		//ローカル変数

	VERTEX_2D* pVtx = {};	//頂点情報のポインタ

	//プレイヤーの取得
	Player* pPlayer = GetPlayer();

	//頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEn = 0; nCntEn < MAX_ENEMY; nCntEn++)
	{
		if (g_aEnemy[nCntEn].bUse == true)
		{
			//敵が使用されていたら
			switch (g_aEnemy[nCntEn].state)
			{
			case ENEMYSTATE_NORMAL:

				g_aEnemy[nCntEn].nBulletCounter++;		//カウンターを加算

				//プレイヤーの使用
				if (
					g_aEnemy[nCntEn].pos.x <= pPlayer->pos.x + MAX_ENEMY_SIZE &&
					g_aEnemy[nCntEn].pos.x >= pPlayer->pos.x - MAX_ENEMY_SIZE &&
					g_aEnemy[nCntEn].pos.y <= pPlayer->pos.y + MAX_ENEMY_SIZE &&
					g_aEnemy[nCntEn].pos.y >= pPlayer->pos.y - MAX_ENEMY_SIZE &&
					pPlayer->state == PLAYERSTATE_NORMAL)
				{
					PlaySound(SOUND_LABEL_HIT);

					HitPlayer(1);
					SetParticle(g_aEnemy[nCntEn].pos,D3DXCOLOR (1.0f,1.0f,1.0f,1.0f));
				}
	//===========================
	//1種類目の敵が使われている時
	//===========================
				else if (g_aEnemy[nCntEn].nType == ENEMYTYPE_OWN)
				{
					g_aEnemy[nCntEn].move.y = 0.0f;

					if (g_aEnemy[nCntEn].bRight == true)
					{
						g_aEnemy[nCntEn].move.x = 5.0f;

						if (g_aEnemy[nCntEn].pos.x >= SCREEN_WIDTH - MAX_ENEMY_SIZE)
						{
							g_aEnemy[nCntEn].bRight = false;
						}
					}
					else
					{
						g_aEnemy[nCntEn].move.x = -5.0f;

						if (g_aEnemy[nCntEn].pos.x <= MAX_ENEMY_SIZE)
						{
							g_aEnemy[nCntEn].bRight = true;
						}

					}

					//敵の弾の設定
					if (g_aEnemy[nCntEn].nBulletCounter >= MAX_ANIMATION_TIME + 70)		//一定時間が経過した
					{
						if (g_aEnemy[nCntEn].pos.y <= SCREEN_HEIGHT * 0.5f)
						{
							//posが上半分
							SetBullet(g_aEnemy[nCntEn].pos, D3DXVECTOR3(-sinf(g_aEnemy[nCntEn].rot.z + D3DX_PI) * 10.0f, -cosf(g_aEnemy[nCntEn].rot.z - D3DX_PI) * 10.0f, 0.0f), g_aEnemy[nCntEn].rot, 50.0f, 50.0f, 50, BULLETTYPE_ENEMY);

						}
						else if (g_aEnemy[nCntEn].pos.y >= SCREEN_HEIGHT * 0.5f)
						{
							//posが下半分
							SetBullet(g_aEnemy[nCntEn].pos, D3DXVECTOR3(sinf(g_aEnemy[nCntEn].rot.z + D3DX_PI) * 10.0f, cosf(g_aEnemy[nCntEn].rot.z + D3DX_PI) * 10.0f, 0.0f), g_aEnemy[nCntEn].rot, 50.0f, 50.0f, 50, BULLETTYPE_ENEMY);


						}
						g_aEnemy[nCntEn].nBulletCounter = 0;		//カウンターをリセット
					}

				}

	//===========================
	//2種類目の敵が使われている時
	//===========================
				else if (g_aEnemy[nCntEn].nType == ENEMYTYPE_TWO)
				{
					g_aEnemy[nCntEn].move.x = 0.0f;

					if (g_aEnemy[nCntEn].bTop == true)
					{
						g_aEnemy[nCntEn].move.y = -3.0f;

						if (g_aEnemy[nCntEn].pos.y <= 80.0f)
						{
							g_aEnemy[nCntEn].bTop = false;
						}
					}
					else
					{
						g_aEnemy[nCntEn].move.y = 3.0f;

						if (g_aEnemy[nCntEn].pos.y >= 650.0f)
						{
							g_aEnemy[nCntEn].bTop = true;
						}

					}

					//敵の弾の設定
					if (g_aEnemy[nCntEn].pos.x >= SCREEN_WIDTH * 0.5f)
					{
						if (g_aEnemy[nCntEn].nBulletCounter >= 50)		//一定時間が経過した
						{
							SetBullet(g_aEnemy[nCntEn].pos, D3DXVECTOR3(-sinf(g_aEnemy[nCntEn].rot.z + D3DX_PI * 0.5f) * 10.0f, -cosf(g_aEnemy[nCntEn].rot.z - D3DX_PI * 0.5f) * 10.0f, 0.0f), g_aEnemy[nCntEn].rot, 50.0f, 50.0f, 65, BULLETTYPE_ENEMY);
							g_aEnemy[nCntEn].nBulletCounter = 0;		//カウンターをリセット
						}

					}
					else
					{
						if (g_aEnemy[nCntEn].nBulletCounter >= 50)		//一定時間が経過した
						{

							SetBullet(g_aEnemy[nCntEn].pos, D3DXVECTOR3(-sinf(g_aEnemy[nCntEn].rot.z + (-D3DX_PI * 0.5f)) * 10.0f, -cosf(g_aEnemy[nCntEn].rot.z + (D3DX_PI * 0.5f)) * 10.0f, 0.0f), g_aEnemy[nCntEn].rot, 50.0f, 50.0f, 65, BULLETTYPE_ENEMY);
							g_aEnemy[nCntEn].nBulletCounter = 0;		//カウンターをリセット
						}
					}

				}
	//===========================
	//3種類目の敵が使われている時
	//===========================
				else if (g_aEnemy[nCntEn].nType == ENEMYTYPE_THREE)
				{
					switch (g_aEnemy[nCntEn].Threestate)
					{
					case 	ENEMY_TO_RIGHT:
						g_aEnemy[nCntEn].move.x = 5.0f;
						g_aEnemy[nCntEn].move.y = 0.0f;
						if (g_aEnemy[nCntEn].pos.x >= SCREEN_WIDTH - MAX_ENEMY_SIZE)
						{
							g_aEnemy[nCntEn].Threestate = ENEMY_TO_UNDER;
						}

						if (g_aEnemy[nCntEn].nBulletCounter >= 150)		//一定時間が経過した
						{
							SetBullet(g_aEnemy[nCntEn].pos, D3DXVECTOR3(-sinf(g_aEnemy[nCntEn].rot.z + D3DX_PI) * 10.0f, -cosf(g_aEnemy[nCntEn].rot.z - D3DX_PI) * 10.0f, 0.0f), g_aEnemy[nCntEn].rot, 50.0f, 50.0f, 100, BULLETTYPE_ENEMY);
							g_aEnemy[nCntEn].nBulletCounter = 0;		//カウンターをリセット
						}

						break;

					case 	ENEMY_TO_UNDER:
						g_aEnemy[nCntEn].move.x = 0.0f;
						g_aEnemy[nCntEn].move.y = 3.0f;
						if (g_aEnemy[nCntEn].pos.y >= 535.0f)
						{
							g_aEnemy[nCntEn].Threestate = ENEMY_TO_LEFT;
						}

						if (g_aEnemy[nCntEn].nBulletCounter >= 150)		//一定時間が経過した
						{
							SetBullet(g_aEnemy[nCntEn].pos, D3DXVECTOR3(-sinf(g_aEnemy[nCntEn].rot.z + D3DX_PI * 0.5f) * 10.0f, -cosf(g_aEnemy[nCntEn].rot.z - D3DX_PI * 0.5f) * 10.0f, 0.0f), g_aEnemy[nCntEn].rot, 50.0f, 50.0f, 30, BULLETTYPE_ENEMY);
							g_aEnemy[nCntEn].nBulletCounter = 0;		//カウンターをリセット
						}

						break;

					case 	ENEMY_TO_LEFT:
						g_aEnemy[nCntEn].move.x = -5.0f;
						g_aEnemy[nCntEn].move.y = 0.0f;
						if (g_aEnemy[nCntEn].pos.x <= MAX_ENEMY_SIZE)
						{
							g_aEnemy[nCntEn].Threestate = ENEMY_TO_TOP;
						}

						if (g_aEnemy[nCntEn].nBulletCounter >= 150)		//一定時間が経過した
						{
							SetBullet(g_aEnemy[nCntEn].pos, D3DXVECTOR3(-sinf(g_aEnemy[nCntEn].rot.z + D3DX_PI) * 10.0f, -cosf(g_aEnemy[nCntEn].rot.z - D3DX_PI) * 10.0f, 0.0f), g_aEnemy[nCntEn].rot, 50.0f, 50.0f, 100, BULLETTYPE_ENEMY);
							g_aEnemy[nCntEn].nBulletCounter = 0;		//カウンターをリセット
						}

						break;

					case 	ENEMY_TO_TOP:
						g_aEnemy[nCntEn].move.x = 0.0f;
						g_aEnemy[nCntEn].move.y = -3.0f;
						if (g_aEnemy[nCntEn].pos.y <= 130.0f)
						{
							g_aEnemy[nCntEn].Threestate = ENEMY_TO_RIGHT;
						}

						if (g_aEnemy[nCntEn].nBulletCounter >= 150)		//一定時間が経過した
						{

							SetBullet(g_aEnemy[nCntEn].pos, D3DXVECTOR3(-sinf(g_aEnemy[nCntEn].rot.z + (-D3DX_PI * 0.5f)) * 10.0f, -cosf(g_aEnemy[nCntEn].rot.z + (D3DX_PI * 0.5f)) * 10.0f, 0.0f), g_aEnemy[nCntEn].rot, 50.0f, 50.0f, 30, BULLETTYPE_ENEMY);
							g_aEnemy[nCntEn].nBulletCounter = 0;		//カウンターをリセット
						}

						break;

					default:
						break;
					}

				}
	//===========================
	//4種類目の敵が使われている時
	//===========================
				else if (g_aEnemy[nCntEn].nType == ENEMYTYPE_FOUR)
				{
					g_aEnemy[nCntEn].move.x = 0.0f;
					g_aEnemy[nCntEn].move.y = 0.0f;

					if (g_aEnemy[nCntEn].pos.x >= SCREEN_WIDTH * 0.5f)
					{
						//位置が右上の時
						if (g_aEnemy[nCntEn].pos.y <= SCREEN_HEIGHT * 0.5f)
						{
							//敵の弾の設定
							if (g_aEnemy[nCntEn].nBulletCounter >= 100)		//一定時間が経過した
							{
								SetBullet(g_aEnemy[nCntEn].pos, D3DXVECTOR3(-sinf(g_aEnemy[nCntEn].rot.z + D3DX_PI * (0.75f)) * 10.0f, -cosf(g_aEnemy[nCntEn].rot.z - D3DX_PI * (0.75f)) * 10.0f, 0.0f), g_aEnemy[nCntEn].rot, 50.0f, 50.0f, 80, BULLETTYPE_ENEMY);
								g_aEnemy[nCntEn].nBulletCounter = 0;		//カウンターをリセット
							}

						}
						else
						{
							//右下
							//敵の弾の設定
							if (g_aEnemy[nCntEn].nBulletCounter >= 150)		//一定時間が経過した
							{
								SetBullet(g_aEnemy[nCntEn].pos, D3DXVECTOR3(-sinf(g_aEnemy[nCntEn].rot.z + D3DX_PI * (0.25f)) * 10.0f, -cosf(g_aEnemy[nCntEn].rot.z - D3DX_PI * (0.25f)) * 10.0f, 0.0f), g_aEnemy[nCntEn].rot, 50.0f, 50.0f, 80, BULLETTYPE_ENEMY);
								g_aEnemy[nCntEn].nBulletCounter = 0;		//カウンターをリセット
							}

						}
					}
					else
					{
						//位置が右上の時
						if (g_aEnemy[nCntEn].pos.y <= SCREEN_HEIGHT * 0.5f)
						{
							//敵の弾の設定
							if (g_aEnemy[nCntEn].nBulletCounter >= 50)		//一定時間が経過した
							{
								SetBullet(g_aEnemy[nCntEn].pos, D3DXVECTOR3(-sinf(g_aEnemy[nCntEn].rot.z + D3DX_PI * (-0.75f)) * 10.0f, -cosf(g_aEnemy[nCntEn].rot.z - D3DX_PI * (-0.75f)) * 10.0f, 0.0f), g_aEnemy[nCntEn].rot, 50.0f, 50.0f, 80, BULLETTYPE_ENEMY);
								g_aEnemy[nCntEn].nBulletCounter = 0;		//カウンターをリセット
							}

						}
						else
						{
							//右下
							//敵の弾の設定
							if (g_aEnemy[nCntEn].nBulletCounter >= 200)		//一定時間が経過した
							{
								SetBullet(g_aEnemy[nCntEn].pos, D3DXVECTOR3(-sinf(g_aEnemy[nCntEn].rot.z + D3DX_PI * (-0.25f)) * 10.0f, -cosf(g_aEnemy[nCntEn].rot.z - D3DX_PI * (-0.25f)) * 10.0f, 0.0f), g_aEnemy[nCntEn].rot, 50.0f, 50.0f, 80, BULLETTYPE_ENEMY);
								g_aEnemy[nCntEn].nBulletCounter = 0;		//カウンターをリセット
							}

						}

					}

				}

	//===========================
	//5種類目の敵が使われている時
	//===========================
				else if (g_aEnemy[nCntEn].nType == ENEMYTYPE_FIVE)
				{
					float fspeed = (float)(rand() % 5) + 0.3f;

					//プレイヤーの位置を取得
					D3DXVECTOR3 playerPos = GetPlayer()->pos;

					//プレイヤーとの距離の差を求める
					D3DXVECTOR3 diff = playerPos - g_aEnemy[nCntEn].pos;

					//角度の算出
					float fAngle = atan2f(diff.x, diff.y);

					//移動量の更新
					g_aEnemy[nCntEn].move.x = sinf(fAngle) * fspeed;
					g_aEnemy[nCntEn].move.y = cosf(fAngle) * fspeed;

					//位置を更新
					g_aEnemy[nCntEn].pos.x += g_aEnemy[nCntEn].move.x;
					g_aEnemy[nCntEn].pos.y += g_aEnemy[nCntEn].move.y;


					//敵の弾の設定
					if (g_aEnemy[nCntEn].nBulletCounter >= 120)		//一定時間が経過した
					{
						SetBullet(g_aEnemy[nCntEn].pos, D3DXVECTOR3(-sinf(g_aEnemy[nCntEn].rot.z + D3DX_PI) * 10.0f, -cosf(g_aEnemy[nCntEn].rot.z - D3DX_PI) * 10.0f, 0.0f), g_aEnemy[nCntEn].rot, 50.0f, 50.0f, 70, BULLETTYPE_ENEMY);
						g_aEnemy[nCntEn].nBulletCounter = 0;		//カウンターをリセット
					}

				}
				
	//===========================
	//6種類目の敵が使われている時
	//===========================
				else if (g_aEnemy[nCntEn].nType == ENEMYTYPE_SIX)
				{
					g_aEnemy[nCntEn].move.y = 0.0f;
					//タイマーを加算
					g_nEnemyTimer++;

					if (g_nEnemyTimer == 60)
					{
						g_aEnemy[nCntEn].pos.x = 1260.0f;
					}
					else if (g_nEnemyTimer == 120)
					{
						g_aEnemy[nCntEn].pos.x = 640.0f;

					}
					else if (g_nEnemyTimer == 180)
					{
						g_aEnemy[nCntEn].pos.x = 20.0f;

						g_nEnemyTimer = 0;	//初期値に戻す

					}

					//敵の弾の設定
					if (g_aEnemy[nCntEn].nBulletCounter >= 50)		//一定時間が経過した
					{
						SetBullet(g_aEnemy[nCntEn].pos, D3DXVECTOR3(-sinf(g_aEnemy[nCntEn].rot.z + D3DX_PI) * 10.0f, -cosf(g_aEnemy[nCntEn].rot.z - D3DX_PI) * 10.0f, 0.0f), g_aEnemy[nCntEn].rot, 50.0f, 50.0f, 50, BULLETTYPE_ENEMY);
						g_aEnemy[nCntEn].nBulletCounter = 0;		//カウンターをリセット
					}

				}

				g_aEnemy[nCntEn].pos += g_aEnemy[nCntEn].move;		//位置の更新

				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEn].pos.x - MAX_ENEMY_SIZE, g_aEnemy[nCntEn].pos.y - MAX_ENEMY_SIZE, 0.0f);//1つ目の頂点情報
				pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEn].pos.x + MAX_ENEMY_SIZE, g_aEnemy[nCntEn].pos.y - MAX_ENEMY_SIZE, 0.0f);//2つ目の頂点情報
				pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEn].pos.x - MAX_ENEMY_SIZE, g_aEnemy[nCntEn].pos.y + MAX_ENEMY_SIZE, 0.0f);//3つ目の頂点情報
				pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEn].pos.x + MAX_ENEMY_SIZE, g_aEnemy[nCntEn].pos.y + MAX_ENEMY_SIZE, 0.0f);//4つ目の頂点情報

				//頂点カラーの設定(白に戻す)
				pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					
				break;

			case ENEMYSTATE_DAMAGE:
				g_aEnemy[nCntEn].nCounterState--;
				if (g_aEnemy[nCntEn].nCounterState <= 0)
				{
					//通常状態に戻す
					g_aEnemy[nCntEn].state = ENEMYSTATE_NORMAL;
				}
				break;
			}
		}
		pVtx += 4;
	}

	//アンロック
	g_pVtxBuffEnemy->Unlock();
}
//===========================
//敵の描画処理
//===========================
void DrawEnemy(void)
{
	int nCntEnemy;		//ローカル変数

	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffEnemy, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{
			//使用されてる
			int nType = g_aEnemy[nCntEnemy].nType;
			//テクスチャの設定
			pDevice->SetTexture(0, g_apTextureEnemy[nType]);

			//敵の描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEnemy * 4, 2);//始点(0番目～3番目)

		}
	}

#ifdef _DEBUG

	//敵のデバッグ表示
	DrawEnemySet(g_nNumEnemy);

#endif
}
//============================
//敵の設定処理
//============================
void SetEnemy(D3DXVECTOR3 pos, int nType)
{
	int nCntEnemy;		//ローカル変数

	VERTEX_2D* pVtx;		//頂点情報のポインタ


	//頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == false)
		{
			//使用されてない
			g_aEnemy[nCntEnemy].pos = pos;					//座標
			g_aEnemy[nCntEnemy].nType = nType;				//種類
			g_aEnemy[nCntEnemy].bUse = true;				//使用判定
			g_aEnemy[nCntEnemy].nLife = 0;					//体力
			g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;  //状態

			//敵のタイプ分け
			if (ENEMYTYPE_OWN == nType)
			{
				//１種類目が使用されている
				g_aEnemy[nCntEnemy].nLife = 2;

			}
			else if (ENEMYTYPE_TWO == nType)
			{
				//2種類目が使用されている
				g_aEnemy[nCntEnemy].nLife = 2;
			}
			else if (ENEMYTYPE_THREE == nType)
			{
				//3種類目が使用されている
				g_aEnemy[nCntEnemy].nLife = 3;
			}
			else if (ENEMYTYPE_FOUR == nType)
			{
				//6種類目が使用されている
				g_aEnemy[nCntEnemy].nLife = 4;
			}
			else if (ENEMYTYPE_FIVE == nType)
			{
				//5種類目が使用されている
				g_aEnemy[nCntEnemy].nLife = 1;
			}
			else if (ENEMYTYPE_SIX == nType)
			{
				//6種類目が使用されている
				g_aEnemy[nCntEnemy].nLife = 5;
			}

			//頂点情報の設定
			pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - MAX_ENEMY_SIZE, g_aEnemy[nCntEnemy].pos.y - MAX_ENEMY_SIZE, 0.0f);	//1つ目の頂点情報
			pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + MAX_ENEMY_SIZE, g_aEnemy[nCntEnemy].pos.y - MAX_ENEMY_SIZE, 0.0f);	//2つ目の頂点情報
			pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - MAX_ENEMY_SIZE, g_aEnemy[nCntEnemy].pos.y + MAX_ENEMY_SIZE, 0.0f);	//3つ目の頂点情報
			pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + MAX_ENEMY_SIZE, g_aEnemy[nCntEnemy].pos.y + MAX_ENEMY_SIZE, 0.0f);	//4つ目の頂点情報

			g_nNumEnemy++;	//カウントを加算

			break;
		}

		pVtx += 4;
	}
	//アンロック
	g_pVtxBuffEnemy->Unlock();

}
//============================
//敵のヒット処理
//============================
void HitEnemy(int nCntEnemy, int nDamage)
{
	VERTEX_2D* pVtx;//頂点情報のポイタ

			//プレイヤーの取得
	Player* pPlayer = GetPlayer();

	g_aEnemy[nCntEnemy].nLife -= nDamage;		//ダメージ受けたら体力減らす

	if (g_aEnemy[nCntEnemy].nLife <= 0)
	{
		g_aEnemy[nCntEnemy].bUse = false;
		//パーティクル処理
		SetParticle(g_aEnemy[nCntEnemy].pos,D3DXCOLOR (1.0f,0.2f,0.5f,1.0f));

		if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_OWN)
		{
			AddScore(10000);//スコアを加算

			//アイテム生成
			SetItem(g_aEnemy[nCntEnemy].pos,0);

		}
		if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_TWO)
		{
			AddScore(20000);//スコアを加算

			//アイテム生成
			SetItem(g_aEnemy[nCntEnemy].pos, 1);

		}
		if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_THREE)
		{
			AddScore(30000);//スコアを加算

			//アイテム生成
			SetItem(g_aEnemy[nCntEnemy].pos, 2);

		}
		if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_FOUR)
		{
			AddScore(40000);//スコアを加算

			//アイテム生成
			SetItem(g_aEnemy[nCntEnemy].pos, 3);

		}
		if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_FIVE)
		{
			AddScore(50000);//スコアを加算

			//アイテム生成
			SetItem(g_aEnemy[nCntEnemy].pos, 4);

		}
		if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_SIX)
		{
			AddScore(70000);//スコアを加算

			//アイテム生成
			SetItem(g_aEnemy[nCntEnemy].pos, 5);
		}

		g_nNumEnemy--;		//敵の総数をカウント(デクリメント)

	}
	else
	{
		//頂点バッファをロックし,頂点情報へのポインタを取得
		g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

		g_aEnemy[nCntEnemy].state = ENEMYSTATE_DAMAGE;		//ダメージ状態
		g_aEnemy[nCntEnemy].nCounterState = 5;				//ダメージ状態の時間

		pVtx += 4 * nCntEnemy;

		//頂点カラーの設定(ダメージ色)
		pVtx[0].col = D3DXCOLOR(0.0f,0.0f,0.7f,1.0f);
		pVtx[1].col = D3DXCOLOR(0.0f,0.0f,0.7f,1.0f);
		pVtx[2].col = D3DXCOLOR(0.0f,0.0f,0.7f,1.0f);
		pVtx[3].col = D3DXCOLOR(0.0f,0.0f,0.7f,1.0f);

	}

	//アンロック
	g_pVtxBuffEnemy->Unlock();

}
//=========================
//敵のカウントを取得
//=========================
int GetNumEnemy(void)
{
	return g_nNumEnemy;
}
//============================
//敵の取得
//============================
Enemy* GetEnemy(void)
{
	return &g_aEnemy[0];
}

