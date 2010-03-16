#include "MosaicConstants.hpp"

#include <ctime>

#define VAR(V, N)       __typeof(N) V = (N)
#define FOREACH(I, C)   for(VAR(I, (C).begin()); I != (C).end(); ++I)


std::string MosaicConstants::m_ImageFile("mosaic.txt");


MosaicConstants::MosaicConstants(const char * filename) : MosaicConstants::AbstractProperties(filename)
{
    static int i = 0;
    std::cout << "[ MosaicConstants ] : " << ++i << " " << filename << std::endl;
    //ctor
    m_Epsilon = atof(GetValueOf("epsilon").c_str());
    m_PMFHeight = atof(GetValueOf("pmf-height").c_str());
    m_PMFWidth = atof(GetValueOf("pmf-width").c_str());
    m_ImageFile = GetValueOf("image");
}


MosaicConstants::~MosaicConstants()
{
    std::cout << "[ MosaicConstants ] : destroyed" << std::endl;
    //dtor
}


const char *
MosaicConstants::GenerateTimeStamp(bool anew)
{
    static char timestamp[64] = "";
    if (timestamp[0] == '\0'  or  anew)
    {
        time_t ltime;
	    struct tm *Tm;

	    ltime=time(NULL);
	    Tm=localtime(&ltime);

        sprintf(timestamp, "%4d-%02d-%02d_%02d%02d%02d",
	            Tm->tm_year + 1900,
	            Tm->tm_mon,
	            Tm->tm_mday,
	            Tm->tm_hour,
	            Tm->tm_min,
	            Tm->tm_sec);
    }
    return timestamp;
}


std::ostream & operator << (std::ostream & out, const MosaicConstants & mc)
{
    out << "[ MosaicConstants ]" << std::endl;
    const AbstractProperties * prop = static_cast<const AbstractProperties *>(&mc);
    out << *prop << std::endl;
    return out;
}

