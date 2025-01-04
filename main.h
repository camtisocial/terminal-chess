#ifndef MAIN_H
#define MAIN_H

#include "board.h"
#include "menu.h"
#include "network.h"
#include "globals.h"


void startLocalGame();
void startOnlineGame(int localColor, udp::socket& socket, udp::endpoint& peer_endpoint);
void startHost();
void startJoin();
void lanMenu();
#endif //MAIN_H