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

#include <stdbool.h>
#include <stdio.h>

#include "b021.h"

/* | `asset` 모듈 변수... | */

/* 게임 리소스 파일의 목록. */
static Asset assets[] = {
    { .type = AT_TEXTURE, .path = "res/images/board.png"        },
    { .type = AT_TEXTURE, .path = "res/images/cards-large.png"  },
    { .type = AT_FONT,    .path = "res/fonts/neodgm-16pt.fnt"   },
    { .type = AT_FONT,    .path = "res/fonts/neodgm-32pt.fnt"   }
};

/* 게임 리소스 파일의 개수. */
static int assetCount = sizeof(assets) / sizeof(*assets);

/* | `asset` 모듈 함수... | */

/* `i + 1`번째 게임 리소스의 데이터를 메모리로 불러온다. */
bool LoadAsset(int i) {
    if (i < 0 || i > assetCount - 1) return false;

    switch (assets[i].type) {
        case AT_FONT:
            assets[i].data.font = LoadFont(assets[i].path);

            break;

        case AT_MUSIC:
            assets[i].data.music = LoadMusicStream(assets[i].path);

            if (!assets[i].data.music.ctxData) return false;

            break;

        case AT_SOUND:
            assets[i].data.sound = LoadSound(assets[i].path);

            if (!assets[i].data.sound.frameCount) return false;

            break;

        case AT_TEXTURE:
            assets[i].data.texture = LoadTexture(assets[i].path);

            if (!assets[i].data.texture.id) return false;

            break;
    }

    return (assets[i].loaded = true);
}

/* `i + 1`번째 게임 리소스의 데이터에 할당된 메모리를 해제한다.*/
bool UnloadAsset(int i) {
    if (i < 0 || i > assetCount - 1) return false;

    switch (assets[i].type) {
        case AT_FONT:
            UnloadFont(assets[i].data.font);

            break;

        case AT_MUSIC:
            UnloadMusicStream(assets[i].data.music);

            break;

        case AT_SOUND:
            UnloadSound(assets[i].data.sound);

            break;

        case AT_TEXTURE:
            UnloadTexture(assets[i].data.texture);

            break;
    }

    return true;
}

/* `i + 1`번째 게임 리소스 데이터를 반환한다. */
Asset *GetAsset(int i) {
    return (i >= 0 && i <= assetCount - 1)
        ? (Asset *) &assets[i]
        : NULL;
}

/* 게임 리소스 파일의 개수를 반환한다. */
int GetAssetCount(void) {
    return assetCount;
}