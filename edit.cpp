//---------------------------
//
//wave処理(書き出し)
//Author:NISHIO ASUMA
//
//---------------------------

#include "edit.h"
#include "input.h"
#include "wave.h"
#include <stdio.h>

#define EDIT_MOVE (20.0f)		//敵の移動量(配置時)

//エディット情報の構造体
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	int nType;			//種類
	bool bUse;			//使用状態
}EditInfo;

//グローバル変数
int g_nCntEdit;//敵を配置した数をカウント
EditInfo g_aEditInfo[MAX_ENEMY];	//情報
LPDIRECT3DTEXTURE9 g_apTextureEdit[ENEMYTYPE_MAX] = {};//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEdit = NULL;//頂点バッファのポインタ

//========================
//初期化
//========================
void InitEdit(void)
{
	int nCntEdit;	//ローカル変数

	//構造体変数の初期化
	for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++)
	{
		g_aEditInfo[nCnt].nType = 0;							//種類
		g_aEditInfo[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//位置
		g_aEditInfo[nCnt].bUse = false;							//使用判定
	}

	//最初の番号だけ使用判定
	g_aEditInfo[0].bUse = true;	//使用判定

	LPDIRECT3DDEVICE9 pDevice;	//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	VERTEX_2D* pVtx;			//頂点情報のポインタ

	for (int nCnt = 0; nCnt < ENEMYTYPE_MAX; nCnt++)
	{
		//敵のテクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			ENEMY_TEXTURE[nCnt],
			&g_apTextureEdit[nCnt]);
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_ENEMY,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEdit,
		NULL);

	//頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffEdit->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEdit = 0; nCntEdit < MAX_ENEMY; nCntEdit++)
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
	g_pVtxBuffEdit->Unlock();
}
//========================
//終了
//========================
void UninitEdit(void)
{
	int nCntEdit;//敵の数
	//テクスチャの破棄
	for (nCntEdit = 0; nCntEdit < ENEMYTYPE_MAX; nCntEdit++)
	{

		if (g_apTextureEdit[nCntEdit] != NULL)
		{
			g_apTextureEdit[nCntEdit]->Release();
			g_apTextureEdit[nCntEdit] = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBuffEdit != NULL)
	{
		g_pVtxBuffEdit->Release();
		g_pVtxBuffEdit = NULL;
	}
}
//========================
//更新
//========================
void UpdateEdit(void)
{
	//移動処理
	if (KeyboardTrigger(DIK_A) == true)
	{
		//A単体の処理
		g_aEditInfo[g_nCntEdit].pos.x -= EDIT_MOVE;
	}
	else if (KeyboardTrigger(DIK_D) == true)
	{
		//D単体の処理
		g_aEditInfo[g_nCntEdit].pos.x += EDIT_MOVE;
	}
	else if (KeyboardTrigger(DIK_W) == true)
	{
		//W単体の処理
		g_aEditInfo[g_nCntEdit].pos.y -= EDIT_MOVE;
	}
	else if (KeyboardTrigger(DIK_S) == true)
	{
		//S単体の処理
		g_aEditInfo[g_nCntEdit].pos.y += EDIT_MOVE;
	}

	if (KeyboardTrigger(DIK_UP) == true)
	{
		if (g_aEditInfo[g_nCntEdit].nType < ENEMYTYPE_MAX-1)
		{
			//最大数より小さいとき
			g_aEditInfo[g_nCntEdit].nType++;
		}

	}
	else if (KeyboardTrigger(DIK_DOWN) == true)
	{
		if (g_aEditInfo[g_nCntEdit].nType > ENEMYTYPE_OWN)
		{
			//1より大きいとき
			g_aEditInfo[g_nCntEdit].nType--;
		}
	}

	//敵の配置
	if (KeyboardTrigger(DIK_RETURN) == true)
	{
		//次の敵の位置を配置した敵と同じにする
		g_aEditInfo[g_nCntEdit + 1].pos = g_aEditInfo[g_nCntEdit].pos;

		//使用判定にする
		g_aEditInfo[g_nCntEdit + 1].bUse = true;	

		//敵の数をインクリメント
		g_nCntEdit++;
	}

	//書き出し処理
	if (KeyboardTrigger(DIK_F7) == true)
	{
		//保存する
		SaveEdit();
	}

	VERTEX_2D* pVtx;	//頂点情報のポインタ

	//頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffEdit->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEdit = 0; nCntEdit < MAX_ENEMY; nCntEdit++)
	{
		if (g_aEditInfo[nCntEdit].bUse == true)
		{
			//頂点情報の設定
			pVtx[0].pos = D3DXVECTOR3(g_aEditInfo[nCntEdit].pos.x - MAX_ENEMY_SIZE, g_aEditInfo[nCntEdit].pos.y - MAX_ENEMY_SIZE, 0.0f);//1つ目の頂点情報
			pVtx[1].pos = D3DXVECTOR3(g_aEditInfo[nCntEdit].pos.x + MAX_ENEMY_SIZE, g_aEditInfo[nCntEdit].pos.y - MAX_ENEMY_SIZE, 0.0f);//2つ目の頂点情報
			pVtx[2].pos = D3DXVECTOR3(g_aEditInfo[nCntEdit].pos.x - MAX_ENEMY_SIZE, g_aEditInfo[nCntEdit].pos.y + MAX_ENEMY_SIZE, 0.0f);//3つ目の頂点情報
			pVtx[3].pos = D3DXVECTOR3(g_aEditInfo[nCntEdit].pos.x + MAX_ENEMY_SIZE, g_aEditInfo[nCntEdit].pos.y + MAX_ENEMY_SIZE, 0.0f);//4つ目の頂点情報
		}

		pVtx += 4;
	}
	//アンロック
	g_pVtxBuffEdit->Unlock();

}
//========================
//描画
//========================
void DrawEdit(void)
{
	int nCntEdit;//ローカル変数

	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffEdit, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntEdit = 0; nCntEdit < MAX_ENEMY; nCntEdit++)
	{
		if (g_aEditInfo[nCntEdit].bUse == true)
		{
			//使用されてる
			int nType = g_aEditInfo[nCntEdit].nType;
			//テクスチャの設定
			pDevice->SetTexture(0, g_apTextureEdit[nType]);

			//敵の描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEdit * 4, 2);//始点(0番目～3番目)

		}
	}
}
//========================
//ウェーブ処理(書きだし)
//========================
void SaveEdit(void)
{

	FILE* pFile;	//ファイルポインタを宣言

	int nType;			//種類
	D3DXVECTOR3 pos;	//座標

	//ファイルを開く
	pFile = fopen(WAVEPATH_BIN_7,"wb");

	if (pFile != NULL)
	{

		fwrite(&g_nCntEdit, sizeof(int), 1, pFile);

		//配置した敵の数だけ周回
		for (int nCnt = 0; nCnt < g_nCntEdit; nCnt++)
		{
			if (g_aEditInfo[nCnt].bUse == true)
			{
				fwrite(&g_aEditInfo[nCnt], sizeof(EditInfo), 1, pFile);
			}

			//fprintf(pFile, "SETENEMY\n");

			//fprintf(pFile, "ENEMYTYPE %d\n", g_aEditInfo[nCnt].nType);
			//
			//fprintf(pFile, "POS %f %f %f\n", g_aEditInfo[nCnt].pos.x, g_aEditInfo[nCnt].pos.y, g_aEditInfo[nCnt].pos.z);
			//
			//fprintf(pFile, "END_SETENEMY\n");
		}

		//fprintf(pFile, "END_SCRIPT\n");

		//ファイルを閉じる
		fclose(pFile);
	}
	else
	{
		//ファイルが開けなかった時
		//メッセージBOXの表示
		MessageBox(NULL, "開けません", "エラー", MB_OK);

		return;

	}
}
