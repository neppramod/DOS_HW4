#include "utility.h"
#include "constants.h"
#include "server.h"

#include <fstream>
#include <iostream>
#include <string.h>
#include <string>
#include <assert.h>
#include <stdlib.h>


using namespace std;

Utility::Utility()
{
}

void Utility::saveNeighborsList()
{
    char payload[BUFFER_SIZE];

    for (int j = 0; j < node_index; j++) {
        strcat(payload, neighbors_list[j].host_name);
        strcat(payload, " ");
        strcat(payload, neighbors_list[j].host_port);
        strcat(payload, "\n");
    }

    saveFile(NEIGHBORS_FILE_LIST_NAME, payload);
}

void Utility::saveFile(char *fileName, char *content)
{

        ofstream output_file(fileName);

        if(output_file.is_open()) {
            output_file << content << endl;
        }
        output_file.close();
}

char** Utility::str_split(char* a_str, const char a_delim)
{
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;

    result = (char **)malloc(sizeof(char*) * count);

    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
}
