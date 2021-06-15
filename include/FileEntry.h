/**
 * FileEntry
 *
 * Object containing a single distributed database entry representing a file to be shared. This file may be stored
 * locally, or remotely, or both.
 *
 *  Created on: May 31, 2021
 *      Author: smdupor
 */

#ifndef INCLUDE_FILEENTRY_H
#define INCLUDE_FILEENTRY_H

#include <iostream>
#include <fstream>
#include <cstring>

#include "PeerNode.h"

class FileEntry {
private:
   bool local, active, lock;
   std::string path;
   int length, id, cookie, ttl;
   std::string hostname;

public:
   FileEntry(int id, std::string hostname, int cookie, std::string &path, bool local, int ttl);
   FileEntry(int id, std::string hostname, int cookie, std::string path, int length);

   // Getters
   int get_length();
   std::string to_string();
   std::string to_msg();
   std::string get_hostname();
   int get_id();
   int get_cookie();
   std::string get_path();
   void set_length(int len);

   // Setters
   void set_lock();
   void clear_lock();
   void set_local();
   void set_inactive();
   void set_active();
   void decrement_ttl();

   // Booleans
   bool equals(int id, int cookie);
   bool equals (int id);
   bool equals (PeerNode &p);
   bool is_local();
   bool is_locked();
   bool is_active();
};


#endif //INCLUDE_FILEENTRY_H
