#ifndef CUBEMAP_H
#define CUBEMAP_H

struct ChildCorners{
    int x[8];
    int y[8];
    int z[8];
};

struct CubeMap{
    unsigned char map;
    int x;
    int y;
    int z;
};

#endif // CUBEMAP_H
