//==============================
//
//ランキングスコア処理
//Author:NISHIO ASUMA
//
//==============================


#include "rankscore.h"
#include "ranking.h"
#include "input.h"
#include "fade.h"
#include <stdio.h>
#include "score.h"
#include "sound.h"

//マクロ
#define MAX_RANK    (5)						//表示順位数を設定(5位まで)
#define MAX_DIGIT	(8)						//桁数
#define MAX_POLYGON (MAX_RANK * MAX_DIGIT)	//最大ポリゴン数
#define POLYGON_SIZEWIDTH (68.0f)			//ポリゴン幅
#define POLYGON_SIZEHIGHT (68.0f)			//ポリゴン高さ
#define POLYGON_INTERVAL_Y (20.0f)			//スコア表示の間隔空ける高さ

//ランキング構造体
typedef struct
{
	D3DXVECTOR3 pos;			//座標
	int nScore;					//スコア
}RankScore;

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureRankScore = NULL;				//テクスチャのポインタ(スコア)
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRankScore = NULL;			//頂点バッファのポインタ(スコア)
RankScore g_aRankScore[MAX_RANK];							//ランキングスコア情報
int g_nRankUpdate = -1;										//更新ランクNo
int g_nTimeRanking;											//ランキング画面表示タイマー
int g_nCntscoreAnim;										//スコアのアニメーションカウンター

//====================
//初期化
//====================
void InitRankingScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み(スコア)
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\number001.png",
		&g_pTextureRankScore);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_POLYGON,
		D3DUSAGE_WRITEONLY, FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRankScore,
		NULL);

	g_nTimeRanking = 0;     //ランキング画面表示タイマー
	g_nCntscoreAnim = 0;    //スコアのアニメーションカウンター

	VERTEX_2D* pVtx;		//頂点情報のポインタ

	//頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffRankScore->Lock(0, 0, (void**)&pVtx, 0);

	//順位数分回す(5)
	for (int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
		g_aRankScore[nCntRank].pos = D3DXVECTOR3(500.0f,170.0f + (POLYGON_SIZEHIGHT * nCntRank) + (POLYGON_INTERVAL_Y * nCntRank), 0.0f);
		g_aRankScore[nCntRank].nScore = 0;

		//桁数分回す(8)
		for (int nCntScore = 0; nCntScore < MAX_DIGIT; nCntScore++)
		{
			//頂点座標1の設定
			pVtx[0].pos.x = g_aRankScore[nCntRank].pos.x - POLYGON_SIZEWIDTH / 2 + (POLYGON_SIZEWIDTH * nCntScore);	  //1つ目の頂点情報
			pVtx[0].pos.y = g_aRankScore[nCntRank].pos.y - POLYGON_SIZEHIGHT / 2;									  //1つ目の頂点情報
			pVtx[0].pos.z = 0.0f;																					  //1つ目の頂点情報

			pVtx[1].pos.x = g_aRankScore[nCntRank].pos.x + POLYGON_SIZEWIDTH / 2 + (POLYGON_SIZEWIDTH * nCntScore);	  //2つ目の頂点情報
			pVtx[1].pos.y = g_aRankScore[nCntRank].pos.y - POLYGON_SIZEHIGHT / 2;									  //2つ目の頂点情報
			pVtx[1].pos.z = 0.0f;																					  //2つ目の頂点情報

			pVtx[2].pos.x = g_aRankScore[nCntRank].pos.x - POLYGON_SIZEWIDTH / 2 + (POLYGON_SIZEWIDTH * nCntScore);	  //3つ目の頂点情報
			pVtx[2].pos.y = g_aRankScore[nCntRank].pos.y + POLYGON_SIZEHIGHT / 2;									  //3つ目の頂点情報
			pVtx[2].pos.z = 0.0f;																					  //3つ目の頂点情報

			pVtx[3].pos.x = g_aRankScore[nCntRank].pos.x + POLYGON_SIZEWIDTH / 2 + (POLYGON_SIZEWIDTH * nCntScore);	  //4つ目の頂点情報
			pVtx[3].pos.y = g_aRankScore[nCntRank].pos.y + POLYGON_SIZEHIGHT / 2;									  //4つ目の頂点情報
			pVtx[3].pos.z = 0.0f;																					  //4つ目の頂点情報

			//rhwの設定(1.0fで固定)
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f,1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f,1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f,1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f,1.0f, 1.0f);

			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

			pVtx += 4;

		}

	}
	//アンロック
	g_pVtxBuffRankScore->Unlock();

}
//====================
//終了
//====================
void UninitRankingScore(void)
{
	//スコア
	//テクスチャの破棄
	if (g_pTextureRankScore != NULL)
	{
		g_pTextureRankScore->Release();
		g_pTextureRankScore = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffRankScore != NULL)
	{
		g_pVtxBuffRankScore->Release();
		g_pVtxBuffRankScore = NULL;
	}

}
//====================
//更新
//====================
void UpdateRankingScore(void)
{

	static int nCounter{};

	if (g_nRankUpdate != -1)
	{
		nCounter++;   //加算

		//該当スコアを点滅
		if (nCounter == 5)
		{
			VERTEX_2D* pVtx{};       //頂点情報のポインタ

			//頂点バッファをロックし,頂点情報へのポインタを取得
			g_pVtxBuffRankScore->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += (32 * g_nRankUpdate);

			for (int nCnt = 0; nCnt < MAX_DIGIT; nCnt++)
			{
				pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 120);
				pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 120);
				pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 120);
				pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 120);
				pVtx += 4;
			}

			g_pVtxBuffRankScore->Unlock();
		}
		else if (nCounter == 10)
		{
			VERTEX_2D* pVtx{};       //頂点情報のポインタ

			//頂点バッファをロックし,頂点情報へのポインタを取得
			g_pVtxBuffRankScore->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += (32 * g_nRankUpdate);

			for (int nCnt1 = 0; nCnt1 < MAX_DIGIT; nCnt1++)
			{
				pVtx[0].col = D3DCOLOR_RGBA(255, 0, 0, 255);
				pVtx[1].col = D3DCOLOR_RGBA(255, 0, 0, 255);
				pVtx[2].col = D3DCOLOR_RGBA(255, 0, 0, 255);
				pVtx[3].col = D3DCOLOR_RGBA(255, 0, 0, 255);

				pVtx += 4;
			}

			g_pVtxBuffRankScore->Unlock();

			nCounter = 0;
		}

	}

	g_nTimeRanking++;	//タイマーを加算
	if (g_nTimeRanking >= 600 || KeyboardTrigger(DIK_RETURN) == true || JoypadTrigger(JOYKEY_A) == true)
	{
		//タイトルにいく
		SetFade(MODE_TITLE);
	}

	//アンロック
	g_pVtxBuffRankScore->Unlock();

}
//====================
//描画
//====================
void DrawRankingScore(void)
{

	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffRankScore, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCnt = 0; nCnt < MAX_POLYGON; nCnt++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureRankScore);

		//ポリゴンの描画(1種類目)
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
	}

}
//================================
//ランキングの設定処理
//================================
void SetRanking(int nScore)
{
	int Temp{};		// 一時的に保持

	g_nRankUpdate = -1;	// 初期化

	// 降順で入れ替える
	for (int nCnt1 = 0; nCnt1 < MAX_RANK - 1; nCnt1++)	// 要素1
	{
		for (int nCnt2 = nCnt1 + 1; nCnt2 < MAX_RANK; nCnt2++)	// 要素2
		{
			if (g_aRankScore[nCnt1].nScore < g_aRankScore[nCnt2].nScore)
			{ // 要素2が要素1よりも大きかった
				// 入れ替え処理
				Temp = g_aRankScore[nCnt1].nScore;							// 保存用に要素1を入れる
				g_aRankScore[nCnt1].nScore = g_aRankScore[nCnt2].nScore;	// 要素1に要素2を入れる
				g_aRankScore[nCnt2].nScore = Temp;							// 要素2に保存用 (要素1) を入れる
			}
		}
	}

	// 再度降順で入れ替える (今回のスコア込みでソートする)
	for (int nCntScore = MAX_RANK - 1; nCntScore >= 0; nCntScore--)
	{
		if (g_aRankScore[nCntScore].nScore < nScore)
		{
			Temp = g_aRankScore[nCntScore].nScore;

			g_aRankScore[nCntScore].nScore = nScore;

			if (nCntScore != MAX_RANK - 1)
			{ 
				// 最下位じゃなければ入れ替える (最下位は入れ替える必要がない，入れ替える器がない)
				g_aRankScore[nCntScore + 1].nScore = Temp;
			}
			g_nRankUpdate = nCntScore;	// 順位(ランク)を更新
		}
	}

	// スコアの書き出し
	WriteScore();
}
//================================
//ランキングのリセット処理
//================================
void ResetRanking(void)
{
	//ファイル
	FILE* pFile;

	//ファイル開く
	pFile = fopen("data\\ranking.txt", "r");

	if (pFile != NULL)
	{
		for (int nCnt = 0; nCnt < MAX_RANK; nCnt++)
		{
			//ファイル開けたら
			fscanf(pFile, "%d", &g_aRankScore[nCnt].nScore);
		}
	}
	else
	{
		//開けなかった時
		//メッセージBOXの表示
		MessageBox(NULL, "開けません", "エラー", MB_OK);

		return;
	}
	//ファイルを閉じる
	fclose(pFile);

}
//==================================
//ランキングのスコアのテクスチャ処理
//==================================
void SetRankingTexture(void)
{
	int aPosTexU[MAX_DIGIT] = {};		//桁数分の数字を格納
	int aData = 10000000;
	int aData2 = 1000000;

	VERTEX_2D* pVtx;			//頂点情報のポインタ

	//頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffRankScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_RANK; nCnt++)
	{
		for (int nCntscore = 0; nCntscore < MAX_DIGIT; nCntscore++)
		{
			if (nCntscore == 0)
			{
				//0番目の時
				aPosTexU[0] = g_aRankScore[nCnt].nScore / aData;
			}
			else
			{
				//0番目以外の時
				aPosTexU[nCntscore] = g_aRankScore[nCnt].nScore % aData / aData2;
				aData = aData / 10;
				aData2 = aData2 / 10;
			}

			//テクスチャ設定
			pVtx[0].tex = D3DXVECTOR2(0.0f + (0.1f * aPosTexU[nCntscore]), 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f + (0.1f * aPosTexU[nCntscore]), 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f + (0.1f * aPosTexU[nCntscore]), 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f + (0.1f * aPosTexU[nCntscore]), 1.0f);

			pVtx += 4;
		}

		aData = 10000000;
		aData2 = 1000000;

	}

	//アンロック
	g_pVtxBuffRankScore->Unlock();
}
//=====================
//スコアを書き出し
//=====================
void WriteScore(void)
{
	//ファイルポインタ
	FILE* pFile;

	//ファイルを開く
	pFile = fopen("data\\ranking.txt", "w");

	//ファイルを開けたら
	if (pFile != NULL)
	{
		for (int nCnt = 0; nCnt < MAX_RANK; nCnt++)
		{
			//配置した数だけ周回
			fprintf(pFile, "%d\n", g_aRankScore[nCnt].nScore);
		}
	}
	else
	{
		//ファイルが開けなかった時
		//メッセージBOXの表示
		MessageBox(NULL, "開けません", "エラー", MB_OK);

		return;
	}
	//ファイルを閉じる
	fclose(pFile);

}

