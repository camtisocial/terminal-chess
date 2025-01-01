#ifndef NETWORK_H
#define NETWORK_H

#include "chessPiece.h"
#include <nlohmann/json.hpp>
#include <boost/asio.hpp>
#include <mutex>
#include <thread>
#include <queue>
#include <condition_variable>

extern std::queue<std::string> messageQueueIN;
extern std::queue<std::string> messageQueueOUT;
extern std::mutex queueMutex;
extern std::condition_variable queueCondVar;

void startServer(unsigned short port);
void startClient(std::string& ip, unsigned short port);
void addMessageToQueue(const std::string& message); 
std::string getMessageFromQueue();

#endif //NETWORK_H