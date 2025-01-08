#ifndef MENU_H
#define MENU_H

#include <cstdlib>
#include <unistd.h>
#include <sys/ioctl.h>
#include <termios.h>
#include "board.h"
#include "keyPress.h"

//@@@@@@@@@@@@@@@@@@@@@@  UTILITIES @@@@@@@@@@@@@@@@@@@@@@@@@@
  int getTerminalWidth();
  std::string centerChar(char character, int width);
  std::string centerText(std::string text, int width);
  KeyPress getKeyPress();
  void clearLastLine();
//@@@@@@@@@@@@@@@@@@@@@@  MENU  @@@@@@@@@@@@@@@@@@@@@@@@@@
  std::string setPeerIP();
  int setPeerPort();
  int setLocalPort();
  void setRawMode(bool enable);
  void displayMenu(std::vector<std::string> items, int index);
  bool setLocalColor();
//@@@@@@@@@@@@@@@@@@@@@@  GAME  @@@@@@@@@@@@@@@@@@@@@@@@@@
  void announceCheckmate(bool to_play);
  void announceDraw();




#endif //MENU_H