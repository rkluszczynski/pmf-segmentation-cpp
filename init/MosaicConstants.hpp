#ifndef MOSAICCONSTANTS_HPP
#define MOSAICCONSTANTS_HPP

#include <cstdlib>
#include <memory>

#include "AbstractProperties.hpp"


class MosaicConstants : public AbstractProperties
{
    public:
        inline static MosaicConstants & Instance()
        {
            static std::auto_ptr<MosaicConstants> singleton(new MosaicConstants("input/mosaic.txt"));
            return *singleton;
        }

        static double GetEpsilon()      { return Instance().m_Epsilon; }
        static double GetPmfHeight()    { return Instance().m_PMFHeight; }
        static double GetPmfWidth()     { return Instance().m_PMFWidth; }
        static const char * GetImageFile()    { return Instance().m_ImageFile.c_str(); }

        friend std::ostream & operator << (std::ostream &, const MosaicConstants &);

        MosaicConstants(const char *);
        virtual ~MosaicConstants();

    protected:

    private:

        //inline explicit MosaicConstants(MosaicConstants const&) {}
        //inline MosaicConstants& operator=(MosaicConstants const&) { return *this; }

        double m_Epsilon;
        double m_PMFHeight;
        double m_PMFWidth;
        std::string m_ImageFile;
};


#endif // MOSAICCONSTANTS_HPP
