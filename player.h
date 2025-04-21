//---------------------------
//
//プレイヤー処理
//Author:NISHIO ASUMA
//
//---------------------------

#ifndef _PLAYER_H_ //このマクロ定義がされてなかったら
#define _PLAYER_H_ //2重インクルード防止のマクロ定義


#define SPLIT_U (1.0f)				//横の分割数
#define SPLIT_V (1.0f)				//縦の分割数
#define MAX_ANIMATION_TIME (10)		//最大アニメーション時間
#define MAX_ANIMATION_PATTERN (8)	//最大パターン数
#define MAX_SIZE_X (60.0f)			//横のサイズ(自機)
#define MAX_SIZE_Y (60.0f)			//縦のサイズ(自機)

//プレイヤーの状態
typedef enum
{
	PLAYERSTATE_APPEAR = 0,			//無敵状態
	PLAYERSTATE_NORMAL,				//通常状態
	PLAYERSTATE_DAMAGE,				//ダメージ状態
	PLAYERSTATE_DEATH,				//死亡状態
	PLAYERSTATE_MAX
}PLAYERSTATE;

//プレイヤー構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;				//位置
	D3DXVECTOR3 move;				//移動量
	D3DXVECTOR3 rot;				//z軸
	float fLength;					//対角線の長さ
	float fAngle;					//対角線の角度
	bool bHit;						//弾が当たった
	int nLife;						//自機の体力
	int nCntAnimState;				//状態をカウント
	int nCntState;					//カウンター
	PLAYERSTATE state;				//プレイヤーの状態
	bool bDisp;						//表示するかしないか
	bool bDeth;
}Player;

//プロトタイプ宣言
void InitPlayer(void);				//プレイヤーの初期化
void UninitPlayer(void);			//プレイヤーの終了
void UpdatePlayer(void);			//プレイヤーの更新処理
void DrawPlayer(void);				//プレイヤーの描画処理
void HitPlayer(int nDamage);		//弾がプレイヤーに当たった時の処理(自機の当たり判定)
void GetKeyPlayer(void);			//キー入力情報を取得
Player* GetPlayer(void);			//プレイヤーを取得
void GetJoypadPlayer(void);			//ジョイパッド情報を取得
void StickState(void);				//スティック情報を取得

void InitPlayerLifeBar(void);		//Lifeバーの初期化
void UninitPlayerLifeBar(void);		//Lifeバーの初期化
void UpdatePlayerLifeBar(void);		//Lifeバーの更新
void DrawPlayerLifeBar(void);		//Lifeバーの描画


#endif