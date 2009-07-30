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


    void
    GrayscaleImage::OnInit()
    {
        StoreProcessedImage( ConvertToGreyscale() );
    }

}
