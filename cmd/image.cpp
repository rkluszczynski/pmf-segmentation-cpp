#include "image.h"

namespace pmf
{

    pmfImage::ImageRowStructure::ImageRowStructure(unsigned char * r, pmfImage * i) : _row(r), _img(i) {}

    unsigned char *
    pmfImage::ImageRowStructure::operator[](int i)
    {
        if (i < 0) i = 0;
        if (i >= _img->GetWidth()) i = _img->GetWidth() - 1;
        assert(0 <= i  &&  i < _img->GetWidth());
        return _row + 3 * i;
    }

    pmfImage::ImageRowType
    pmfImage::operator[](int row)
    {
        if (row < 0) row = 0;
        if (row >= GetHeight()) row = GetHeight() - 1;
        assert(0 <= row  &&  row < GetHeight());
        unsigned char * data = GetData();
        long offset = row * 3 * GetWidth();
        return ImageRowStructure(data + offset, this);
    }


    pmfImage::pmfImage(const char * filename, long type = wxBITMAP_TYPE_ANY)
    {
        OnInit(wxString(filename, wxConvUTF8), type);
    }

    pmfImage::pmfImage(const wxString filename, long type = wxBITMAP_TYPE_ANY)
    {
        OnInit(filename, type);
    }

    pmfImage::~pmfImage()
    {
        //dtor
    }

    void
    pmfImage::OnInit(const wxString filename, long type)
    {
        wxInitAllImageHandlers();
        LoadFile(filename, type);
    }
}
