//---------------------------
//
//ポーズ処理
//Author:NISHIO ASUMA
//
//---------------------------

#include "pause.h"
#include "fade.h"
#include "input.h"
#include "game.h"
#include "sound.h"


//グローバル
LPDIRECT3DTEXTURE9 g_apTexturePause[MAX_PAUSE_TEX] = {};	//テクスチャへのポインタ(4枚分)
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause = NULL;				//頂点バッファのポインタ

LPDIRECT3DTEXTURE9 g_apTexturearufa = NULL;					//テクスチャへのポインタ(透明)
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffarufa = NULL;				//頂点バッファのポインタ

PAUSE_MENU g_pauseMenu;		//ポーズメニュー
int g_nSelect;				//選択用の変数

//=========================
//ポーズの初期化
//=========================
void InitPause(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み(透明)
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\arufa.png",
		&g_apTexturearufa);

	//テクスチャ1の読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\pause000.png",
		&g_apTexturePause[0]);

	//テクスチャ2の読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\pause001.png",
		&g_apTexturePause[1]);

	//テクスチャ3の読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\pause002.png",
		&g_apTexturePause[2]);

	//テクスチャの4読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\rankingrogo.png",
		&g_apTexturePause[3]);

	g_nSelect = PAUSE_MENU_CONTINUE;  //初期の選択モード
	VERTEX_2D* pVtx;		//頂点情報のポインタ


	//=====================
	//透明ポリゴンの設定
	//=====================

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY, FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffarufa,
		NULL);

	//頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffarufa->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標1の設定
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
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//アンロック
	g_pVtxBuffarufa->Unlock();

	//=========================
	//3つのポリゴン設定
	//=========================

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_PAUSE_TEX,
		D3DUSAGE_WRITEONLY, FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPause,
		NULL);


	//頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_PAUSE_TEX; nCnt++)
	{
		//頂点座標1の設定
		pVtx[0].pos = D3DXVECTOR3(470.0f,100.0f + (nCnt * 120.0f),0.0f);	//1つ目の頂点情報
		pVtx[1].pos = D3DXVECTOR3(870.0f,100.0f + (nCnt * 120.0f),0.0f);	//2つ目の頂点情報
		pVtx[2].pos = D3DXVECTOR3(470.0f,200.0f +(nCnt * 120.0f),0.0f);		//3つ目の頂点情報
		pVtx[3].pos = D3DXVECTOR3(870.0f,200.0f +(nCnt * 120.0f),0.0f);		//4つ目の頂点情報
		 
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

		pVtx += 4;
	}

	//アンロック
	g_pVtxBuffPause->Unlock();

}
//=========================
//ポーズの終了
//=========================
void UninitPause(void)
{
	StopSound();

	//=========================
	//透明の終了
	//=========================
	//テクスチャの終了
	if (g_apTexturearufa != NULL)
	{			   
		g_apTexturearufa->Release();
		g_apTexturearufa = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffarufa != NULL)
	{
		g_pVtxBuffarufa->Release();
		g_pVtxBuffarufa = NULL;
	}

	//=========================
	//4つのポリゴンの終了
	//=========================
	//テクスチャの終了
	for (int nCnt = 0; nCnt < MAX_PAUSE_TEX; nCnt++)
	{
		if (g_apTexturePause[nCnt] != NULL)
		{
			g_apTexturePause[nCnt]->Release();
			g_apTexturePause[nCnt] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffPause != NULL)
	{
		g_pVtxBuffPause->Release();
		g_pVtxBuffPause = NULL;
	}
}
//=========================
//ポーズの更新
//=========================
void UpdatePause(void)
{
	VERTEX_2D* pVtx;		//頂点情報のポインタ

	//頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCount = 0; nCount < MAX_PAUSE_TEX; nCount++)
	{
		//頂点カラーを設定(白)
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		pVtx += 4;
	}

	pVtx -= 4 * MAX_PAUSE_TEX;		//pVtxを戻す
	
	//メニューの選択
	if (KeyboardTrigger(DIK_UP) == true || JoypadTrigger(JOYKEY_UP) == true)
	{
		//上押した
		g_nSelect--;//デクリメント
	
		if (g_nSelect < PAUSE_MENU_CONTINUE)
		{
			//一番下
			g_nSelect = PAUSE_MENU_RANKING;
		}


	}

	else if (KeyboardTrigger(DIK_DOWN) == true || JoypadTrigger(JOYKEY_DOWN) == true)
	{
		//下押したとき
		g_nSelect++;//インクリメント
		
		if (g_nSelect >= PAUSE_MENU_MAX)
		{
			//一番上
			g_nSelect = PAUSE_MENU_CONTINUE;
		}

	}

	pVtx += 4 * g_nSelect;		//選択肢


	pVtx[0].col = D3DXCOLOR(0.0f,0.0f,1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(0.0f,0.0f,1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(0.0f,0.0f,1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(0.0f,0.0f,1.0f, 1.0f);

	//アンロック
	g_pVtxBuffPause->Unlock();

	if (KeyboardTrigger(DIK_RETURN) == true || JoypadTrigger(JOYKEY_A) == true)
	{
		//エンターキーが押された
		//各画面に移動
		switch (g_nSelect)
		{
		case PAUSE_MENU_CONTINUE:
			SetEnablePause(false);
			break;

		case PAUSE_MENU_RETRY:
			SetFade(MODE_GAME);
			break;

		case PAUSE_MENU_QUIT:
			SetFade(MODE_TITLE);
			break;

		case PAUSE_MENU_RANKING:
			SetFade(MODE_RANKING);
			break;

		}
		PlaySound(SOUND_LABEL_PAUSE);
	}
}
//=========================
//ポーズの描画
//=========================
void DrawPause(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//=========================
	//透明ポリゴンの終了
	//=========================
	
	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffarufa, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_apTexturearufa);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);		//始点

	//=========================
	//4つのポリゴンの描画
	//=========================

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPause, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	VERTEX_2D* pVtx = 0;//頂点情報のポインタ


	for (int nCnt = 0; nCnt < MAX_PAUSE_TEX; nCnt++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_apTexturePause[nCnt]);

		//描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);//ポリゴン

	}
}
