/**
 * FileEntry.cpp
 *
 * Object containing a single distributed database entry representing a file to be shared. This file may be stored
 * locally, or remotely, or both.
 *
 *  Created on: May 31, 2021
 *      Author: smdupor
 */

#include <ctime>
#include "FileEntry.h"

/**
 * Constructor for a FileEntry that has been added from a configuration file or a remote host. Does not initialize
 * file length in bytes because the check_files() method will check local file lengths.
 *
 * @param id the rfc-id-number (or more generally, an unique identifier)
 * @param hostname of the remote or local host that holds this file on disk
 * @param cookie of the remote or local host that holds this file on disk
 * @param path to this file on disk
 * @param local -- true if this entry, or another like it, has an on-local-disk copy of the file.
 * @param ttl time to live
 */
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

/**
 * Constructor for a FileEntry that has just been downloaded from a remote host, where file length in bytes needs to be
 * set immediately upon construction.
 *
 * @param id the rfc-id-number (or more generally, an unique identifier)
 * @param hostname of the remote or local host that holds this file on disk
 * @param cookie of the remote or local host that holds this file on disk
 * @param path to this file on disk
 * @param length of the file in bytes
 */
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

/**
 * Getter
 * @return length of this file in bytes
 */
int FileEntry::get_length() {
   return length;
}

/**
 * Specialized to_string that returns the messaging-standardized format
 *
 * @return a message-ready representation of this file, less the control field, which is set in the caller method.
 */
std::string FileEntry::to_msg() {
   return " Cookie: " + std::to_string(cookie) + " Hostname: " + hostname + " FileID: " + std::to_string(id) +
          " TTL: " + std::to_string(ttl) + " \n";
}

/**
 * For debug, print a pretty string copy of this entry.
 */
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

/**
 * @return the hostname of the peer that holds this entry.
 */
std::string FileEntry::get_hostname() {
   return hostname;
}

/**
 * @return the cookie of the peer that holds this entry.
 */
int FileEntry::get_cookie() {
   return cookie;
}

/**
 * @return The path to this file on local hard disk
 */
std::string FileEntry::get_path() {
   return std::string(path);
}

/**
 * @return the unique identifier of this file
 */
int FileEntry::get_id() {
   return id;
}

/**
 * Setter for file length in bytes
 * @param length of file in bytes
 */
void FileEntry::set_length(int length) {
   this->length = length;
}

/**
 * Mark entry as belonging to an inactive host
 */
void FileEntry::set_inactive() {
   active = false;
}

/**
 * Mark entry as belonging to an active host
 */
void FileEntry::set_active() {
   active = true;
}

/**
 * Mark entry that a local copy of this file is available on hard disk
 */
void FileEntry::set_local() {
   local = true;
}

/**
 * Setting of boolean locks for ownership (unrelated to synchronization locking)
 */
void FileEntry::set_lock() {
   lock = true;
}

/**
 * Setting of boolean locks for ownership (unrelated to synchronization locking)
 */
void FileEntry::clear_lock() {
   lock = false;
}

/**
 * Reduce the TTL of this entry. Called every 30s by the keepalive owner method.
 */
void FileEntry::decrement_ttl() {
   ttl -= 30;
   if (ttl < 0)
      ttl = 0;
}

/**
 * Test if (this) file belongs to PeerNode &p
 */
bool FileEntry::equals(PeerNode &p) {
   if (p.get_cookie() == this->cookie) {
      return true;
   }
   return false;
}

/**
 * Test if this file is active
 */
bool FileEntry::is_active() {
   return active;
}

/**
 * Test if this file is write-locked
 */
bool FileEntry::is_locked() {
   return lock;
}

/**
 * Test if this file is equal to another based on passed in unique-id and cookie
 */
bool FileEntry::equals(int id, int cookie) {
   return this->id == id && this->cookie == cookie;
}

/**
 * Test if this file is equal to another based on unique-id (rfc) only
 */
bool FileEntry::equals(int id) {
   return this->id == id;
}

/**
 * Test if this file is available on hard disk
 */
bool FileEntry::is_local() {
   return local;
}