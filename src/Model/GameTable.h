//
// Created by syh on 9/29/18.
//

#ifndef UNOSERVER_GAMETABLE_H
#define UNOSERVER_GAMETABLE_H

#include <stack>
#include "Player.h"
#include "CardDeck.h"
#include "Dealer.h"
#include "../Util/InfoSecurity.h"

using namespace std;

class GameTable { // 游戏桌
private:
    vector<Player *> players;   // 玩家
    int status;                 // 状态
    int mode;                   // 模式
    Dealer dealer;              // 发牌者

public:
    // status 常量
    static const int IDLE;
    static const int WAITING;
    static const int GAMING;

    // mode 常量
    static const int ONLINE;
    static const int PLAYERMAX;

    GameTable();

    const vector<Player *> &getPlayers() const;

    void addPlayer(Player *player);

    Player *getPlayerByUsername(const string &username);

    Player *getNextPlayer(const string &username);

    Player *getPlayerByIndex(int i);

    string getPlayerName(int i);

    void removePlayer(const string &username);

    void removePlayer(const Player *player);

    int getStatus() const;

    void setStatus(int status);

    void gameStart(UNOCard &firstCard);

    Dealer &getDealer();

    const stack<UNOCard> &getCardStack() const;

    UNOCard drawCard(string username);

    string nextTurn(string username);

    void drawCards(string username, int n);
};

#endif //UNOSERVER_GAMETABLE_H
