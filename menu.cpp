#include "menu.h"
using boost::asio::ip::udp;


int getTerminalWidth() {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return w.ws_col;
}

std::string centerText(std::string text, int width) {
    int padding = ((width - text.length()) / 2);
    return std::string(padding, ' ') + text;
}


std::string centerChar(char character, int width) {
    int padding = ((width/2)-7);
    return std::string(padding, ' ') + character;
}

void setRawMode(bool enable) {
    static struct termios oldt, newt;
    if (enable) {
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    } else {
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    }
}

KeyPress getKeyPress() {
    char ch;
    read(STDIN_FILENO, &ch, 1);
    if (ch == '\033') {
        char seq[2];
        read(STDIN_FILENO, &seq[0], 1);
        read(STDIN_FILENO, &seq[1], 1);
        if (seq[0] == '[') {
            switch (seq[1]) {
                case 'A': return UP;
                case 'B': return DOWN;
            }
        }
    } else if (ch == 'w' || ch == 'W') {
        return UP;
    } else if (ch == 's' || ch == 'S') {
        return DOWN;
    } else if (ch == '\n') {
        return ENTER;
    } else if (ch == ' ') {
        return ENTER;
    }
    return UNKNOWN;
}

void displayMenu(const std::vector<std::string> options, int index) {
    int terminalWidth = getTerminalWidth();

    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << centerText("         _                   ", terminalWidth) << std::endl;
    std::cout << centerText("      ___| |__   ___  ___ ___  ", terminalWidth) << std::endl;
    std::cout << centerText("    / __| '_ \\ / _ \\/ __/ __|", terminalWidth) << std::endl;
    std::cout << centerText("   | (__| | | |  __/\\__ \\__ \\", terminalWidth) << std::endl;
    std::cout << centerText("    \\___|_| |_|\\___||___/___/", terminalWidth) << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;

    for (int i = 0; i < options.size(); ++i) {
        if (i == index) {
            std::cout << centerText("> " + options[i] + " <", terminalWidth) << std::endl;
        } else {
            std::cout << centerText("  " + options[i], terminalWidth) << std::endl;
        }
    }
}

std::string getIpClient() {
    std::string ip;
    setRawMode(false);
    while (true) {
        std::cout << std::endl;
        std::cout << centerText("Enter host IP: ", getTerminalWidth());
        std::getline(std::cin, ip);
        setRawMode(true);
        return ip;
        }
}

// void getIpHost() {
    // std::string ip;
    // try {
        //  boost::asio::io_context io_context;
        //  boost::asio::ip::tcp::resolver resolver(io_context);
        //  boost::asio::ip::tcp::resolver::query query(boost::asio::ip::host_name(), "");
        //  boost::asio::ip::tcp::resolver::iterator it = resolver.resolve(query);
// 
        //  while (it != boost::asio::ip::tcp::resolver::iterator()) {
                // boost::asio::ip::tcp::endpoint endpoint = *it++;
                // if (endpoint.address().is_v4() && !endpoint.address().is_loopback()) {
                    // ip = endpoint.address().to_string();
                // }
        // }
        // } catch (std::exception& e) {
            //  std::cerr << "Error couldn't retrieve your IP: " << e.what() << std::endl;
            //  std::cout << "Enter IP manually: "; 
            //  std::getline(std::cin, ip);
        // }
        // std::cout << "Your IP is: " << ip << std::endl;
        // std::cout << "Press enter to continue";
        // std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
// }

std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}



void getLocalIpAndPort(std::string& public_ip, unsigned short& public_port) {
    std::string stun_server = "stun.l.google.com"; // Public STUN server
    unsigned short stun_port = 19302;             // STUN port
    // std::string public_ip;
    // unsigned short public_port;

    try {
        boost::asio::io_context io_context;

        // Create a UDP socket
        udp::socket socket(io_context, udp::endpoint(udp::v4(), 0));

        // Resolve the STUN server address
        udp::resolver resolver(io_context);
        auto endpoints = resolver.resolve(udp::v4(), stun_server, std::to_string(stun_port));

        // Construct a STUN Binding Request
        std::vector<uint8_t> request = {
            0x00, 0x01, // Message Type: Binding Request
            0x00, 0x00, // Message Length: 0
            0x21, 0x12, 0xA4, 0x42, // Magic Cookie
            0x63, 0xC7, 0x11, 0xAB, 0xA1, 0x1F, 0xAC, 0xA4, 0x71, 0x12, 0xD0, 0x13 // Transaction ID (randomized)
        };

        // Send the STUN request
        socket.send_to(boost::asio::buffer(request), *endpoints.begin());

        // Receive the STUN response
        std::vector<uint8_t> response(1024);
        udp::endpoint sender_endpoint;
        size_t length = socket.receive_from(boost::asio::buffer(response), sender_endpoint);

        bool flag = true;
        // Parse the STUN response (simplified for IPv4)
        if (length >= 20 && response[0] == 0x01 && response[1] == 0x01) { // Check if it's a Binding Success Response
            size_t index = 20; // Start after the STUN header (20 bytes)
            while (index + 4 < length && flag) {
                uint16_t attr_type = (response[index] << 8) | response[index + 1];
                uint16_t attr_length = (response[index + 2] << 8) | response[index + 3];
                if (attr_type == 0x0020 && attr_length >= 8) { // XOR-MAPPED-ADDRESS
                    public_port = ((response[index + 6] << 8) | response[index + 7]) ^ 0x2112;
                    uint32_t ip =
                        ((response[index + 8] ^ 0x21) << 24) |
                        ((response[index + 9] ^ 0x12) << 16) |
                        ((response[index + 10] ^ 0xA4) << 8) |
                        (response[index + 11] ^ 0x42);

                    public_ip = std::to_string((ip >> 24) & 0xFF) + "." +
                                std::to_string((ip >> 16) & 0xFF) + "." +
                                std::to_string((ip >> 8) & 0xFF) + "." +
                                std::to_string(ip & 0xFF);

                    // Output the result
                    // std::cout << "Public IP: " << public_ip << std::endl;
                    // std::cout << "Public Port: " << public_port << std::endl;
                    // sleep(3);

                    std::cout << std::endl;
                    std::cout << centerText("Your IP is: " + public_ip, getTerminalWidth()) << std::endl;
                    std::cout << centerText("Your Port is: ", getTerminalWidth()) << public_port << std::endl;
                    std::cout << std::endl;
                    std::cout << centerText("Press enter to continue", getTerminalWidth());
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                    // Exit the loop after parsing the address
                    flag = false;
                }
                index += 4 + attr_length; // Move to the next attribute
            }
        }

        // If no XOR-MAPPED-ADDRESS attribute was found, throw an error
        throw std::runtime_error("Failed to parse STUN response");
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

unsigned short getPortClient() {
    std::string u_input;
    std::cout << centerText("Enter host port: ", getTerminalWidth());
    std::getline(std::cin, u_input);
    int tmpInt = std::stoi(u_input);
    return tmpInt;
}


int getPortHost() {
    int port{12345};
    std::string u_input;
    setRawMode(false);

    while (true) {
        std::cout << std::endl;
        std::cout << centerText("Enter port or leave empty to use default (12345): ", getTerminalWidth());
        std::getline(std::cin, u_input);

        if (u_input.empty()) {
            std::cout << std::endl;
            std::cout << centerText("using default port: 12345", getTerminalWidth()) << std::endl;
            sleep(2);
            break;
        } else {
            try {
                int tmpInt= std::stoi(u_input);
                if ( tmpInt <= 0 || tmpInt > 65535) {
                    std::cout << std::endl;
                    std::cout << centerText("Invalid port number, using default", getTerminalWidth()) << std::endl;
                    sleep(2);
                    break;
                } else {
                    port = tmpInt;
                    break;
                }
                // std::cout << centerText("Port number: ", getTerminalWidth()) << u_input << std::endl;
                // std::cout << centerText("Port number string: ", getTerminalWidth()) << tmpInt << std::endl;
            } catch (const std::out_of_range& e) {
                std::cout << centerText("Port number out of range, using default", getTerminalWidth()) << std::endl;
                sleep(2);
                break;
            } catch (const std::invalid_argument& e) {
                std::cout << centerText("Invalid input, using default", getTerminalWidth()) << std::endl;
                sleep(2);
                break;
            }
        }

    }
    setRawMode(true);
    return port;
}
