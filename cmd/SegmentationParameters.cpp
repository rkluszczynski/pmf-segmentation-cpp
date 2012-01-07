#include "SegmentationParameters.h"

#include <iostream>
#include <fstream>

#define VAR(V, N)       __typeof(N) V = (N)
#define FOREACH(I, C)   for(VAR(I, (C).begin()); I != (C).end(); ++I)


int SegmentationParameters::_debugTrigger = 0;


SegmentationParameters::SegmentationParameters() : m_log(std::clog)
{
    OnInit();
}

SegmentationParameters::SegmentationParameters(char * filename) : m_log(std::clog)
{
    OnInit();
    LoadConfigurationFile(filename);
}

void
SegmentationParameters::OnInit()
{
    m_IterationsNumber = 0L;
    m_PmfEpsilon = m_PMRRate = 0.;
    m_InitialFile = m_PictureFile = m_OutputDiectory = m_OutputFile = m_OutputPrefix = NULL;
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

void
SegmentationParameters::AddParameter(const std::string name, const std::string value)
{
    m_data.insert( make_pair(name, value) );
}

std::string
SegmentationParameters::GetParameter(const std::string name)
{
    return m_data[name];
}

SegmentationParameters::~SegmentationParameters()
{
    if (m_InitialFile) delete m_InitialFile;
    if (m_PictureFile) delete m_PictureFile;
    if (m_OutputFile) delete m_OutputFile;
    if (m_OutputDiectory) delete m_OutputDiectory;
    if (m_OutputPrefix) delete m_OutputPrefix;
}

SegmentationParameters::SegmentationParameters (const SegmentationParameters & other)
: m_log(std::clog)
{
    //copy ctor
    OnInit();
    m_data = other.m_data;

    m_FieldWidth = other.m_FieldWidth;
    m_FieldHeight = other.m_FieldHeight;
    m_Seed = other.m_Seed;
    m_IterationsNumber = other.m_IterationsNumber;
    m_PMRRate = other.m_PMRRate;
    m_PmfEpsilon = other.m_PmfEpsilon;

    if (other.m_PictureFile) m_PictureFile = strdup(other.m_PictureFile);
    if (other.m_InitialFile) m_InitialFile = strdup(other.m_InitialFile);
    if (other.m_OutputDiectory) m_OutputDiectory = strdup(other.m_OutputDiectory);
    if (other.m_OutputFile) m_OutputFile = strdup(other.m_OutputFile);
    if (other.m_OutputPrefix) m_OutputPrefix = strdup(other.m_OutputPrefix);
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
    fprintf(stream, "[ INFO ] : Stopping PMR (-r) = %.4lf\n", m_PMRRate);
    fprintf(stream, "\n");
}


std::ostream & operator << (std::ostream & out, const SegmentationParameters & params)
{
    out << "[ SegmentationParameters ] :  " << std::endl;
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

