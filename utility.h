#ifndef UTILITY_H
#define UTILITY_H


class Utility
{
public:
    Utility();
    void saveNeighborsList();
    void saveFile(char *fileName, char *content);
    char** str_split(char* a_str, const char a_delim);
};

#endif // UTILITY_H
