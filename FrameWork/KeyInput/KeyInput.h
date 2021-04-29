#pragma once

// キー処理の更新
void KeyInputUpdate(void);

// キーが押し込まれたかどうか
int  IsKeyDown(int key);

// キーが押され続けているかどうか
int  IsKeyPress(int key);

// キーが離されたかどうか
int  IsKeyUp(int key);