//
// Created by smdupor on 6/4/21.
//

#ifndef INCLUDE_FILEENTRY_H
#define INCLUDE_FILEENTRY_H

#include <iostream>
#include <fstream>

class FileEntry {
private:
   bool local;
   bool lock;
   char *path;
   int length;
   int id;
   std::string hostname;

public:
   FileEntry(int id, std::string hostname, char *path, bool local);
   std::ifstream get_ifstream();
   std::ofstream get_ofstream();
   int get_length();
   bool is_locked();
   void set_lock();
   void clear_lock();
   void set_local();
};


#endif //INCLUDE_FILEENTRY_H
