#ifndef IMAGE_H
#define IMAGE_H

class Image
{
public:
    Image();

    void initializeEmpty();
    void setSideSize(int sideSize);
    void setPath(const char* path);
    bool save();

    bool isPath();
};

#endif // IMAGE_H
