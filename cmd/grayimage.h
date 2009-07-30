#ifndef GRAYSCALEIMAGE_H
#define GRAYSCALEIMAGE_H

#include "image.h"

namespace pmf
{

    class GrayscaleImage : public pmfImage
    {
        public:
            GrayscaleImage(const char *, long = wxBITMAP_TYPE_ANY);
            GrayscaleImage( wxString, long = wxBITMAP_TYPE_ANY);

            virtual ~GrayscaleImage();


        protected:
        private:
            void OnInit();
    };

}

#endif // GRAYSCALEIMAGE_H
