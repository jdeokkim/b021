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

/* | `scene` 모듈 변수... | */

/* 게임의 현재 장면. */
static SceneType sceneType = ST_INIT;

/* | `scene` 모듈 함수... | */

/* 게임 장면을 변경한다. */
void LoadScene(SceneType st) {
    if (sceneType == st) return;

    switch (st) {
        case ST_INIT:
            break;

        case ST_LOADING:
            InitLoadingScene();

            break;

        case ST_GAME:
            InitGameScene();

            break;
    }

    sceneType = st;
}

/* 게임 화면을 업데이트한다. */
void UpdateGame(void) {
    BeginDrawing();

    switch (sceneType) {
        case ST_INIT:
            SetExitKey(KEY_NULL);

            LoadScene(ST_LOADING);

            break;

        case ST_LOADING:
            UpdateLoadingScene();

            if (FinishLoadingScene())
                LoadScene(ST_GAME);

            break;

        case ST_GAME:
            UpdateGameScene();
            
            break;
    }

    EndDrawing();
}