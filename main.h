//---------------------------
//
//DirectX シューティング
//Author:NISHIO ASUMA
//
//---------------------------
#ifndef _MAIN_H_ //このマクロ定義がされてなかったら
#define _MAIN_H_ //2重インクルード防止のマクロ定義

#include <windows.h>
#include "d3dx9.h"					//描画処理に必要
#define DIRECTINPUT_VERSION (0x0800)//ビルド時警告対処用のマクロ
#include "dinput.h"					//入力処理に必要
#include "Xinput.h"					//ジョイパッド処理に必要
#include "xaudio2.h"

//ライブラリのリンク
#pragma comment(lib,"d3d9.lib")		//描画処理に必要
#pragma comment(lib,"d3dx9.lib")	//[d3d9.lib]の拡張ライブラリ
#pragma comment(lib,"dxguid.lib")	//DirectXコンポーネント(部品)使用に必要
#pragma comment(lib,"dinput8.lib")	//入力処理に必要
#pragma comment(lib,"xinput.lib")	//ジョイパッド処理に必要
#pragma comment(lib,"Winmm.lib")

//マクロ定義
#define CLASS_NAME "WindowClass"          //ウインドウクラスの名前
#define WINDOW_NAME "THE SHOT"			  //ウインドウの名前(キャプションに表示)
#define SCREEN_WIDTH (1280)               //ウインドウの幅
#define SCREEN_HEIGHT (720)               //ウインドウの高さ
#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1) //頂点フォーマット[2D]


//頂点情報[2D]の構造体
typedef struct
{
	D3DXVECTOR3 pos;			//頂点座標
	float rhw;					//座標変換用係数(1.0fで固定)
	D3DCOLOR col;				//頂点カラー
	D3DXVECTOR2 tex;			//テクスチャ座標
}VERTEX_2D;

//画面の種類
typedef enum
{
	MODE_TITLE = 0,				//タイトル
	MODE_TUTORIAL,				//チュートリアル
	MODE_GAME,					//ゲーム
	MODE_RESULT,				//リザルト
	MODE_EDIT,					//エディット
	MODE_RANKING,				//ランキング
	MODE_MAX,
}MODE;


//プロトタイプ宣言
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);	//ウィンドウプロシージャ
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);							//初期化処理
void Uninit(void);																	//終了処理
void Update(void);																	//更新処理
void Draw(void);																	//描画処理
LPDIRECT3DDEVICE9 GetDevice(void);													//デバイスの取得
void SetMode(MODE mode);															//モードの設定
MODE GetMode(void);																	//モードの取得
void DrawFPS(void);																	//デバッグ表示
void DrawEnemySet(int g_nNumEnemy);													//敵の総数の表示
void ToggleFullscreen(HWND hWnd);													// ウィンドウをフルスクリーンにする方法

#endif