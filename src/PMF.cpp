#include <iostream>
#include <fstream>


template <class T_REAL>
PMF<T_REAL> :: PMF (T_REAL fWidth, T_REAL fHeight)
{
    fieldWidth  = fWidth;
    fieldHeight = fHeight;
    seed        = 0;
    pmfConf     = NULL;
}


template <class T_REAL>
PMF<T_REAL> :: ~PMF ()
{
    ;
}


template <class T_REAL>
void
PMF<T_REAL> :: SetSeed (time_t sseed) { seed = sseed; }


#include "generate.hpp"
#include "rotate.hpp"
#include "adding.hpp"
/// TODO : do more operations
//#include "update.hpp"
//#include "delete.hpp"


template <class T_REAL>
pmf_point<T_REAL> *
PMF<T_REAL> :: FindClosestTo(T_REAL xx, T_REAL yy)
{
    Element<pmf_point<T_REAL> > * iter = pmfConf->getHead();
    double dist;
    pmf_point<T_REAL> * result = NULL;

    if (iter) {
        result = iter->data;
        dist = (result->x - xx)*(result->x - xx)+(result->y - yy)*(result->y - yy);
        iter = iter->next;
    }
    while (iter) {
        double tmp = (iter->data->x - xx)*(iter->data->x - xx)+(iter->data->y - yy)*(iter->data->y - yy);
        if (tmp < dist) {
            result = iter->data;
            dist = tmp;
        }
        iter = iter->next;
    }
    return result;
}


template <class T_REAL>
bool
PMF<T_REAL> :: SaveConfiguration (const char * filename)
{
    std::cerr << std::endl <<"[ SAVE ] : saving configuration to a file '" << filename << "'" << std::endl;
    ofstream fout(filename);
    pmfConf->set_points_ids();
    pmfConf->save_configuration(fout);
    fout.close();
    return true;
}


template <class T_REAL>
bool
PMF<T_REAL> :: SaveConfigurationAsSVG (const char * filename, double strokeWidth)
{
    std::cerr << std::endl <<"[ SAVE ] : saving SVG graphics to a file '" << filename << "'" << std::endl;
    ofstream fout(filename);
    pmfConf->set_points_ids();
    pmfConf->save_svg(fout, strokeWidth);
    fout.close();
    return true;
}

