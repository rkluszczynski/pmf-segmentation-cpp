#ifndef MOSAICCONSTANTS_HPP
#define MOSAICCONSTANTS_HPP

#include <cstdlib>

#include "AbstractProperties.hpp"


class MosaicConstants : public AbstractProperties
{
    public:
        static MosaicConstants * Instance()
        {
            static MosaicConstants * singleton = NULL;
            if (! NULL)
            {
                singleton = new MosaicConstants("input/mosaic.txt");
            }
            return singleton;
        }
        static double GetEpsilon()      { return Instance()->m_Epsilon; }
        static double GetPmfHeight()    { return Instance()->m_PMFHeight; }
        static double GetPmfWidth()     { return Instance()->m_PMFWidth; }
        static const char * GetImageFile()    { return Instance()->m_ImageFile.c_str(); }

        friend std::ostream & operator << (std::ostream &, const MosaicConstants &);

    protected:

    private:
        MosaicConstants(const char *);
        virtual ~MosaicConstants();

        double m_Epsilon;
        double m_PMFHeight;
        double m_PMFWidth;
        std::string m_ImageFile;
};


#endif // MOSAICCONSTANTS_HPP
