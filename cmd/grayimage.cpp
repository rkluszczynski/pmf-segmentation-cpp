#include "grayimage.h"

namespace pmf
{

    GrayscaleImage::~GrayscaleImage()
    {
        //dtor
    }

    void
    GrayscaleImage::OnInit(const wxString filename, long type)
    {
        pmfImage::OnInit(filename, type);
        ConvertToGreyscale();
    }

}
