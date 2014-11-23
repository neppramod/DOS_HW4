#include <sys/types.h>
#include <sys/stat.h>
#include "content.h"

#define FILE_DIRECTORY_TYPE 'd'
#define FILE_FILE_TYPE 'f'
#define CONTENT_DIR_NAME "content"

Content::Content()
{

}

void Content::createFile(char *fileName, char fileType) {

    if(fileType == FILE_DIRECTORY_TYPE){
        struct stat st = {0};

        if (stat(CONTENT_DIR_NAME, &st) == -1) {
            mkdir(CONTENT_DIR_NAME, 0777);
        }


    }else if(fileType == FILE_FILE_TYPE){

    }
}