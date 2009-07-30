#include <iostream>
#include "image.h"

namespace pmf
{
    pmfImage::pmfImage(const char * filename, long type = wxBITMAP_TYPE_ANY) { OnInit(wxString(filename, wxConvUTF8), type); }
    pmfImage::pmfImage( wxString filename, long type = wxBITMAP_TYPE_ANY)    { OnInit(filename, type); }

    pmfImage::~pmfImage()
    {
        //dtor
    }

    void
    pmfImage::OnInit(wxString filename, long type)
    {
        wxInitAllImageHandlers();
        assert( wxImage::LoadFile(filename, type) );
    }

    void
    pmfImage::StoreProcessedImage(wxImage img)
    {
        wxImage::Paste(img, 0, 0);
    }


    pmfImage::RGB *
    pmfImage::operator[](int row)
    {
        if (row < 0) row = 0;
        if (row >= GetHeight()) row = GetHeight() - 1;
        assert(0 <= row  &&  row < GetHeight());
        unsigned char * data = GetData();
        long offset = row * 3L * GetWidth();
        RGB * ptr = (RGB *)(data + offset);
        return ptr;
    }
}
