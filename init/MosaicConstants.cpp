#include "MosaicConstants.hpp"

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


std::ostream & operator << (std::ostream & out, const MosaicConstants & mc)
{
    out << "[ MosaicConstants ]" << std::endl;
    const AbstractProperties * prop = static_cast<const AbstractProperties *>(&mc);
    out << *prop << std::endl;
    return out;
}

