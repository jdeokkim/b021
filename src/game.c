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

#include <stdio.h>

#include "b021.h"

/* | `game` 모듈 상수... | */

/* 블랙잭 카드 덱의 기본 위치. */
static const Vector2 deckPosition = {
    0.5f * (SCREEN_WIDTH - 2 * CARD_WIDTH),
    0.14f * (SCREEN_HEIGHT - CARD_HEIGHT)
};

/* | `game` 모듈 변수... | */

/* 블랙잭 보드의 리소스 데이터. */
static Asset *astBoard;

/* 블랙잭 카드의 리소스 데이터. */
static Asset *astCards;

/* 블랙잭 카드 덱. */
static Deck deck;

/* | `game` 모듈 함수... | */

/* 블랙잭 카드 덱을 생성한다. */
static void GenerateDeck(void);

/* 
    피셔-예이츠 셔플 알고리즘을 이용하여, 
    블랙잭 카드 덱을 섞는다.
*/
static void ShuffleDeck(void);

/* 블랙잭 보드를 그린다. */
static void DrawBoard(void);

/* 블랙잭 카드를 그린다. */
static void DrawCard(Card *card, Vector2 position);

/* 블랙잭 카드 덱을 그린다. */
static void DrawDeck(Vector2 position);

/* 게임 플레이 장면을 초기화한다. */
void InitGameScene(void) {
    astBoard = GetAsset(0);
    astCards = GetAsset(1);

    GenerateDeck();
}

/* 게임 플레이 장면을 업데이트한다. */
void UpdateGameScene(void) {
    ClearBackground(BLACK);

    // 블랙잭 보드를 그린다.
    DrawBoard();

    // 블랙잭 카드 덱을 그린다.
    DrawDeck(deckPosition);

#ifdef _DEBUG
    DrawFPS(8, 8);
#endif
}

/* 게임 플레이 장면을 종료한다. */
int FinishGameScene(void) {
    /* TODO: ... */
}

/* 블랙잭 카드 덱을 생성한다. */
static void GenerateDeck(void) {
    deck.length = MAX_CARD_COUNT;

    for (int y = 0; y < _SUIT_COUNT; y++)
        for (int x = 0; x < MAX_CARD_NUMBER; x++) {
            const int rv = GetRandomValue(4, 8);

            deck.cards[y * MAX_CARD_NUMBER + x] = (Card) {
                .suit = y,
                .index = x,
                .offset = 0.4f * rv,
                .state = false
            };
        }

    ShuffleDeck();

    TraceLog(LOG_INFO, "GAME: Generated a new deck successfully");
}

/* 
    피셔-예이츠 셔플 알고리즘을 이용하여, 
    블랙잭 카드 덱을 섞는다.
*/
static void ShuffleDeck(void) {
    /* https://en.wikipedia.org/wiki/Fisher%E2%80%93Yates_shuffle */

    for (int i = MAX_CARD_COUNT - 1; i >= 1; i--) {
        int j = GetRandomValue(0, MAX_CARD_COUNT - 1);

        int tempSuit = deck.cards[i].suit;
        int tempIndex = deck.cards[i].index;

        deck.cards[i].suit = deck.cards[j].suit;
        deck.cards[i].index = deck.cards[j].index;

        deck.cards[j].suit = tempSuit;
        deck.cards[j].index = tempIndex;
    }
}

/* 블랙잭 보드를 그린다. */
static void DrawBoard(void) {
    DrawTextureV(astBoard->data.texture, (Vector2) { 0.0f }, WHITE);
}

/* 블랙잭 카드를 그린다. */
static void DrawCard(Card *card, Vector2 position) {
    if (card == NULL) return;

    Rectangle source = {
        .x = card->index * CARD_WIDTH,
        .y = card->suit * CARD_HEIGHT,
        .width = CARD_WIDTH,
        .height = CARD_HEIGHT
    };

    // 카드가 뒷면인 경우?
    if (!card->state) {
        source.x = MAX_CARD_NUMBER * CARD_WIDTH;
        source.y = SUIT_DIAMOND * CARD_HEIGHT;
    }

    Card *top = &deck.cards[deck.length - 1];

    // 이 카드가 맨 위에 있지 않을 경우?
    if (top->suit != card->suit || top->index != card->index)
        source.height -= card->offset;

    const Rectangle dest = {
        .x = position.x,
        .y = position.y,
        .width = 2.0f * CARD_WIDTH,
        .height = 2.0f * CARD_HEIGHT
    };

    DrawTexturePro(
        astCards->data.texture,
        source,
        dest,
        (Vector2) { 0.0f },
        0.0f,
        WHITE
    );
}

/* 블랙잭 카드 덱을 그린다. */
static void DrawDeck(Vector2 position) {
    for (int i = 0; i < deck.length; i++) {
        DrawCard(&deck.cards[i], position);

        position.y -= 0.8f;
    }
}