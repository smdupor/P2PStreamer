//
// Created by smdupor on 6/4/21.
//

#include "FileEntry.h"

FileEntry::FileEntry(int id, std::string host_ident, char *path, bool local){
   lock=true;
   this->id = id;
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