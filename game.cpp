//---------------------------
//
//ゲーム処理
//Author:NISHIO ASUMA
//
//---------------------------

#include "game.h"
#include "Player.h"
#include "background.h"
#include "input.h"
#include "bullet.h"
#include "explosion.h"
#include "enemy.h"
#include "fade.h"
#include "score.h"
#include "item.h"
#include "title.h"
#include "tutorial.h"
#include "effect.h"
#include "pause.h"
#include "wave.h"
#include "edit.h"
#include "ranking.h"
#include "rankscore.h"
#include "particle.h"
#include "Timer.h"
#include "sound.h"

//グローバル
GAMESTATE g_gameState = GAMESTATE_NONE;		//ゲーム状態
int g_nCounterGameState = 0;				//状態管理カウンター
bool g_bPause = false;						//ポーズ中かどうか

//===================
//ゲームの初期化
//===================
void InitGame(void)
{
	InitBackground();		//背景

	InitBullet();			//弾

	InitPlayer();			//プレイヤー

	InitWave();				//ウェーブ

	InitEnemy();			//敵

	InitExplosion();		//爆発

	InitScore();			//スコア

	InitItem();				//アイテム

	InitEffect();			//エフェクト

	InitParticle();			//パーティクル

	InitPause();			//ポーズ

	InitTimer();			//タイマー

	InitPlayerLifeBar();	//ライフバー

	g_gameState = GAMESTATE_NORMAL;	//通常状態
	g_nCounterGameState = 0;
	g_bPause = false;				//ポーズ画面の初期化(ポーズ解除)

	LoadWave();						//ウェーブ読み込み

	PlaySound(SOUND_LABEL_GAME);
}
//===================
//ゲームの終了
//===================
void UninitGame(void)
{
	StopSound();

	SaveScore();			//スコアを保存

	UninitBackground();		//背景

	UninitBullet();			//弾

	UninitPlayer();			//プレイヤー

	UninitEnemy();			//敵の終了

	UninitExplosion();		//爆発

	UninitScore();			//スコア終了

	UninitItem();			//アイテム

	UninitEffect();			//エフェクト

	UninitParticle();		//パーティクル

	UninitPause();			//ポーズ

	UninitTimer();			//タイマー

	UninitPlayerLifeBar();	//ライフバー

}
//===================
//ゲームの更新
//===================
void UpdateGame(void)
{
	int nNum;//ローカル変数

	//プレイヤーの取得
	Player* pPlayer = GetPlayer();

	//敵の取得
	nNum = GetNumEnemy();

	//ウェーブを取得
	int nWave = GetWave();

	//タイマーを取得
	int nTime = GetTimer();

	if (nNum <= 0)
	{
		//ウェーブを読み込む
		LoadWave();
	}

	//ウェーブの終了判定を取得
	bool bFinish = GetFinish();


	if ((pPlayer->bDisp == false || bFinish == true || nTime <= 0) && g_gameState != GAMESTATE_NONE)
	{
		g_gameState = GAMESTATE_END;  //終了状態
	}
	switch(g_gameState)
	{
	case GAMESTATE_NORMAL://通常状態
		break;

	case GAMESTATE_END:
  		g_nCounterGameState++;
		if (g_nCounterGameState >= 30)
		{
			//カウンターを初期化
			g_nCounterGameState = 0;

			//1秒経過
			g_gameState = GAMESTATE_NONE;//何もしていない状態
			//画面設定
			SetFade(MODE_RESULT);

			//読み込み
			ResetRanking();

			//ランキングの設定
			SetRanking(GetScore());
		}
		break;
	}

	//ポーズ画面
	if (KeyboardTrigger(DIK_P) == true || JoypadTrigger(JOYKEY_START))
	{
		//pが押された or startボタン
		g_bPause = g_bPause ? false : true;
	}

#ifdef _DEBUG

	if (KeyboardTrigger(DIK_F1) == true)
	{

		SaveEdit();//エディット画面

	}

#endif // DEBUG

	if (g_bPause == true)
	{
		//ポーズ中
		//ポーズの更新処理
		UpdatePause();
	}
	else 
	{
		//ポーズ中で無ければ
		UpdateBackground();			//背景

		UpdatePlayer();				//プレイヤー

		UpdateBullet();				//弾

		UpdateEnemy();				//敵

		UpdateExplosion();			//爆発

		UpdateScore();				//スコア更新

		UpdateItem();				//アイテム

		UpdateEffect();				//エフェクト

		UpdateParticle();			//パーティクル

		UpdateTimer();				//タイマー

		UpdatePlayerLifeBar();		//ライフバー
	}
}
//===================
//ゲームの描画
//===================
void DrawGame(void)
{
	//背景
	DrawBackground();

	//アイテム
	DrawItem();

	//弾
	DrawBullet();

	//プレイヤー
	DrawPlayer();

	//敵
	DrawEnemy();

	//爆破
	DrawExplosion();

	//スコア
	DrawScore();

	//エフェクト
	DrawEffect();

	//パーティクル
	DrawParticle();

	//タイマー
	DrawTimer();

	//ライフバー
	DrawPlayerLifeBar();

	if (g_bPause == true)
	{
		//ポーズ中
		//ポーズの描画
		DrawPause();
	}
}
//=====================
//ポーズの有効無効設定
//=====================
void SetEnablePause(bool bPause)
{
	g_bPause = bPause;
}
//=====================
//げーむ状態設定
//=====================
void SetGamestate(GAMESTATE state)
{
	g_gameState = state;
	g_nCounterGameState = 0;
}
//=====================
//げーむ状態取得
//=====================
GAMESTATE GetGameState(void)
{
	return g_gameState;
}
