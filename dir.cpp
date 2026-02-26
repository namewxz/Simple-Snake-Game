#include "dir.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <errno.h>
using namespace std;

Dir::Dir(std::string path) : _path(path)
{
    _pdir = ::opendir(_path.c_str());
    if (_pdir == nullptr)
        throw std::runtime_error(std::string("open dir error: ") + strerror(errno));

    cout << "open dir ok!" << endl;

    if (_path.at(_path.size() - 1) != '/')
        _path += '/';
}

Dir::~Dir() 
{
    ::closedir(_pdir);
}

List Dir::getAllPathFiles()
{
    List list;
    while (1)
    {
        struct dirent *p = ::readdir(_pdir);
        if (p == nullptr)
            break;

        std::string pathfile = _path + p->d_name;
        list.push_back(pathfile);
    }

    return list;
}
