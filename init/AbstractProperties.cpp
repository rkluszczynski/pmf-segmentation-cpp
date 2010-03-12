#include "AbstractProperties.hpp"

#define VAR(V, N)       __typeof(N) V = (N)
#define FOREACH(I, C)   for(VAR(I, (C).begin()); I != (C).end(); ++I)


AbstractProperties::AbstractProperties(const char * filename)
{
    ///std::cout << "Abstract ctor = " << filename << std::endl;
    m_data = new std::map<std::string, std::string>();
    //ctor
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

        bool wasnew = m_data->insert( make_pair(index, value) ).second;
        if (!wasnew)
        {
            fprintf(stderr, "[ WARN ] : value of '%s' overwritten (by '%s')\n", index.c_str(), value.c_str());
        }
    }
    m_file = std::string(filename);
    //std::cout << m_data->size() << std::endl;
}

AbstractProperties::AbstractProperties(std::string filename)  { AbstractProperties(filename.c_str()); }


AbstractProperties::~AbstractProperties()
{
    ///std::cout << "Abstract dtor = " << std::endl;
    //dtor
    delete m_data;
}

/*
AbstractProperties::AbstractProperties(const AbstractProperties& other)
{
    std::cout << "Abstract copy ctor = " << std::endl;
    //copy ctor
    m_data = other.m_data;
}
// */

const std::string
AbstractProperties::GetValueOf(std::string key)
{
    std::map<std::string, std::string>::iterator it = m_data->find(key);
    return (it == m_data->end()) ? std::string("") : it->second;
}

const std::string
AbstractProperties::GetValueOf(const char * key)
{
    return GetValueOf(std::string(key));
}


/*
AbstractProperties& AbstractProperties::operator=(const AbstractProperties& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    //assignment operator
    return *this;
}
*/

std::ostream & operator << (std::ostream & out, const AbstractProperties & prop)
{
    out << "[ AbstractProperties ] :  file = '" << prop.m_file << "'" << std::endl;
    size_t width = 0;
    FOREACH(it, *prop.m_data) width = std::max(width, it->first.length());
    width += 5;
    FOREACH(it, *prop.m_data)
    {
        out.setf(std::ios::right);
        out.width(width);
        out << ("_" + it->first + "_");
        out.setf(std::ios::left);
        out << " = _" << it->second << "_" << std::endl;
    }
    return out;
}
