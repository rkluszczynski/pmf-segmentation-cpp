#include <iostream>
#include "grayimage.h"

namespace pmf
{

    GrayscaleImage::GrayscaleImage(const char * filename, long type)    :  pmfImage( filename, type )  { OnInit(); }
    GrayscaleImage::GrayscaleImage(wxString filename, long type)  :  pmfImage( filename, type )  { OnInit(); }

    GrayscaleImage::~GrayscaleImage()
    {
        //dtor
    }


    long
    GrayscaleImage::GetSquarePrefixSum(int row, int col)
    {
        if (row < 0  and  col < 0) return 0;
        if (row < 0) row = 0;
        else if (row > height) row = height;
        if (col < 0) col = 0;
        else if (col > width) col = width;
        return sums[row][col];
    }


    void
    GrayscaleImage::OnInit()
    {
        StoreProcessedImage( ConvertToGreyscale() );

        // calculating square prefixes
        width = GetWidth();
        height = GetHeight();

        sums = new long * [height + 1];
        long * tmp = new long[width + 1];
        for (int r = 0; r < height; ++r)
        {
            sums[r] = new long[width + 1];
            for (int c = 0; c < width; ++c)
            {
                long value = long(GetGreen(c, r));
                tmp[c] = ((c > 0) ? tmp[c-1] : 0L) + value;
                sums[r][c] = ((r > 0) ? sums[r-1][c] : 0L) + tmp[c];
            }
            sums[r][width] = sums[r][width-1];
        }
        sums[height] = new long[width + 1];
        for (int c = 0; c < width; ++c)
            sums[height][c] = sums[height-1][c];
        sums[height][width] = sums[height-1][width-1];
    }

}
