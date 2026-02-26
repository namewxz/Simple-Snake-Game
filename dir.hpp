#ifndef DIR_HPP
#define DIR_HPP

#include <string>
#include <dirent.h>
#include "list.hpp"
 

class Dir
{
public:
    Dir(std::string path);
    ~Dir();
    // 读目录，把目录下的带路径的文件名保存到一个链表中，并返回
    List getAllPathFiles();
private:
    std::string _path;
    DIR * _pdir;
};


#endif