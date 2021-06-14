//
// Created by smdupor on 6/4/21.
//

#include <ctime>
#include "FileEntry.h"

// We just added this either from a config file or a remote host entry
FileEntry::FileEntry(int id, std::string hostname, int cookie, std::string &path, bool local, int ttl) {
   this->id = id;
   this->cookie = cookie;
   this->hostname = hostname;
   this->path = path;
   this->local = local;
   lock = false;
   length = 0;
   this->ttl = ttl;
}

// We just downloaded this
FileEntry::FileEntry(int id, std::string hostname, int cookie, std::string path, int length) {
   this->id = id;
   this->cookie = cookie;
   this->hostname = hostname;
   this->path = path;
   this->length = length;
   local = true;
   lock = false;
   ttl = 7200;
}

int FileEntry::get_length() {
   return length;
}

/* Specialized tostring that returns the messaging-standardized format
 * Example:
 * Tokenized:
 * [0] Control Port (NOT SET HERE, SET IN CALLER FUNCTION)
 * [1] Cookie:
 * [2] <cookie ID>
 * [3] Host:
 * [4] <hostname>
 * [5] FileID:
 * [6] <file identifier>
 * [7] TTL:
 * [8] <TTL value>
 * [9] <cr><lf>
 */
std::string FileEntry::to_msg() {
   return " Cookie: " + std::to_string(cookie) + " Hostname: " + hostname + " FileID: " + std::to_string(id) +
          " TTL: " + std::to_string(ttl) + " \n";
}

std::string FileEntry::to_string() {
   std::string adder;
   if (lock) {
      adder = "LOCKED-TRUE. ";
   } else {
      adder = "NOT locked. ";
   }
   if (local) {
      return "Dir: " + std::string(path) + " Id: " + std::to_string(id) + " Local: TRUE Host: " + hostname +
             " Cookie: " + std::to_string(cookie) + adder;
   } else {
      return "Dir: " + std::string(path) + " Id: " + std::to_string(id) + " Local: FALSE Host: " + hostname +
             " Cookie: " + std::to_string(cookie) + adder;
   }
}

std::string FileEntry::get_hostname() {
   return hostname;
}

int FileEntry::get_cookie() {
   return cookie;
}

std::string FileEntry::get_path() {
   return std::string(path);
}

int FileEntry::get_id() {
   return id;
}

void FileEntry::set_length(int len) {
   length = len;
}

void FileEntry::set_inactive() {
   active = false;
}

void FileEntry::set_active() {
   active = true;
}

void FileEntry::set_local() {
   local = true;
}

void FileEntry::set_lock() {
   lock = true;
}

void FileEntry::clear_lock() {
   lock = false;
}

void FileEntry::decrement_ttl() {
   ttl -= 30;
   if (ttl < 0)
      ttl = 0;
}

bool FileEntry::equals(PeerNode &p) {
   if (p.get_cookie() == this->cookie) {
      return true;
   }
   return false;
}

bool FileEntry::is_active() {
   return active;
}

bool FileEntry::is_locked() {
   return lock;
}

bool FileEntry::equals(int id, int cookie) {
   return this->id == id && this->cookie == cookie;
}

bool FileEntry::equals(int id) {
   return this->id == id;
}

bool FileEntry::is_local() {
   return local;
}