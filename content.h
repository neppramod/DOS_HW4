#ifndef  CONTENT_H
#define  CONTENT_H

class  Content{

public:
    Content();
    // For creating a file or directory
    // fileName: file or directory name, fileType: file(f) or directory(d) type
    void createFile(char *fileName, char fileType);

};

#endif