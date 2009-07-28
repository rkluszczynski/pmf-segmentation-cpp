#ifndef GRAYSCALEIMAGE_H
#define GRAYSCALEIMAGE_H

#include "image.h"

namespace pmf
{

    class GrayscaleImage : public pmfImage
    {
        public:
            GrayscaleImage(const char *, long);
            GrayscaleImage(const wxString, long);

            virtual ~GrayscaleImage();

        protected:
        private:
            virtual void OnInit(const wxString, long);
    };

}

#endif // GRAYSCALEIMAGE_H
