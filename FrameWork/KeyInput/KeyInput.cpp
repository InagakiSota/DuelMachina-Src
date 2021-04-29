#include <memory.h>

// キーバッファの数
#define KEY_INPUT_BUFFER		256 

// キーが押されたかの情報を保存
char g_isPressKeys[KEY_INPUT_BUFFER] = {};
//int g_isPressKeys = 0;

// 前のフレームでキーが押されたかの情報を保存
char g_isPrevPressKeys[KEY_INPUT_BUFFER] = {};
//int g_isPrevPressKeys;

/*-------------------------------
キー処理の更新
引数	：なし
戻り値	：点数
-------------------------------*/
void KeyInputUpdate(void)
{
	// 前のフレームに押されたキーを保存
	memcpy(g_isPrevPressKeys, g_isPressKeys, sizeof(char) * KEY_INPUT_BUFFER);
	// g_isPrevPressKeys = g_isPressKeys;

	// キーの押下状態を保存
	GetHitKeyStateAll(g_isPressKeys);
	//g_isPressKeys = GetJoypadInputState(DX_INPUT_KEY_PAD1);
}

/*-------------------------------
キーが押し込まれた瞬間かどうか
引数	：なし
戻り値	：真偽値
-------------------------------*/
int  IsKeyDown(int key)
{
	if (g_isPrevPressKeys[key] == FALSE && g_isPressKeys[key] == TRUE)
	//if ((g_isPressKeys & key) &&  !(g_isPrevPressKeys & key))
	{
		return TRUE;
	}

	return FALSE;
}

/*-------------------------------
キーが押され続けているかどうか
引数	：なし
戻り値	：真偽値
-------------------------------*/
int  IsKeyPress(int key)
{
	if (g_isPressKeys[key] == TRUE)
	//if ((g_isPressKeys & key))
	{
		return TRUE;
	}

	return FALSE;
}

/*-------------------------------
キーが離されたかどうか
引数	：なし
戻り値	：真偽値
-------------------------------*/
int  IsKeyUp(int key)
{
	if (g_isPrevPressKeys[key] == TRUE && g_isPressKeys[key] == FALSE)
	//if (!(g_isPressKeys & key) && (g_isPrevPressKeys & key))
	{
		return TRUE;
	}

	return FALSE;
}