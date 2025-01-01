#include "main.h"


void startLocalGame() {
    //ADD ANIMATION BEFORE BOARD FLIPS SO ITS LESS ABRUPT
    GameBoard board;
    //0 = white to play, 1 = black to play
    bool to_play = 0;
    int turn = 1;
    
    std::string q = "";
    while(q != "q") {
        std::cout << "\n\n\n";
        if(!to_play) {
            board.printBoardWhite(to_play, turn);
        } else {
            board.printBoardBlack(to_play, turn);
        }
        std::cout << "\n\n";
        std::cout << "   Enter move: ";
        std::cout.flush();
        std::getline(std::cin,  q);
        if (q != "q") {
            if (board.movePiece(q, to_play)) {
                if(to_play) {turn++;}
                to_play = !to_play;
            };
        }
        system("clear");
    }
}

void startOnlineGame() {


    //listen for messages

    //send message



    std::string q = "";
    while (q != "q") {
        std::cout << "Enter message: ";
        // std::cin >> q;
        std::getline(std::cin, q);
        if (q != "q") {
            addMessageToQueue(q);
        }
    }

    // GameBoard board;
    // //0 = white to play, 1 = black to play
    // bool local_color = 0;
    // bool to_play = 0;
    // int turn = 1;
    
    // std::string q = "";
    // while(q != "q") {
    //     std::cout << "\n\n\n";
    //     if(local_color) {
    //         board.printBoardWhite(to_play, turn);
    //     } else {
    //         board.printBoardBlack(to_play, turn);
    //     }
    //     std::cout << "\n\n";
    //     std::cout << "   Enter move: ";
    //     std::cout.flush();
    //     std::getline(std::cin,  q);
    //     if (q != "q") {
    //         if (board.movePiece(q, to_play)) {
    //             if(to_play) {turn++;}
    //             to_play = !to_play;
    //         };
    //     }
    //     system("clear");
    // }
}


void joinOnlineGame() {

    std::string q = "";
    while (q != "q") {
        std::cout << "Enter message: ";
        std::getline(std::cin, q);
        addMessageToQueue(q);
    }
}


int main(int argc, char** argv) {
    std::string tmpIP; 
    unsigned short tmpPort;
    int selected = 0;
    std::vector<std::string> options = {"Host Game", "Join Game", "Local", "Quit"};
    setRawMode(true);

    while (true) {
        // system("clear");
        displayMenu(options, selected);

        KeyPress key = getKeyPress();
        if (key == UP) {
            selected = (selected - 1 + options.size()) % options.size();
        } else if (key == DOWN) {
            selected = (selected + 1) % options.size();
        } else if (key == ENTER) {
            if (options[selected] == "Quit") {
                break;
            } else if (options[selected] == "Host Game") {
                setRawMode(false);
                // int port = getPortHost();
                

                getLocalIpAndPort(tmpIP, tmpPort);
                std::thread serverThread(startServer, tmpPort);
                serverThread.detach();
                // startOnlineGame();

            } else if (options[selected] == "Join Game") {
                setRawMode(false);
                std::string ip = getIpClient();
                unsigned short port = getPortClient(); 
                std::thread clientThread(startClient, std::ref(ip), port);
                clientThread.detach();
                // joinOnlineGame();
            } else if (options[selected] == "Local") {
                // system("clear");
                setRawMode(false);
                startLocalGame();
            }
        }
    }
    
//         std::cout << centerText(R"(
//          _                   
//      ___| |__   ___  ___ ___  
//     / __| '_ \ / _ \/ __/ __|
//    | (__| | | |  __/\__ \__ \
//     \___|_| |_|\___||___/___/)", terminalWidth);



    //     board.printBoard();
    //     std::cout << "\n\n";
    //     std::cout << "   Enter move: ";
    //     std::cout.flush();
    //     std::getline(std::cin,  q);
    //     if (q != "q") {
    //         if (board.movePiece(q, to_play)) {
    //             turn++;
    //             to_play = !to_play;
    //             board.saveBoardState(turn, to_play, boardState);
    //             std::cout << boardState.dump(4) << std::endl;
    //         };
            
    //     }
    //     std::cout << "\n";
    //     system("cls");

    setRawMode(false);
	return 0;
}

/*

int main(int argc, char** argv) {

    GameBoard board;
    nlohmann::json boardState;
    //0 = white to play, 1 = black to play
    bool to_play = 0;
    int turn = 0;
    
    std::cout << std::endl;
    std::string q = "";
    while(q != "q") {
        std::cout << R"(
         _                   
     ___| |__   ___  ___ ___  
    / __| '_ \ / _ \/ __/ __|
   | (__| | | |  __/\__ \__ \
    \___|_| |_|\___||___/___/)";


        std::cout << "\n\n\n";
        board.printBoard();
        std::cout << "\n\n";
        std::cout << "   Enter move: ";
        std::cout.flush();
        std::getline(std::cin,  q);
        if (q != "q") {
            if (board.movePiece(q, to_play)) {
                turn++;
                to_play = !to_play;
                board.saveBoardState(turn, to_play, boardState);
                std::cout << boardState.dump(4) << std::endl;
            };
            
        }
        std::cout << "\n";
        system("cls");
    }

	return 0;
}


*/