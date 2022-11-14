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

#ifndef B021_H
#define B021_H

#include <stdbool.h>

#include "raylib.h"

/* | 매크로 정의... | */

#define GAME_NAME         "jdeokkim/b021"
#define GAME_VERSION      "0.0.1"

#define TARGET_FPS        60

#define SCREEN_WIDTH      640
#define SCREEN_HEIGHT     480

#define CARD_WIDTH        64.0f
#define CARD_HEIGHT       64.0f

#define MAX_CARD_NUMBER   13
#define MAX_CARD_COUNT    52

/* | 자료형 정의... | */

/* 게임의 리소스 파일 종류를 나타내는 열거형. */
typedef enum AssetType {
    AT_FONT,     // 비트맵 글꼴
    AT_MUSIC,    // 배경 음악
    AT_SOUND,    // 효과음
    AT_TEXTURE   // 텍스처
} AssetType;

/* 게임의 장면을 나타내는 열거형. */
typedef enum SceneType {
    ST_INIT,     // 초기 화면
    ST_LOADING,  // 로딩 화면
    ST_GAME      // 게임 화면
} SceneType;

/* 게임 리소스 데이터를 나타내는 구조체. */
typedef struct Asset {
    bool loaded;            // 로딩 여부.
    AssetType type;         // 리소스 데이터의 종류.
    const char *path;       // 리소스 파일 경로.
    union {
        Font font;
        Music music;
        Sound sound;
        Texture2D texture;
    } data;                 // 리소스 데이터.
} Asset;

/* 블랙잭 카드의 무늬를 나타내는 열거형. */
typedef enum Suit {
    SUIT_HEART,    // 하트 무늬.
    SUIT_DIAMOND,  // 다이아몬드 무늬.
    SUIT_CLOVER,   // 클로버 무늬.
    SUIT_SPADE,    // 스페이드 무늬.
    _SUIT_COUNT
} Suit;

/* 블랙잭 카드의 정보를 나타내는 구조체. */
typedef struct Card {
    Suit suit;      // 카드의 무늬.
    int index;      // 카드의 인덱스.
    float offset;   // 카드의 랜덤 값.
    bool state;     // 카드의 상태.
} Card;

/* 블랙잭 카드 덱을 나타내는 구조체. */
typedef struct Deck {
    Card cards[MAX_CARD_COUNT];  // 블랙잭 카드 배열.
    int length;                  // 남아있는 카드 수.
} Deck;

/* | `asset` 모듈 함수... | */

/* `i + 1`번째 게임 리소스의 데이터를 메모리로 불러온다. */
bool LoadAsset(int i);

/* `i + 1`번째 게임 리소스의 데이터에 할당된 메모리를 해제한다.*/
bool UnloadAsset(int i);

/* `i + 1`번째 게임 리소스 데이터를 반환한다. */
Asset *GetAsset(int i);

/* 게임 리소스 파일의 개수를 반환한다. */
int GetAssetCount(void);

/* | `game` 모듈 함수... | */

/* 게임 플레이 장면을 초기화한다. */
void InitGameScene(void);

/* 게임 플레이 장면을 업데이트한다. */
void UpdateGameScene(void);

/* 게임 플레이 장면을 종료한다. */
int FinishGameScene(void);

/* | `loader` 모듈 함수... | */

/* 로딩 화면을 초기화한다. */
void InitLoadingScene(void);

/* 로딩 화면을 업데이트한다. */
void UpdateLoadingScene(void);

/* 로딩 화면을 종료한다. */
int FinishLoadingScene(void);

/* | `scene` 모듈 함수... | */

/* 게임 장면을 변경한다. */
void LoadScene(SceneType st);

/* 게임 화면을 업데이트한다. */
void UpdateGame(void);

#endif