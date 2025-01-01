#include "network.h"
using boost::asio::ip::udp;
using boost::asio::ip::tcp;



//@@@@@@@@@@@@@@@@@@@@@@  SERVER  @@@@@@@@@@@@@@@@@@@@@@@@@@
std::queue<std::string> messageQueueIN;
std::queue<std::string> messageQueueOUT;
std::mutex queueMutex;
std::condition_variable queueCondVar;

void startServer(unsigned short port) {
    // try {
    //     boost::asio::io_context io_context;

    //     tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), port));
    //     std::cout << "server listening on port " << port << std::endl;
    //     sleep(3);

    //     boost::asio::ip::tcp::endpoint local_endpoint = acceptor.local_endpoint();
    //     std::string local_ip = local_endpoint.address().to_string();
    //     std::cout << "Server IP address: " << local_ip << std::endl; 
    //     sleep(3);

    //     tcp::socket socket(io_context);
    //     acceptor.accept(socket);
    //     sleep(1);
    //     std::cout << "connection established" << std::endl;

    //     //LISTEN
    //     char buffer[1024];
    //     boost::system::error_code error;
    //     while (true) {
    //         size_t len = socket.read_some(boost::asio::buffer(buffer), error);
    //         if (error == boost::asio::error::eof) {
    //             std::cout << "client disconnected." << std::endl;
    //             sleep(2);
    //             break;
    //         }
    //         std::string message(buffer, len);
    //         std::cout << "message: " << message << std::endl;
    //     }

    // } catch (std::exception& e) {
    //     sleep(5);
    //     std::cerr << "server error: " << e.what() << std::endl;
    // }


    //%%%%%
    
        try {
        boost::asio::io_context io_context;

        // Create a UDP socket
        udp::socket socket(io_context, udp::endpoint(udp::v4(), port));
        std::cout << "Server listening on port " << port << std::endl;

        char buffer[1024];
        udp::endpoint remote_endpoint;

        while (true) {
            // Wait for a message from the client
            size_t len = socket.receive_from(boost::asio::buffer(buffer), remote_endpoint);

            // Print the received message
            std::string message(buffer, len);
            std::cout << "Received message: " << message << " from " << remote_endpoint.address().to_string() << ":" << remote_endpoint.port() << std::endl;

            // Send a response back to the client
            std::string response = "Hello from server!";
            socket.send_to(boost::asio::buffer(response), remote_endpoint);
        }
    } catch (const std::exception& e) {
        std::cerr << "Server error: " << e.what() << std::endl;
    }

    //%%%%%

}



//@@@@@@@@@@@@@@@@@@@@@@  CLIENT  @@@@@@@@@@@@@@@@@@@@@@@@@@
void startClient(std::string& server_ip, unsigned short server_port) {
    std::cout << "Client connecting to " << server_ip << ":" << server_port << std::endl;
    // try {
    //     boost::asio::io_context io_context;

    //     tcp::resolver resolver(io_context);
    //     auto endpoints = resolver.resolve(ip, std::to_string(port));
    //     std::cout << "IP: " << ip << " Port: " << port << std::endl;

    //     // create socket and connect to server
    //     tcp::socket socket(io_context);
    //     boost::asio::connect(socket, endpoints);
    //     std::cout << "\nconnected to the server" << std::endl;
    //     sleep(2);


    //     while (true) {
    //         std::unique_lock<std::mutex> lock(queueMutex);
    //         queueCondVar.wait(lock, [] { return !messageQueueIN.empty() || !messageQueueOUT.empty(); });
    //     // LISTEN


    //     //WRITE
    //         while (!messageQueueOUT.empty()) {
    //             std::string message = messageQueueOUT.front();
    //             messageQueueOUT.pop();
    //             lock.unlock();
    //             boost::asio::write(socket, boost::asio::buffer(message));
    //             lock.lock();
    //         }
    //     }

    // } catch (std::exception& e) {
    //     std::cerr << "Client error: " << e.what() << std::endl;
    //     sleep(5);
    // }




    //%%%%%
    try {
        boost::asio::io_context io_context;

        // Create a UDP socket
        udp::socket socket(io_context, udp::endpoint(udp::v4(), 0));

        // Resolve the server's IP and port
        udp::resolver resolver(io_context);
        udp::endpoint server_endpoint = *resolver.resolve(udp::v4(), server_ip, std::to_string(server_port)).begin();

        // Send a message to the server
        std::string message = "Hello from client!";
        socket.send_to(boost::asio::buffer(message), server_endpoint);

        // Receive a response from the server
        char buffer[1024];
        udp::endpoint remote_endpoint;
        size_t len = socket.receive_from(boost::asio::buffer(buffer), remote_endpoint);

        // Print the received response
        std::string response(buffer, len);
        std::cout << "Received response: " << response << " from " << remote_endpoint.address().to_string() << ":" << remote_endpoint.port() << std::endl;
        sleep(5);
    } catch (const std::exception& e) {
        std::cerr << "Client error: " << e.what() << std::endl;
        sleep(5);
    }
    //%%%%%
}

void addMessageToQueue(const std::string& message) {
    std::lock_guard<std::mutex> lock(queueMutex);
    messageQueueOUT.push(message);
    queueCondVar.notify_one();
}

std::string getMessageFromQueue() {
    std::unique_lock<std::mutex> lock(queueMutex);
    queueCondVar.wait(lock, [] { return !messageQueueIN.empty(); });
    std::string message = messageQueueIN.front();
    messageQueueIN.pop();
    return message;
}