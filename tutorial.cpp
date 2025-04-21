//========================================
//
//チュートリアル処理
//Author:ASUMA NISHIO
//
//========================================

#include "tutorial.h"
#include "player.h"
#include "game.h"
#include "fade.h"
#include "enemy.h"
#include "bullet.h"
#include "background.h"
#include "effect.h"
#include "wave.h"
#include "score.h"
#include "sound.h"
#include "tutorial2.h"
#include "effect.h"
#include "particle.h"

//マクロ定義
#define SIZE_WIDTH  (50.0f)	//横幅
#define SIZE_HEIGHT (50.0f) //高さ

//チュートリアル構造体
typedef struct
{
	D3DXVECTOR3 pos;		//座標
	D3DXVECTOR3 col;		//カラー
	D3DXVECTOR3 move;		//移動量
	int nType;				//種類
	bool bUse;				//使用状態
	float fHeight;			//高さ
	float fWidth;			//幅
	TUTORIALSTATE state;	//状態
}Tutorial;

//グローバル変数
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTutorial = NULL;			//頂点バッファのポインタ
LPDIRECT3DTEXTURE9 g_pTextureTutorial[TUTORIAL_MAX] = {};	//テクスチャのポインタ

LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTutorialBG = NULL;		//頂点バッファのポインタ(背景)
LPDIRECT3DTEXTURE9 g_pTextureTutorialBG = NULL;				//テクスチャのポインタ(背景)

Tutorial g_aTutorial[TUTORIAL_MAX];							//チュートリアル構造体の情報
TUTORIAL g_Tutorial;										//チュートリアル列挙型の情報

int g_nCntTutoAnim;			//チュートリアル状態
int g_nPatternTutoAnim;		//状態パターン
bool g_bNext;				//フラグ
int g_nCntMove;				//移動量のカウント


//======================
//チュートリアルの初期化
//======================
void InitTutorial(void)
{
	InitPlayer();			//プレイヤーの初期化

	InitBullet();			//弾の初期化

	InitTutorial2();		//2個目のチュートリアルの初期化

	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	VERTEX_2D* pVtx;		//頂点情報のポインタ

	for (int nCnt = 0; nCnt < TUTORIAL_MAX; nCnt++)
	{
		//敵のテクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			TUTORIAL_TEXTURE[nCnt],
			&g_pTextureTutorial[nCnt]);
	}

	//テクスチャの読み込み(背景)
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\tutorialbg.png",
		&g_pTextureTutorialBG);

	for (int nCntTutorial = 0; nCntTutorial < TUTORIAL_MAX; nCntTutorial++)
	{
		g_aTutorial[nCntTutorial].nType = 0;							//種類
		g_aTutorial[nCntTutorial].fHeight = 0.0f;						//高さ
		g_aTutorial[nCntTutorial].fWidth = 0.0f;						//幅
		g_aTutorial[nCntTutorial].bUse = false;							//未使用判定
		g_aTutorial[nCntTutorial].state = TUTORIALSTATE_NONE;			//何もしていない状態
		g_aTutorial[nCntTutorial].move = D3DXVECTOR3(-20.0f,0.0f,0.0f); //移動量
	}

	g_nCntTutoAnim = 0;			//チュートリアル状態
	g_nPatternTutoAnim = 0;		//状態パターン
	g_bNext = false;
	g_nCntMove = 0;
//=========================
//チュートリアルの背景設定
//=========================

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY, FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTutorialBG,
		NULL);

	//頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffTutorialBG->Lock(0, 0, (void**)&pVtx, 0);

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
	g_pVtxBuffTutorialBG->Unlock();

//============================
//チュートリアルの各種の初期化
//============================

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * TUTORIAL_MAX,
		D3DUSAGE_WRITEONLY, FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTutorial,
		NULL);

	//頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffTutorial->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < TUTORIAL_MAX; nCnt++)
	{
		//頂点座標1の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//1つ目の頂点情報
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//2つ目の頂点情報
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//3つ目の頂点情報
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//4つ目の頂点情報

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
	g_pVtxBuffTutorial->Unlock();

	SetTutorial(TUTORIAL_ENTER,			D3DXVECTOR3(1050.0f, 50.0f, 0.0f));			//チュートリアル2
	SetTutorial(TUTORIAL_KEY_W,			D3DXVECTOR3(300.0f,	170.0f, 0.0f));			//チュートリアル3(w)
	SetTutorial(TUTORIAL_KEY_A,			D3DXVECTOR3(250.0f, 220.0f, 0.0f));			//チュートリアル4(a)
	SetTutorial(TUTORIAL_KEY_S,			D3DXVECTOR3(300.0f, 220.0f, 0.0f));			//チュートリアル5(s)
	SetTutorial(TUTORIAL_KEY_D,			D3DXVECTOR3(350.0f, 220.0f, 0.0f));			//チュートリアル6(d)
	SetTutorial(TUTORIAL_KEY_SPACE,		D3DXVECTOR3(300.0f, 330.0f, 0.0f));			//チュートリアル7(スペース)
	SetTutorial(TUTORIAL_BUTTON_B,		D3DXVECTOR3(1115.0f, 340.0f, 0.0f));		//チュートリアル8
	SetTutorial(TUTORIAL_BUTTON_UP,     D3DXVECTOR3(1030.0f, 170.0f, 0.0f));		//チュートリアル9
	SetTutorial(TUTORIAL_BUTTON_DOWN,   D3DXVECTOR3(1030.0f, 220.0f, 0.0f));		//チュートリアル10
	SetTutorial(TUTORIAL_BUTTON_RIGHT,  D3DXVECTOR3(1081.0f, 200.0f, 0.0f));		//チュートリアル11
	SetTutorial(TUTORIAL_BUTTON_LEFT,   D3DXVECTOR3(980.0f, 200.0f, 0.0f));			//チュートリアル12
	SetTutorial(TUTORIAL_KEY_P,			D3DXVECTOR3(300.0f,	 470.0f, 0.0f));		//チュートリアル12
	SetTutorial(TUTORIAL_BUTTON_ST,		D3DXVECTOR3(1120.0f, 460.0f, 0.0f));		//チュートリアル12
	SetTutorial(TUTORIAL_KEY_E,			D3DXVECTOR3(355.0f,	 630.0f, 0.0f));		//チュートリアル13
	SetTutorial(TUTORIAL_KEY_Q,			D3DXVECTOR3(215.0f,	 630.0f, 0.0f));		//チュートリアル14
	SetTutorial(TUTORIAL_BUTTON_RB,		D3DXVECTOR3(1200.0f, 625.0f, 0.0f));		//チュートリアル15
	SetTutorial(TUTORIAL_BUTTON_LB,		D3DXVECTOR3(1040.0f, 625.0f, 0.0f));		//チュートリアル16
	SetTutorial(TUTORIAL_LSTICK,		D3DXVECTOR3(1190.0f, 200.0f, 0.0f));		//チュートリアル16

	SetTutorial2();			//2個目のチュートリアル

	PlaySound(SOUND_LABEL_TUTORIAL);
}
//=========================
//チュートリアルの終了処理
//=========================
void UninitTutorial(void)
{
	StopSound();

//============================
//チュートリアルの各種の終了
//============================

	for (int nCnt = 0; nCnt < TUTORIAL_MAX; nCnt++)
	{
		//テクスチャの破棄
		if (g_pTextureTutorial[nCnt] != NULL)
		{					   
			g_pTextureTutorial[nCnt]->Release();
			g_pTextureTutorial[nCnt] = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBuffTutorial != NULL)
	{
		g_pVtxBuffTutorial->Release();
		g_pVtxBuffTutorial = NULL;
	}

//============================
//チュートリアルの背景の終了
//============================

	//テクスチャの破棄
	if (g_pTextureTutorialBG != NULL)
	{					  
		g_pTextureTutorialBG->Release();
		g_pTextureTutorialBG = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffTutorialBG != NULL)
	{					  
		g_pVtxBuffTutorialBG->Release();
		g_pVtxBuffTutorialBG = NULL;
	}

	UninitPlayer();		//プレイヤーの終了

	UninitBullet();		//弾の終了

	UninitTutorial2();	//2個目のチュートリアルの終了
}
//=====================
//チュートリアルの更新
//=====================
void UpdateTutorial(void)
{

	UpdatePlayer();		//プレイヤーの更新

	UpdateBullet();		//弾の更新

	FADE g_fade = GetFade();	//現在の状態


	if (KeyboardTrigger(DIK_RETURN) == true || JoypadTrigger(JOYKEY_A) == true)
	{
		PlaySound(SOUND_LABEL_SE_ENTER);

		//Enterキーが押された or STARTボタンが押された
		for (int nCnt = 0; nCnt < TUTORIAL_MAX; nCnt++)
		{
			if (g_aTutorial[nCnt].bUse == true && g_aTutorial[nCnt].nType == TUTORIAL_ENTER)
			{
				g_aTutorial[nCnt].state = TUTORIALSTATE_FLASH;
			}

		}

		if (g_aTutorial[0].pos.x <= -(SCREEN_WIDTH * 0.5f) && g_bNext == false)
		{
			//モード設定(ゲーム画面に移動)
			SetFade(MODE_GAME);
		}

		g_bNext = true;
	}

//================
//キーを押したら
//================

	g_nCntMove++;		//加算

	if (g_aTutorial[0].pos.x <= -640.0f)
	{
		g_bNext = false;

	}

	XINPUT_STATE* pStick;
	pStick = GetJoyStickAngle();

	//頂点バッファをロックし,頂点情報へのポインタを取得
	VERTEX_2D* pVtx;			//頂点情報のポインタ
	g_pVtxBuffTutorial->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < TUTORIAL_MAX; nCnt++)
	{
		if (g_aTutorial[nCnt].bUse == false)
		{
			//キーが押されてないとき
			continue;
		}

		//押したかのフラグ
		bool bPush = false;

		if (g_aTutorial[nCnt].nType == TUTORIAL_KEY_W)
		{
			if (GetKeyboardPress(DIK_W) == true)
			{
				//押した状態にする
				bPush = true;
			}
		}

		if (g_aTutorial[nCnt].nType == TUTORIAL_KEY_A)
		{
			if (GetKeyboardPress(DIK_A) == true)
			{
				//押した状態にする

				bPush = true;
			}
		}

		if (g_aTutorial[nCnt].nType == TUTORIAL_KEY_S)
		{
			if (GetKeyboardPress(DIK_S) == true)
			{
				//押した状態にする

				bPush = true;
			}
		}

		if (g_aTutorial[nCnt].nType == TUTORIAL_KEY_D)
		{
			if (GetKeyboardPress(DIK_D) == true)
			{
				//押した状態にする

				bPush = true;
			}
		}

		if (g_aTutorial[nCnt].nType == TUTORIAL_KEY_SPACE)
		{
			if (GetKeyboardPress(DIK_SPACE) == true)
			{
				//押した状態にする

				bPush = true;
			}
		}
		
		if (g_aTutorial[nCnt].nType == TUTORIAL_BUTTON_B)
		{
#if 0
			if (GetJoyStickRrepeat() == true)
			{
				//押した状態にする
				bPush = true;
			}
#endif
			if (JoypadTrigger(JOYKEY_B) == true)
			{
				//押した状態にする
				bPush = true;
			}
		}

		if (g_aTutorial[nCnt].nType == TUTORIAL_BUTTON_UP)
		{
			if (JoypadPress(JOYKEY_UP) == true)
			{
				//押した状態にする
				bPush = true;
			}
		}

		if (g_aTutorial[nCnt].nType == TUTORIAL_BUTTON_DOWN)
		{
			if (JoypadPress(JOYKEY_DOWN) == true)
			{
				//押した状態にする
				bPush = true;
			}
		}

		if (g_aTutorial[nCnt].nType == TUTORIAL_BUTTON_RIGHT)
		{
			if (JoypadPress(JOYKEY_RIGHT) == true)
			{
				//押した状態にする
				bPush = true;
			}
		}

		if (g_aTutorial[nCnt].nType == TUTORIAL_BUTTON_LEFT)
		{
			if (JoypadPress(JOYKEY_LEFT) == true)
			{
				//押した状態にする
				bPush = true;
			}
		}

		if (g_aTutorial[nCnt].nType == TUTORIAL_KEY_E)
		{
			if (GetKeyboardPress(DIK_E) == true)
			{
				//押した状態にする

				bPush = true;
			}
		}

		if (g_aTutorial[nCnt].nType == TUTORIAL_KEY_Q)
		{
			if (GetKeyboardPress(DIK_Q) == true)
			{
				//押した状態にする

				bPush = true;
			}
		}

		if (g_aTutorial[nCnt].nType == TUTORIAL_BUTTON_RB)
		{
			if (JoypadPress(JOYKEY_RIGHT_B) == true)
			{
				//押した状態にする
				bPush = true;
			}
		}

		if (g_aTutorial[nCnt].nType == TUTORIAL_BUTTON_LB)
		{
			if (JoypadPress(JOYKEY_LEFT_B) == true)
			{
				//押した状態にする
				bPush = true;
			}
		}

		if (g_aTutorial[nCnt].nType == TUTORIAL_LSTICK)
		{
			if (GetJoyStick() == true)
			{
				//押した状態にする
				bPush = true;
			}
		}

		if (g_bNext == true)
		{

			g_aTutorial[nCnt].pos += g_aTutorial[nCnt].move;

			//場合分け
			switch (g_aTutorial[nCnt].nType)
			{
			case TUTORIAL_ONE:		//1つ目

				g_aTutorial[nCnt].fHeight = SIZE_HEIGHT;	//高さ
				g_aTutorial[nCnt].fWidth = 400.0f;			//横幅

				break;

			case TUTORIAL_ENTER:	//2つ目

				g_aTutorial[nCnt].fHeight = SIZE_HEIGHT;	//高さ
				g_aTutorial[nCnt].fWidth = 400.0f;			//横幅
				break;

			case TUTORIAL_KEY_W:	//3つ目

				g_aTutorial[nCnt].fHeight = SIZE_HEIGHT;	//高さ
				g_aTutorial[nCnt].fWidth = SIZE_WIDTH;		//横幅


				break;

			case TUTORIAL_KEY_A:	//4つ目

				g_aTutorial[nCnt].fHeight = SIZE_HEIGHT;	//高さ
				g_aTutorial[nCnt].fWidth = SIZE_WIDTH;		//横幅
				break;

			case TUTORIAL_KEY_S:	//5つ目

				g_aTutorial[nCnt].fHeight = SIZE_HEIGHT;	//高さ
				g_aTutorial[nCnt].fWidth = SIZE_WIDTH;		//横幅
				break;

			case TUTORIAL_KEY_D:	//6つ目

				g_aTutorial[nCnt].fHeight = SIZE_HEIGHT;	 //高さ
				g_aTutorial[nCnt].fWidth = SIZE_WIDTH;		 //横幅
				break;

			case TUTORIAL_KEY_SPACE://7つ目

				g_aTutorial[nCnt].fHeight = SIZE_HEIGHT;	//高さ
				g_aTutorial[nCnt].fWidth = 150.0f;			//横幅
				break;

			case TUTORIAL_BUTTON_B:	//8つ目

				g_aTutorial[nCnt].fHeight = 80.0f;			//高さ
				g_aTutorial[nCnt].fWidth = 80.0f;			//横幅
				break;

			case TUTORIAL_BUTTON_UP: //9つ目

				g_aTutorial[nCnt].fHeight = SIZE_HEIGHT;	//高さ
				g_aTutorial[nCnt].fWidth = SIZE_WIDTH;		//横幅
				break;

			case TUTORIAL_BUTTON_DOWN: //10個目

				g_aTutorial[nCnt].fHeight = SIZE_HEIGHT;	//高さ
				g_aTutorial[nCnt].fWidth = SIZE_WIDTH;		//横幅
				break;

			case TUTORIAL_BUTTON_RIGHT: //11個目

				g_aTutorial[nCnt].fHeight = SIZE_HEIGHT;	//高さ
				g_aTutorial[nCnt].fWidth = SIZE_WIDTH;		//横幅
				break;

			case TUTORIAL_BUTTON_LEFT: //12個目

				g_aTutorial[nCnt].fHeight = SIZE_HEIGHT;	//高さ
				g_aTutorial[nCnt].fWidth = SIZE_WIDTH;		//横幅
				break;

			case TUTORIAL_KEY_P:	  //13個目

				g_aTutorial[nCnt].fHeight = SIZE_HEIGHT;	//高さ
				g_aTutorial[nCnt].fWidth = SIZE_WIDTH;		//横幅
				break;

			case TUTORIAL_BUTTON_ST:	//14個目

				g_aTutorial[nCnt].fHeight = 80.0f;			//高さ
				g_aTutorial[nCnt].fWidth = 80.0f;			//横幅
				break;

			case TUTORIAL_KEY_E:	//15個目

				g_aTutorial[nCnt].fHeight = SIZE_HEIGHT;	//高さ
				g_aTutorial[nCnt].fWidth = SIZE_WIDTH;		//横幅
				break;

			case TUTORIAL_KEY_Q:	//16個目

				g_aTutorial[nCnt].fHeight = SIZE_HEIGHT;	//高さ
				g_aTutorial[nCnt].fWidth = SIZE_WIDTH;		//横幅
				break;

			case TUTORIAL_BUTTON_RB:	//17個目

				g_aTutorial[nCnt].fHeight = SIZE_HEIGHT;	//高さ
				g_aTutorial[nCnt].fWidth = 80.0f;			//横幅
				break;

			case TUTORIAL_BUTTON_LB:	//18個目

				g_aTutorial[nCnt].fHeight = SIZE_HEIGHT;	//高さ
				g_aTutorial[nCnt].fWidth = 80.0f;			//横幅
				break;

			case TUTORIAL_LSTICK:	//19個目

				g_aTutorial[nCnt].fHeight = 70.0f;			//高さ
				g_aTutorial[nCnt].fWidth = 70.0f;			//横幅
				break;

				break;
			}

			//頂点座標1の設定
			pVtx[0].pos = D3DXVECTOR3(g_aTutorial[nCnt].pos.x - g_aTutorial[nCnt].fWidth * 0.5f, g_aTutorial[nCnt].pos.y - g_aTutorial[nCnt].fHeight * 0.5f, 0.0f);		//1つ目の頂点情報
			pVtx[1].pos = D3DXVECTOR3(g_aTutorial[nCnt].pos.x + g_aTutorial[nCnt].fWidth * 0.5f, g_aTutorial[nCnt].pos.y - g_aTutorial[nCnt].fHeight * 0.5f, 0.0f);		//2つ目の頂点情報
			pVtx[2].pos = D3DXVECTOR3(g_aTutorial[nCnt].pos.x - g_aTutorial[nCnt].fWidth * 0.5f, g_aTutorial[nCnt].pos.y + g_aTutorial[nCnt].fHeight * 0.5f, 0.0f);		//3つ目の頂点情報
			pVtx[3].pos = D3DXVECTOR3(g_aTutorial[nCnt].pos.x + g_aTutorial[nCnt].fWidth * 0.5f, g_aTutorial[nCnt].pos.y + g_aTutorial[nCnt].fHeight * 0.5f, 0.0f);		//4つ目の頂点情報

		}

		if (bPush == true)
		{
			//頂点カラーの設定(赤)
			pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		}
		else
		{
			//頂点カラーの設定(白)
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		
		pVtx += 4;
	}


	//アンロック
	g_pVtxBuffTutorial->Unlock();


	TutorialFlash(TUTORIAL_ENTER);		//点滅

	UpdateTutorial2();		//2個目のチュートリアルの更新

}
//====================
//チュートリアルの描画
//====================
void DrawTutorial(void)
{

	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

//==============================
//チュートリアルの背景描画
//==============================

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTutorialBG, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureTutorialBG);

	//チュートリアルポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

//==============================
//チュートリアルの各種描画
//==============================

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTutorial, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCnt = 0; nCnt < TUTORIAL_MAX; nCnt++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureTutorial[g_aTutorial[nCnt].nType]);

		//チュートリアルポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
	}

	DrawPlayer();		//プレイヤーの描画

	DrawBullet();		//弾の描画

	DrawTutorial2();	//2個目のチュートリアルの描画


}
//================================
//チュートリアルの設定(テクスチャ)
//================================
void SetTutorial(int nType, D3DXVECTOR3 pos)
{
	VERTEX_2D* pVtx;//頂点情報のポインタ

	//頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffTutorial->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < TUTORIAL_MAX; nCnt++)
	{
		if (g_aTutorial[nCnt].bUse == false)
		{
			g_aTutorial[nCnt].nType = nType;
			g_aTutorial[nCnt].pos = pos;
			g_aTutorial[nCnt].bUse = true;

			//場合分け
			switch (g_aTutorial[nCnt].nType)
			{
			case TUTORIAL_ONE:		//1つ目

				g_aTutorial[nCnt].fHeight = 50.0f;	//高さ
				g_aTutorial[nCnt].fWidth = 400.0f;	//横幅

				break;

			case TUTORIAL_ENTER:	//2つ目

				g_aTutorial[nCnt].fHeight = 50.0f;	//高さ
				g_aTutorial[nCnt].fWidth = 400.0f;  //横幅
				break;

			case TUTORIAL_KEY_W:	//3つ目

				g_aTutorial[nCnt].fHeight = 50.0f;	//高さ
				g_aTutorial[nCnt].fWidth = 50.0f;   //横幅


				break;

			case TUTORIAL_KEY_A:	//4つ目

				g_aTutorial[nCnt].fHeight = 50.0f; //高さ
				g_aTutorial[nCnt].fWidth = 50.0f;  //横幅
				break;

			case TUTORIAL_KEY_S:	//5つ目

				g_aTutorial[nCnt].fHeight = 50.0f; //高さ
				g_aTutorial[nCnt].fWidth = 50.0f;  //横幅
				break;

			case TUTORIAL_KEY_D:	//6つ目

				g_aTutorial[nCnt].fHeight = 50.0f; //高さ
				g_aTutorial[nCnt].fWidth = 50.0f;  //横幅
				break;

			case TUTORIAL_KEY_SPACE://7つ目

				g_aTutorial[nCnt].fHeight = 50.0f; //高さ
				g_aTutorial[nCnt].fWidth = 150.0f; //横幅
				break;

			case TUTORIAL_BUTTON_B:	//8つ目

				g_aTutorial[nCnt].fHeight = 80.0f; //高さ
				g_aTutorial[nCnt].fWidth = 80.0f;  //横幅
				break;

			case TUTORIAL_BUTTON_UP: //9つ目

				g_aTutorial[nCnt].fHeight = 50.0f; //高さ
				g_aTutorial[nCnt].fWidth = 50.0f;  //横幅
				break;

			case TUTORIAL_BUTTON_DOWN: //10個目

				g_aTutorial[nCnt].fHeight = 50.0f; //高さ
				g_aTutorial[nCnt].fWidth = 50.0f;  //横幅
				break;

			case TUTORIAL_BUTTON_RIGHT: //11個目

				g_aTutorial[nCnt].fHeight = 50.0f; //高さ
				g_aTutorial[nCnt].fWidth = 50.0f;  //横幅
				break;

			case TUTORIAL_BUTTON_LEFT: //12個目

				g_aTutorial[nCnt].fHeight = 50.0f; //高さ
				g_aTutorial[nCnt].fWidth = 50.0f;  //横幅
				break;

			case TUTORIAL_KEY_P:	  //13個目

				g_aTutorial[nCnt].fHeight = 50.0f; //高さ
				g_aTutorial[nCnt].fWidth = 50.0f;  //横幅
				break;

			case TUTORIAL_BUTTON_ST://14個目

				g_aTutorial[nCnt].fHeight = 80.0f; //高さ
				g_aTutorial[nCnt].fWidth = 80.0f;  //横幅
				break;

			case TUTORIAL_KEY_E:	//15個目

				g_aTutorial[nCnt].fHeight = SIZE_HEIGHT;	//高さ
				g_aTutorial[nCnt].fWidth = SIZE_WIDTH;		//横幅
				break;

			case TUTORIAL_KEY_Q:	//16個目

				g_aTutorial[nCnt].fHeight = SIZE_HEIGHT;	//高さ
				g_aTutorial[nCnt].fWidth = SIZE_WIDTH;		//横幅
				break;

			case TUTORIAL_BUTTON_RB:	//17個目

				g_aTutorial[nCnt].fHeight = SIZE_HEIGHT;	//高さ
				g_aTutorial[nCnt].fWidth = 80.0f;			//横幅
				break;
				
			case TUTORIAL_BUTTON_LB:	//18個目

				g_aTutorial[nCnt].fHeight = SIZE_HEIGHT;	//高さ
				g_aTutorial[nCnt].fWidth = 80.0f;			//横幅
				break;

			case TUTORIAL_LSTICK:	//19個目

				g_aTutorial[nCnt].fHeight = 70.0f;			//高さ
				g_aTutorial[nCnt].fWidth = 70.0f;			//横幅
				break;

				break;
			}

			//頂点座標1の設定
			pVtx[0].pos = D3DXVECTOR3(g_aTutorial[nCnt].pos.x - g_aTutorial[nCnt].fWidth * 0.5f, g_aTutorial[nCnt].pos.y - g_aTutorial[nCnt].fHeight * 0.5f, 0.0f);//1つ目の頂点情報
			pVtx[1].pos = D3DXVECTOR3(g_aTutorial[nCnt].pos.x + g_aTutorial[nCnt].fWidth * 0.5f, g_aTutorial[nCnt].pos.y - g_aTutorial[nCnt].fHeight * 0.5f, 0.0f);//2つ目の頂点情報
			pVtx[2].pos = D3DXVECTOR3(g_aTutorial[nCnt].pos.x - g_aTutorial[nCnt].fWidth * 0.5f, g_aTutorial[nCnt].pos.y + g_aTutorial[nCnt].fHeight * 0.5f, 0.0f);//3つ目の頂点情報
			pVtx[3].pos = D3DXVECTOR3(g_aTutorial[nCnt].pos.x + g_aTutorial[nCnt].fWidth * 0.5f, g_aTutorial[nCnt].pos.y + g_aTutorial[nCnt].fHeight * 0.5f, 0.0f);//4つ目の頂点情報

			break;
		}

		pVtx += 4;
	}

	//アンロック
	g_pVtxBuffTutorial->Unlock();
}
//==========================
//点滅の処理
//==========================
void TutorialFlash(int nType)
{
	VERTEX_2D* pVtx = 0;		//頂点情報のポインタ

	//頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffTutorial->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < TUTORIAL_MAX; nCnt++)
	{
		if (g_aTutorial[nCnt].bUse == true && g_aTutorial[nCnt].state == TUTORIALSTATE_FLASH)
		{
			g_nCntTutoAnim++;			//カウンターを加算

			if (g_nCntTutoAnim == 5)	//5の時
			{
				//頂点カラーの設定
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.2f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.2f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.2f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.2f);

			}

			else if (g_nCntTutoAnim == 10)	//10の時
			{
				//頂点カラーの設定
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				g_nCntTutoAnim = 0;		//初期化に戻す
			}
		}
		pVtx += 4;
	}
	//アンロック
	g_pVtxBuffTutorial->Unlock();
}
//==========================
//次の画面の取得
//==========================
bool GetIsNext(void)
{
	return g_bNext;
}
//==========================
//次の画面への移動量の取得
//==========================
int GetTutorialCountMove(void)
{
	return g_nCntMove;
}
