// James N's command-line todo list app! -- listMgr.h

#define listMgr_ver 1.1
#include <string>

void printList();
void deleteItem(const int index);
void addItem(const std::string item, const std::string dd);
void addItem(const std::string item, const std::string dd);
void moveItem(const int indexSrc, const int indexDes);
void editItem(const int index, const std::string item, const std::string dd);
std::string toLower(const std::string toBeLowered);