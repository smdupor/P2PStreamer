//
// Created by smdupor on 6/4/21.
//

#include "FileEntry.h"

FileEntry::FileEntry(int id, std::string hostname, int cookie, std::string &path, bool local){
   lock=true;
   this->id = id;
   this->cookie = cookie;
   this->hostname = hostname;
   this->path = path;
   this->local = local;
   lock = false;
}

std::ifstream FileEntry::get_ifstream(){
   if(lock)
      return std::ifstream("");  // fail in case of file being locked.
   lock = true;
   return std::ifstream(path);
}
std::ofstream FileEntry::get_ofstream(){
   if(lock)
      return std::ofstream(""); // fail in case of file being locked.
   lock = true;
   return std::ofstream(path);
}
int FileEntry::get_length(){
   return length;
}
bool FileEntry::is_locked(){
   return lock;
}
void FileEntry::set_lock()
{
   lock = true;
}
void FileEntry::clear_lock() {
   lock = false;
}
void FileEntry::set_local(){
   local = true;
}
std::string FileEntry::to_s() {
   if (local){
      return "Dir: " + std::string(path) + " Id: " + std::to_string(id) + " Local: TRUE Host: " + hostname +
             " Cookie: " + std::to_string(cookie);
   }
   else
   {
      return "Dir: " + std::string(path) + " Id: " + std::to_string(id) + " Local: FALSE Host: " + hostname +
             " Cookie: " + std::to_string(cookie);
   }

}

std::string FileEntry::to_msg() {
   return " FileID: " + std::to_string(id) + " Cookie: " + std::to_string(cookie) + " Hostname: " + hostname +  " \n";
}

bool FileEntry::equals(int id, int cookie){
   return this->id == id && this->cookie == cookie;
}

bool FileEntry::equals(int id){
   return this->id == id;
}

bool FileEntry::is_local(){
   return local;
}