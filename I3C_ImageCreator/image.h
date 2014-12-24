#ifndef IMAGE_H
#define IMAGE_H

class Image
{
public:
    Image();

    void setPath(const char* path);
    void save();

    bool isPath();
};

#endif // IMAGE_H
