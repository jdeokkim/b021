/*
    Copyright (c) 2022 Jaedeok Kim (https://github.com/jdeokkim)

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

#include "b021.h"

/* | `game` 모듈 변수... | */

/* 블랙잭 보드. */
static Asset *astBoard;

/* 블랙잭 카드. */
static Asset *astCards;

/* | `game` 모듈 함수... | */

/* 게임 플레이 장면을 초기화한다. */
void InitGameScene(void) {
    astBoard = GetAsset(0);
    astCards = GetAsset(1);
}

/* 게임 플레이 장면을 업데이트한다. */
void UpdateGameScene(void) {
    ClearBackground(BLACK);

    // 블랙잭 보드를 그린다.
    DrawTextureV(astBoard->data.texture, (Vector2) { 0.0f }, WHITE);

#ifdef _DEBUG
    DrawFPS(8, 8);
#endif
}

/* 게임 플레이 장면을 종료한다. */
int FinishGameScene(void) {
    /* TODO: ... */
}