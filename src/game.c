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

/* | `game` 모듈 상수... | */

/* 블랙잭 카드 덱의 기본 위치. */
static const Vector2 deckPosition = {
    0.5f * (SCREEN_WIDTH - 2 * CARD_WIDTH),
    0.14f * (SCREEN_HEIGHT - CARD_HEIGHT)
};

/* 딜러 패의 기본 위치. */
static const Vector2 dealerHandPosition = { 352.0f - 14.0f, 322.0f + 2.0f };

/* 플레이어 패의 기본 위치. */
static const Vector2 playerHandPosition = { 25.0f - 14.0f, 322.0f + 2.0f };

/* | `game` 모듈 변수... | */

/* 블랙잭 보드의 리소스 데이터. */
static Asset *astBoard;

/* 블랙잭 카드의 리소스 데이터. */
static Asset *astCards;

/* 딜러와 플레이어의 패. */
static Deck dealerHand, playerHand;

/* 블랙잭 카드 덱. */
static Deck deck;

/* 블랙잭 게임의 상태. */
static GameState gameState;

/* 게임 플레이 화면의 결과값. */
static int result;

/* | `game` 모듈 함수... | */

/* 덱에서 카드를 한 장 뽑는다. */
static void DealCard(bool toPlayer);

/* 블랙잭 보드를 그린다. */
static void DrawBoard(void);

/* 블랙잭 카드를 그린다. */
static void DrawCard(Card *card, Vector2 position);

/* 블랙잭 카드 덱을 그린다. */
static void DrawDeck(Vector2 position);

/* 딜러의 패를 그린다. */
static void DrawDealerHand(Vector2 position);

/* 플레이어의 패를 그린다.*/
static void DrawPlayerHand(Vector2 position);

/* 블랙잭 카드 덱을 생성한다. */
static void GenerateDeck(void);

/* 마우스 이벤트를 처리한다. */
static void HandleMouseEvents(void);

/* 게임의 상태를 변경한다. */
static void TakeTurn(bool hit);

/* 게임을 초기화한다. */
static void ResetGame(void);

/* 게임 플레이 장면을 초기화한다. */
void InitGameScene(void) {
    astBoard = GetAsset(0);
    astCards = GetAsset(1);

    ResetGame();
}

/* 게임 플레이 장면을 업데이트한다. */
void UpdateGameScene(void) {
#ifdef _DEBUG
    /* TODO: 창 및 버튼 추가... */

    if (IsKeyPressed(KEY_Q) || IsKeyPressed(KEY_W))
        TakeTurn(IsKeyPressed(KEY_Q));

    if (IsKeyPressed(KEY_R)) ResetGame();
#endif

    // 마우스 이벤트를 처리한다.
    HandleMouseEvents();

    {
        // 블랙잭 보드를 그린다.
        DrawBoard();

        // 블랙잭 카드 덱을 그린다.
        DrawDeck(deckPosition);

        // 플레이어의 패를 그린다.
        DrawPlayerHand(playerHandPosition);

        // 딜러의 패를 그린다.
        DrawDealerHand(dealerHandPosition);
        
#ifdef _DEBUG
        // FPS 카운터를 그린다.
        DrawFPS(8, 8);
#endif
    }
}

/* 게임 플레이 장면을 종료한다. */
int FinishGameScene(void) {
    return result;
}

/* 덱에서 카드를 한 장 뽑는다. */
static void DealCard(bool toPlayer) {
    if (deck.length <= 0) {
        TraceLog(
            LOG_INFO, 
            "Unable to deal a card because the deck is empty"
        );
        
        return;
    } else if ((toPlayer && playerHand.length >= MAX_HAND_COUNT)
        || (!toPlayer && dealerHand.length >= MAX_HAND_COUNT)) {
        TraceLog(
            LOG_INFO, 
            "Unable to deal a card because %s's hand is full",
            toPlayer ? "the player" : "the dealer"
        );

        return;
    }

    // 덱의 맨 위에 있는 카드를 삭제한다.
    Card top = deck.cards[--deck.length];

    top.offset = deck.cards[deck.length - 1].offset = 0.0f;

    TraceLog(
        LOG_INFO, 
        "Dealing card #%d to %s",
        toPlayer ? playerHand.length : dealerHand.length,
        toPlayer ? "the player" : "the dealer"
    );

    if (toPlayer) {
        // 플레이어는 자신의 패를 다 볼 수 있다.
        top.state = CS_FRONT_NORMAL;

        playerHand.cards[playerHand.length++] = top;
        playerHand.total += (top.index < 10) ? top.index + 1 : 10;
    } else {
        // 딜러의 첫 번째 카드만 보여준다.
        top.state = (dealerHand.length == 0) ? CS_FRONT_NORMAL : CS_BACK_NORMAL;

        dealerHand.cards[dealerHand.length++] = top;
        dealerHand.total += (top.index < 10) ? top.index + 1 : 10;
    }
}

/* 블랙잭 보드를 그린다. */
static void DrawBoard(void) {
    ClearBackground(BLACK);

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
    if (card->state == CS_BACK_NORMAL || card->state == CS_BACK_HOVER) {
        source.x = MAX_CARD_NUMBER * CARD_WIDTH;
        source.y = SU_DIAMOND * CARD_HEIGHT;
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

/* 딜러의 패를 그린다. */
static void DrawDealerHand(Vector2 position) {
    DrawTextEx(
        GetFontDefault(),
        gameState == GS_DEALER_TURN 
            ? TextFormat("TOTAL : %d", dealerHand.total)
            : TextFormat("TOTAL : ?"),
        (Vector2) {
            dealerHandPosition.x + 9.0f,
            dealerHandPosition.y - 32.0f
        },
        20.0f,
        2.0f,
        WHITE
    );

    for (int i = 0; i < dealerHand.length; i++)
        DrawCard(
            &dealerHand.cards[i], 
            (Vector2) { 
                position.x + (0.25f * CARD_WIDTH) * i,
                position.y + (
                    (dealerHand.cards[i].state == CS_FRONT_HOVER
                        || dealerHand.cards[i].state == CS_BACK_HOVER)
                        ? -0.75f * CARD_HEIGHT
                        : 0.0f
                )
            }
        );
}

/* 플레이어의 패를 그린다.*/
static void DrawPlayerHand(Vector2 position) {
    DrawTextEx(
        GetFontDefault(),
        TextFormat("TOTAL : %d", playerHand.total),
        (Vector2) {
            playerHandPosition.x + 9.0f,
            playerHandPosition.y - 32.0f
        },
        20.0f,
        2.0f,
        WHITE
    );

    for (int i = 0; i < playerHand.length; i++)
        DrawCard(
            &playerHand.cards[i], 
            (Vector2) { 
                position.x + (0.25f * CARD_WIDTH) * i,
                position.y + (
                    (playerHand.cards[i].state == CS_FRONT_HOVER
                        || playerHand.cards[i].state == CS_BACK_HOVER)
                        ? -0.75f * CARD_HEIGHT
                        : 0.0f
                )
            }
        );
}

/* 블랙잭 카드 덱을 생성한다. */
static void GenerateDeck(void) {
    deck.length = MAX_CARD_COUNT;

    for (int y = 0; y < _SU_COUNT; y++)
        for (int x = 0; x < MAX_CARD_NUMBER; x++) {
            deck.cards[y * MAX_CARD_NUMBER + x] = (Card) {
                .suit = y,
                .index = x,
                .offset = 0.4f * GetRandomValue(4, 8),
                .state = CS_BACK_NORMAL
            };
        }

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

    TraceLog(LOG_INFO, "GAME: Generated a new deck successfully");
}

/* 마우스 이벤트를 처리한다. */
static void HandleMouseEvents(void) {
    for (int i = 0; i < playerHand.length; i++) {
        Rectangle aabb = {
            .x = playerHandPosition.x + (0.274f * CARD_WIDTH) * (i + 1),
            .y = playerHandPosition.y,
            .width = (i != playerHand.length - 1) 
                ? 0.25f * CARD_WIDTH 
                : 1.35f * CARD_WIDTH,
            .height = 2.0f * CARD_HEIGHT
        };

        playerHand.cards[i].state = CheckCollisionPointRec(GetMousePosition(), aabb) 
            ? CS_FRONT_HOVER
            : CS_FRONT_NORMAL;
    }

    for (int i = 0; i < dealerHand.length; i++) {
        Rectangle aabb = {
            .x = dealerHandPosition.x + (0.274f * CARD_WIDTH) * (i + 1),
            .y = dealerHandPosition.y,
            .width = (i != dealerHand.length - 1)
                ? 0.25f * CARD_WIDTH 
                : 1.35f * CARD_WIDTH,
            .height = 2.0f * CARD_HEIGHT
        };

        if (CheckCollisionPointRec(GetMousePosition(), aabb)) {
            if (dealerHand.cards[i].state == CS_FRONT_NORMAL) 
                dealerHand.cards[i].state = CS_FRONT_HOVER;
            else if (dealerHand.cards[i].state == CS_BACK_NORMAL)
                dealerHand.cards[i].state = CS_BACK_HOVER;
        } else {
            if (dealerHand.cards[i].state == CS_FRONT_HOVER) 
                dealerHand.cards[i].state = CS_FRONT_NORMAL;
            else if (dealerHand.cards[i].state == CS_BACK_HOVER)
                dealerHand.cards[i].state = CS_BACK_NORMAL;
        }
    }
}

/* 게임의 상태를 변경한다. */
static void TakeTurn(bool hit) {
    if (gameState == GS_PLAYER_TURN && !hit)
        gameState = GS_DEALER_TURN;
    
    switch (gameState) {
        case GS_INIT:
            for (int i = 0; i < 2; i++)
                DealCard(true), DealCard(false);
            
            gameState = GS_PLAYER_TURN;

            break;

        case GS_PLAYER_TURN:
            DealCard(true);

            if (playerHand.total >= BLACKJACK)
                TakeTurn(false);

            break;

        case GS_DEALER_TURN:
            for (int i = 0; i < dealerHand.length; i++)
                dealerHand.cards[i].state = CS_FRONT_NORMAL;

            break;
    }
}

/* 게임을 초기화한다. */
static void ResetGame(void) {
    playerHand.length = playerHand.total = 0;
    dealerHand.length = dealerHand.total = 0;

    gameState = GS_INIT;

    GenerateDeck();
}