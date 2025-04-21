//---------------------------
//
//敵の処理
//Author:NISHIO ASUMA
//
//---------------------------

#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"

//マクロ
#define MAX_ENEMY (128)				// 敵の最大数
#define MAX_ENEMY_SIZE (25.0f)		// 敵のサイズ

//敵の状態の列挙型
typedef enum
{
	ENEMYSTATE_NORMAL = 0,			// 通常状態
	ENEMYSTATE_DAMAGE,				// ダメージ状態
	ENEMYSTATE_MAX
}ENEMYSTATE;

//3タイプ目の敵の列挙型
typedef enum
{
	ENEMY_TO_RIGHT = 0,
	ENEMY_TO_UNDER,
	ENEMY_TO_LEFT,
	ENEMY_TO_TOP,
	ENEMY_TO_MAX
}ENEMYTHREESTATE;

//敵の種類情報(列挙型)
typedef enum
{
	ENEMYTYPE_OWN = 0,	//1番目
	ENEMYTYPE_TWO,		//2番目
	ENEMYTYPE_THREE,	//3番目
	ENEMYTYPE_FOUR,		//4番目
	ENEMYTYPE_FIVE,		//5番目
	ENEMYTYPE_SIX,		//ボス
	ENEMYTYPE_MAX
}ENEMYTYPE;

static const char* ENEMY_TEXTURE[ENEMYTYPE_MAX] =   //敵のテクスチャの設定
{
	"data\\TEXTURE\\enemy100.png",
	"data\\TEXTURE\\enemy101.png",
	"data\\TEXTURE\\enemy102.png",
	"data\\TEXTURE\\enemy103.png",
	"data\\TEXTURE\\enemy104.png",
	"data\\TEXTURE\\enemy105.png",
};

//構造体
typedef struct
{
	D3DXVECTOR3 pos;						//座標
	D3DXVECTOR3 move;						//移動量
	D3DXVECTOR3 rot;						//回転量
	ENEMYSTATE state;						//敵の状態
	ENEMYTHREESTATE Threestate;				//3タイプ目の敵の情報
	int nCounterState;						//状態管理カウンター
	float fLength;							//対角線
	float fAngle;							//角度
	int nType;								//敵の種類
	bool bUse;								//使用しているか
	int nLife;								//敵の体力
	int nBulletCounter;						//敵の弾の数をカウント
	bool bRight;							//フラグ
	bool bTop;								//フラグ
}Enemy;

//プロト
void InitEnemy(void);						//敵の初期化
void UninitEnemy(void);						//敵の終了
void UpdateEnemy(void);						//敵の更新
void DrawEnemy(void);						//敵の描画
void SetEnemy(D3DXVECTOR3 pos,int nType);	//敵の設定処理
void HitEnemy(int nCntEnemy, int nDamage);	//敵の当たり判定
Enemy* GetEnemy(void);						//判定を取得
int GetNumEnemy(void);						//敵の総数の取得

#endif

