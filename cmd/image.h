#ifndef IMAGE_H
#define IMAGE_H

#include <wx/image.h>

namespace pmf
{

    class pmfImage : public wxImage
    {
        public:
            typedef struct ImageRowStructure
            {
                unsigned char  * _row;
                pmfImage * _img;

                public :
                    ImageRowStructure(unsigned char *, pmfImage *);
                    unsigned char * operator[](int);
            }
            ImageRowType;

            ImageRowType operator[](int);

            pmfImage(const char *, long);
            pmfImage(const wxString, long);
            virtual ~pmfImage();

        protected:
            virtual void OnInit(const wxString, long);

        private:

    };

}

#endif // IMAGE_H
