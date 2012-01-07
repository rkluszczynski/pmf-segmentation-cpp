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
        return sums[row][col];
    }


    void
    GrayscaleImage::OnInit()
    {
        StoreProcessedImage( ConvertToGreyscale() );

        // calculating square prefixes
        width = GetWidth();
        height = GetHeight();
        sums = new long * [height];
        for (int r = 0; r < height; ++r)
        {
            sums[r] = new long[width];
            for (int c = 0; c < width; ++c)
            {
                long prev = (c > 0  and  r > 0) ? sums[r-1][c-1] : 0L;
                sums[r][c] = prev + GetGreen(r, c);
            }
        }
    }

}
