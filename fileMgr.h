// James N's command-line todo list app! -- fileMgr.h

#define fileMgr_ver 1.0
#include <string>

#define FILE_READ_ERROR 20
#define FILE_WRITE_ERROR 21

std::string ** pullList();
int list_numPairs(const std::string ** list);
std::string getTDFileLocation();
void saveList();