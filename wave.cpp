//---------------------------
//
//wave処理(読み込み)
//Author:NISHIO ASUMA
//
//---------------------------

#include "wave.h"
#include "input.h"
#include <stdio.h>
#include <string.h>
#include "enemy.h"

//マクロ
#define MAX_WORD (4096)		//最大文字数

//構造体
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	int nType;			//種類
	bool bUse;			//使用状態
}LoadInfo;

//グローバル
LoadInfo g_aInfo[MAX_ENEMY];		//構造体情報
int g_nCntEnemy;					//敵をカウント
int g_nWave;						//ウェーブを管理
bool g_bFinish;						//ウェーブが終わったか

//==============================
// 初期化
//==============================
void InitWave(void)
{
	//構造体の初期化
	for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++)
	{
		g_aInfo[nCnt].bUse = false;
		g_aInfo[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aInfo[nCnt].nType = 0;
	}

	 g_nCntEnemy = 0;		//敵をカウント
	 g_nWave = 0;			//ウェーブを管理
	 g_bFinish = false;		//ウェーブの終了判定

}
//==============================
//ウェーブ処理(読み込み)
//==============================
void LoadWave(void)
{
	FILE* pFile = NULL;		//ファイルポインタ
	int nType;				//タイプ
	D3DXVECTOR3 pos;		//座標

	//敵の取得
	g_nCntEnemy = GetNumEnemy();

	//ウェーブ関係
	switch (g_nWave)
	{

	case 0:
		//初期ウェブ
		//ファイル開く
		pFile = fopen(WAVEPATH_BIN, "rb");
		break;

	case 1:

		//ファイル開く
		pFile = fopen(WAVEPATH_BIN_1, "rb");

		break;

	case 2:
		//ファイルを開く
		pFile = fopen(WAVEPATH_BIN_2, "rb");

		break;

	case 3:
		//ファイルを開く
		pFile = fopen(WAVEPATH_BIN_3, "rb");

		break;

	case 4:
		//ファイルを開く
		pFile = fopen(WAVEPATH_BIN_4, "rb");

		break;

	case 5:
		//ファイルを開く
		pFile = fopen(WAVEPATH_BIN_5, "rb");

		break;

	case 6:
		//ファイルを開く
		pFile = fopen(WAVEPATH_BIN_6, "rb");

		break;

	case 7:
		//ファイルを開く
		pFile = fopen(WAVEPATH_BIN_7, "rb");

		break;

	case 8:
		//ファイルを開く
		pFile = fopen(WAVEPATH_BIN_8, "rb");

		break;

	case 9:
		//ファイルを開く
		pFile = fopen(WAVEPATH_BIN_9, "rb");

		break;

	default:
		pFile = NULL;
		g_bFinish = true;	//ウェーブの終了
		break;
	}

	//開けたら
	if (pFile != NULL)
	{
		fread(&g_nCntEnemy, sizeof(int), 1, pFile);					//敵の数
		fread(&g_aInfo[0], sizeof(LoadInfo), g_nCntEnemy, pFile);	//敵の読み込み

		for (int nCnt = 0; nCnt < g_nCntEnemy; nCnt++)
		{
			SetEnemy(g_aInfo[nCnt].pos, g_aInfo[nCnt].nType);		//敵の設定
		}

		//ファイルを閉じる
		fclose(pFile);

		//ウェーブのカウントアップ
		g_nWave++;

	}
	else
	{
		//開けなかった時
		return;
	}

}
//====================
//ウェーブの値を返す
//====================
int GetWave(void)
{
	return g_nWave;
}
//====================
//ウェーブの終了判定を返す
//====================
bool GetFinish(void)
{
	return g_bFinish;
}

//while (1)
//{
//	char aString[MAX_WORD];//読み込む文字数

//	//ファイルから読み込む
//	fscanf(pFile, "%s", &aString[0]);

//	//SETENEMYを読み取ったら
//	if (strcmp(aString, "SETENEMY") == 0)
//	{
//		while (1)
//		{
//			//ファイルから読み込む
//			fscanf(pFile, "%s", &aString[0]);

//			if (strcmp(aString, "ENEMYTYPE") == 0)
//			{
//				//敵の種類
//				fscanf(pFile, "%d", &nType);

//			}
//			else if (strcmp(aString, "POS") == 0)
//			{
//				//敵の座標
//				fscanf(pFile, "%f", &pos.x);
//				fscanf(pFile, "%f", &pos.y);
//				fscanf(pFile, "%f", &pos.z);

//			}
//			else if (strcmp(aString, "END_SETENEMY") == 0)
//			{
//				//END_SETENEMYを読み取ったら
//				SetEnemy(pos, nType);
//				break;
//			}
//		}
//	}
//	//データを代入
//	if (strcmp(aString, "END_SCRIPT") == 0)
//	{
//		
//	}

//}

