#ifndef ABSTRACTPROPERTIES_HPP
#define ABSTRACTPROPERTIES_HPP

#include <cstdio>
#include <cassert>

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <string>
#include <algorithm>


class AbstractProperties
{
    public:
        AbstractProperties(const std::string);
        AbstractProperties(const char *);
        virtual ~AbstractProperties();

        //AbstractProperties(const AbstractProperties& other);
        //AbstractProperties& operator=(const AbstractProperties& other);

        const std::string GetValueOf(std::string);
        const std::string GetValueOf(const char *);

        friend std::ostream & operator << (std::ostream &, const AbstractProperties &);

    protected:
        std::map<std::string, std::string> * m_data;

    private:
        std::string m_file;
};

#endif // ABSTRACTPROPERTIES_HPP
