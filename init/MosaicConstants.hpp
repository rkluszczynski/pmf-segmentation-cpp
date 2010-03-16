#ifndef MOSAICCONSTANTS_HPP
#define MOSAICCONSTANTS_HPP

#include <cstdlib>
#include <memory>

#include "AbstractProperties.hpp"


class MosaicConstants : public AbstractProperties
{
    public:
        typedef std::auto_ptr<MosaicConstants> InstancePointer;

        inline static void SetupFile (std::string filename) { m_ImageFile = filename; }
        inline static InstancePointer & Instance()
        {
            static InstancePointer singleton(new MosaicConstants(m_ImageFile.c_str()));
            return singleton;
        }
        virtual ~MosaicConstants();

        static double         GetEpsilon()  { return Instance()->m_Epsilon; }
        static double       GetPmfHeight()  { return Instance()->m_PMFHeight; }
        static double        GetPmfWidth()  { return Instance()->m_PMFWidth; }
        static const char * GetImageFile()  { return Instance()->m_ImageFile.c_str(); }

        static const char * GetValueOfKey(const char * key) { return Instance()->GetValueOf(key).c_str(); }
        static const char * GetTimeStamp(bool anew = false) { return Instance()->GenerateTimeStamp(anew); }

        friend std::ostream & operator << (std::ostream &, const MosaicConstants &);

        static std::string m_ImageFile;

    protected:
        explicit MosaicConstants(const char *);

    private:
        const char * GenerateTimeStamp(bool);

        //inline explicit MosaicConstants (MosaicConstants const &) {}
        inline MosaicConstants & operator = (MosaicConstants const &) { return *this; }

        double m_Epsilon;
        double m_PMFHeight;
        double m_PMFWidth;
};


#endif // MOSAICCONSTANTS_HPP
