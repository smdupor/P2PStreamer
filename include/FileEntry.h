//
// Created by smdupor on 6/4/21.
//

#ifndef INCLUDE_FILEENTRY_H
#define INCLUDE_FILEENTRY_H

#include <iostream>
#include <fstream>
#include <cstring>

#include "PeerNode.h"

class FileEntry {
private:
   bool local;
   bool active;
   bool lock;
   //const char *path;
   std::string path;
   int length, id, cookie;
   std::string hostname;

public:
   FileEntry(int id, std::string hostname, int cookie, std::string &path, bool local);
   FileEntry(int id, std::string hostname, int cookie, std::string path);
   int get_length();
   bool is_locked();
   void set_lock();
   void clear_lock();
   void set_local();
   std::string to_s();
   std::string to_msg();
   bool equals(int id, int cookie);
   bool equals (int id);
   bool equals (PeerNode &p);
   bool is_local();
   bool is_active();
   void set_inactive();
   void set_active();



   std::string get_hostname();
   int get_id();
   int get_cookie();
   std::string get_path();
   void set_length(int len);

};


#endif //INCLUDE_FILEENTRY_H
