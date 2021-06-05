//
// Created by smdupor on 6/4/21.
//

#ifndef INCLUDE_FILEENTRY_H
#define INCLUDE_FILEENTRY_H

#include <iostream>
#include <fstream>
#include <cstring>

class FileEntry {
private:
   bool local;
   bool lock;
   std::string path;
   int length, id, cookie;
   std::string hostname;

public:
   FileEntry(int id, std::string hostname, int cookie, std::string &path, bool local);
   std::ifstream get_ifstream();
   std::ofstream get_ofstream();
   int get_length();
   bool is_locked();
   void set_lock();
   void clear_lock();
   void set_local();
   std::string to_s();
   std::string to_msg();
   bool equals(int id, int cookie);
   bool equals (int id);
   bool is_local();
};


#endif //INCLUDE_FILEENTRY_H
