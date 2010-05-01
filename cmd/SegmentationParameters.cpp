#include "SegmentationParameters.h"

#include <fstream>

#define VAR(V, N)       __typeof(N) V = (N)
#define FOREACH(I, C)   for(VAR(I, (C).begin()); I != (C).end(); ++I)


SegmentationParameters::SegmentationParameters()
{
    Init();
}

SegmentationParameters::SegmentationParameters(char * filename)
{
    Init();
    LoadConfigurationFile(filename);
}

void
SegmentationParameters::Init()
{
    m_InitialFile = m_PictureFile = m_OutputFile = NULL;
}

void
SegmentationParameters::LoadConfigurationFile(char * filename)
{
    std::ifstream propertiesfile(filename);
    std::string line;
    while ( getline(propertiesfile, line) )
    {
        if (line.length() == 0) continue;
        if (line[0] == '#') continue;
        if (line[0] == ';') continue;

        size_t eqpos = line.find('=');
        if (eqpos == std::string::npos) continue;

        size_t endpos = line.find(';');
        if (endpos == std::string::npos) endpos = line.length();

        std::string index = line.substr(0, eqpos);
        std::string value = line.substr(eqpos + 1, endpos - eqpos - 1);

        bool wasnew = m_data.insert( make_pair(index, value) ).second;
        if (!wasnew)
        {
            fprintf(stderr, "[ WARN ] : value of '%s' overwritten (by '%s')\n", index.c_str(), value.c_str());
        }
    }
}

SegmentationParameters::~SegmentationParameters()
{
    if (m_InitialFile) delete m_InitialFile;
    if (m_PictureFile) delete m_PictureFile;
    if (m_OutputFile) delete m_OutputFile;
}

SegmentationParameters::SegmentationParameters(const SegmentationParameters & other)
{
    //copy ctor
    m_data = other.m_data;

    m_FieldWidth = other.m_FieldWidth;
    m_FieldHeight = other.m_FieldHeight;
    m_Seed = other.m_Seed;
    m_IterationsNumber = other.m_IterationsNumber;
    m_PMRRate = other.m_PMRRate;

    m_PictureFile = m_InitialFile = m_OutputFile = NULL;
    if (other.m_PictureFile) m_PictureFile = strdup(other.m_PictureFile);
    if (other.m_InitialFile) m_InitialFile = strdup(other.m_InitialFile);
    if (other.m_OutputFile) m_OutputFile = strdup(other.m_OutputFile);
}

SegmentationParameters & SegmentationParameters::operator = (const SegmentationParameters & rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    //assignment operator
    return *this;
}


void
SegmentationParameters::PrintParameters (FILE * stream)
{
    fprintf(stream, "[ INFO ] : Field Height (-s) = %.2lf\n", m_FieldHeight);
    fprintf(stream, "[ INFO ] :  Field Width (-s) = %.2lf\n", m_FieldWidth);
    fprintf(stream, "[ INFO ] : Initial File (-i) = '%s'\n" , m_InitialFile);
    fprintf(stream, "[ INFO ] :  Output File (-o) = '%s'\n" , m_OutputFile);
    fprintf(stream, "[ INFO ] :         Seed (-e) = %li\n"  , m_Seed);
    fprintf(stream, "[ INFO ] : Picture File (-p) = '%s'\n" , m_PictureFile);
    fprintf(stream, "[ INFO ] :   Iterations (-n) = %li\n"  , m_IterationsNumber);
    fprintf(stream, "[ INFO ] : Stopping PMR (-r) = %.2lf\n", m_PMRRate);
    fprintf(stream, "\n");
}


std::ostream & operator << (std::ostream & out, const SegmentationParameters & params)
{
    out << "[ AbstractProperties ] :  "<<std::endl;
    size_t width = 0;
    FOREACH(it, params.m_data) width = std::max(width, it->first.length());
    width += 5;
    FOREACH(it, params.m_data)
    {
        out.setf(std::ios::right);
        out.width(width);
        out << ("_" + it->first + "_");
        out.setf(std::ios::left);
        out << " = _" << it->second << "_" << std::endl;
    }
    return out;
}

