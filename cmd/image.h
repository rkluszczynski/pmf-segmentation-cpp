#ifndef IMAGE_H
#define IMAGE_H

#include <wx/image.h>

namespace pmf
{

    class Image : public wxImage
    {
        public:
            Image();
            virtual ~Image();

            char  operator[](int column);

        protected:
        private:
    };


}

#endif // IMAGE_H
