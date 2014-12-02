#ifndef UTILITY_H
#define UTILITY_H

#include <fstream>
#include <iostream>
#include <string.h>
#include <string>
#include <assert.h>
#include <stdlib.h>

class Utility
{
public:
    Utility();
    void saveFile(char *fileName, char *content);
    char** str_split(char* a_str, const char a_delim);
};

#endif // UTILITY_H
