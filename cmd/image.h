#ifndef IMAGE_H
#define IMAGE_H

#include <wx/image.h>

namespace pmf
{

    class pmfImage : public wxImage
    {
        public:
            typedef unsigned char RGB [3];

            RGB * operator[](int);

            pmfImage(const char *, long);
            pmfImage( wxString, long);
            virtual ~pmfImage();

        protected:
            void OnInit(const wxString, long);
            void StoreProcessedImage(wxImage);

        private:

    };

}

#endif // IMAGE_H
