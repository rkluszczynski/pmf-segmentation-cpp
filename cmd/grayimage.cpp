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
        else if (row < 0) return sums[height - 1][col];
        else if (col < 0) return sums[row][width - 1];
        else if (row >= height  and  col >= width)
            return sums[height - 1][width - 1];
        else
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
        for (int r = 0; r < height; ++r)
        {
            sums[r] = new long[width + 1];
            for (int c = 0; c < width; ++c)
            {
                long prev = (c > 0  and  r > 0) ? sums[r-1][c-1] : 0L;
                sums[r][c] = prev + GetGreen(r, c);
            }
        }
        for (int r = 0; r < height; ++r)
    }

}
