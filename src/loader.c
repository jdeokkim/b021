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
#include "b021-preload.h"

/* | `loader` 모듈 매크로 정의... | */

#define PROGRESS_BAR_INNER_COLOR  ((Color) { 56, 135, 122, 255 })
#define PROGRESS_BAR_OUTER_THICK  2.5f

#define LOADER_DELAY_TIME         0.35f

/* | `loader` 모듈 변수... | */

/* 로딩 화면에 사용되는 기본 글꼴. */
static Font fntPreload;

/* 로딩 바의 안쪽 부분 직사각형. */
static Rectangle progressBarInnerBounds = {
    .width = 476.0f,
    .height = 44.0f
};

/* 로딩 바의 바깥쪽 부분 직사각형. */
static Rectangle progressBarOuterBounds = {
    .width = 480.0f,
    .height = 48.0f
};

/* 로딩 화면에 사용되는 내장 리소스 파일. */
static Texture2D txMessage01, txMessage02;

/* 리소스 파일 로딩 관련 오류 발생 여부. */
static bool loadingFailed;

/* 로딩이 완료된 리소스 파일의 개수. */
static int assetCount;

/* 게임 로딩 완료 여부. */
static int result;

/* | `loader` 모듈 함수... | */

/* 로딩 바를 그린다. */
static void DrawProgressBar(void);

/* 로딩 화면을 초기화한다. */
void InitLoadingScene(void) {
    Image imgPreload01 = LoadImageFromMemory(
        ".png", 
        preload_01_bin,
        PRELOAD_01_SIZE
    );

    Image imgPreload02 = LoadImageFromMemory(
        ".png", 
        preload_02_bin,
        PRELOAD_02_SIZE
    );

    Image imgPreload03 = LoadImageFromMemory(
        ".png", 
        preload_03_bin,
        PRELOAD_03_SIZE
    );

    fntPreload = LoadFontFromImage(imgPreload01, MAGENTA, 0x20);

    txMessage01 = LoadTextureFromImage(imgPreload02);
    txMessage02 = LoadTextureFromImage(imgPreload03);

    UnloadImage(imgPreload03), UnloadImage(imgPreload02), UnloadImage(imgPreload01);

    progressBarOuterBounds.x = 0.5f * (SCREEN_WIDTH - progressBarOuterBounds.width);
    progressBarOuterBounds.y = 0.45f * (SCREEN_HEIGHT - progressBarOuterBounds.height);

    progressBarInnerBounds.x = progressBarOuterBounds.x 
        + 0.5f * (progressBarOuterBounds.width - progressBarInnerBounds.width);

    progressBarInnerBounds.y = progressBarOuterBounds.y
        + 0.5f * (progressBarOuterBounds.height - progressBarInnerBounds.height);
}

/* 로딩 화면을 업데이트한다. */
void UpdateLoadingScene(void) {
    static float frameTimer;

    ClearBackground(BLACK);

    DrawTextureEx(
        txMessage02, 
        (Vector2) { 
            (SCREEN_WIDTH - txMessage02.width) - 12.0f, 
            (SCREEN_HEIGHT - txMessage02.height) - 12.0f
        },
        0.0f,
        1.0f,
        WHITE
    );

    if (loadingFailed) {
        DrawTextureEx(
            txMessage01, 
            (Vector2) { 
                0.5f * (SCREEN_WIDTH - txMessage01.width), 
                0.5f * (SCREEN_HEIGHT - txMessage01.height)
            },
            0.0f,
            1.0f,
            WHITE
        );

        return;
    }

    const int maxAssetCount = GetAssetCount();

    if (assetCount < maxAssetCount) {
        Asset *ast = GetAsset(assetCount);

        if (frameTimer >= 0.5f * LOADER_DELAY_TIME) {
            bool check = LoadAsset(assetCount);

            if (!check) {
                loadingFailed = true;

                return;
            }

            assetCount++, frameTimer = 0.0f;
        }

        const char *pathText = TextFormat(
            "./%s (%d / %d)", 
            ast->path,
            assetCount + 1,
            maxAssetCount
        );

        const Vector2 textDimensions = MeasureTextEx(
            fntPreload,
            pathText,
            PRELOAD_FONT_SIZE,
            2.0f
        );

        DrawTextEx(
            fntPreload,
            pathText,
            (Vector2) {
                0.5f * (SCREEN_WIDTH - textDimensions.x),
                progressBarOuterBounds.y + 63.0f
            },
            PRELOAD_FONT_SIZE,
            2.0f,
            WHITE
        );
    } else {
        if (frameTimer >= 2.0f * LOADER_DELAY_TIME) {
            frameTimer = 0.0f, result = 1;

            return;
        }
    }

    frameTimer += GetFrameTime();

    DrawProgressBar();
}

/* 로딩 화면을 종료한다. */
int FinishLoadingScene(void) {
    if (result == 1) {
        UnloadTexture(txMessage02);
        UnloadTexture(txMessage01);

        UnloadFont(fntPreload);
    }

    return result;
}

/* 로딩 바를 그린다. */
static void DrawProgressBar(void) {
    DrawRectangleRec(
        progressBarInnerBounds,
        PROGRESS_BAR_INNER_COLOR
    );

    DrawRectangleLinesEx(
        progressBarOuterBounds,
        PROGRESS_BAR_OUTER_THICK,
        WHITE
    );
}