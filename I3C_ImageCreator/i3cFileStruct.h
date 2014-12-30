#ifndef I3CFILESTRUCT_H
#define I3CFILESTRUCT_H

struct i3cPixelCube{
    unsigned char map;
    unsigned char red[8];
    unsigned char green[8];
    unsigned char blue[8];
};

struct i3cReferenceCube{
    unsigned char map;
    i3cReferenceCube *childs;
    i3cPixelCube *pixels;
};

struct i3cFile{
    int sideSize;
    int numOfLevels;
    i3cReferenceCube content;
};


#endif // I3CFILESTRUCT_H
