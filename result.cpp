//---------------------------
//
//リザルト処理
//Author:NISHIO ASUMA
//
//---------------------------

#include "result.h"
#include "fade.h"
#include "wave.h"
#include "player.h"
#include "score.h"
#include "sound.h"
#include "resultscore.h"
#include "Timer.h"


//グローバル変数
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResult = NULL;		//頂点バッファのポインタ
LPDIRECT3DTEXTURE9 g_pTextureResult= NULL;				//テクスチャのポインタ
int g_aCount;

//==================
//リザルトの初期化
//==================
void InitResult(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//プレイヤーの取得
	Player* pPlayer = GetPlayer();

	//タイマー情報を取得
	int nTime = GetTimer();

	VERTEX_2D* pVtx;		//頂点情報のポインタ

	if (pPlayer->bDisp == false || nTime <= 0)
	{
		PlaySound(SOUND_LABEL_RESULT1);

		//敵全滅でゲームクリア Wave終了
		//テクスチャ2の読み込み
		D3DXCreateTextureFromFile(pDevice,
			"data\\TEXTURE\\result001.png",
			&g_pTextureResult);
	}

	//初期化
	g_aCount = 0;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY, FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffResult,
		NULL);

	//頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

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
	pVtx[0].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//アンロック
	g_pVtxBuffResult->Unlock();

	//リザルトスコアの初期化
	InitResultScore();
}
//==================
//リザルトの終了
//==================
void UninitResult(void)
{
	StopSound();

	//テクスチャの破棄
	if (g_pTextureResult != NULL)
	{
		g_pTextureResult->Release();
		g_pTextureResult = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffResult != NULL)
	{
		g_pVtxBuffResult->Release();
		g_pVtxBuffResult = NULL;
	}

	//リザルトスコアの終了
	UninitResultScore();
}
//==================
//リザルトの更新
//==================
void UpdateResult(void)
{
	//カウントを加算
	g_aCount++;

	if (g_aCount >= 380 || KeyboardTrigger(DIK_RETURN) == true || JoypadTrigger(JOYKEY_A) == true)
	{
		//Enterキーが押された
		//モード設定(リザルトに移動)
		SetFade(MODE_RANKING);
	}

	//リザルトスコアの更新
	UpdateResultScore();

}
//==================
//リザルトの描画
//==================
void DrawResult(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffResult, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureResult);

	//リザルトポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//リザルトスコアの描画
	DrawResultScore();

}
