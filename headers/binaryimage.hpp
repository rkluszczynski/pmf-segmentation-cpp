#ifndef BINARYIMAGE_HPP_INCLUDED
#define BINARYIMAGE_HPP_INCLUDED

#include "CImg.h"

using namespace cimg_library;


public class BinaryImage
{
    private :
        CImg<int> img();
        int ** data;
        int x, y, v, z;

    public :
        BinaryImage(char *);
        virtual ~BinaryImage();

        inline int GetWidth()   { return x; }
        inline int GetHeight()  { return y; }

        inline int GetPixel(int x, int y) { return data[x][y]; }
}


BinaryImage :: BinaryImage (char * filename)
{
    img.load(filename);
    x = img.dimx();
    y = img.dimy();
    v = img.dimv();
    z = img.dimz();

    printf("[ INFO ] : '%s' - [%i,%i,%i,%i]\n", filename, x,y,v,z);
    if (v > 1 && z > 1) {
        printf("[ WARN ] : color picture - reading only RED value\n");
    }

    data = (int **) calloc(x, sizeof(int *));
    int * p = img.data;
    for (int i = 0; i < x; i++)
    {
        data[i] = (int *) calloc(y, sizeof(int));
        memcpy(&data[i][1], p, y*sizeof(int));
        p += y;
    }
}


BinaryImage :: ~BinaryImage ()
{
    for (int i = 0; i < x; i++) free(data[i]);
    free(data);
}


#endif // BINARYIMAGE_HPP_INCLUDED
