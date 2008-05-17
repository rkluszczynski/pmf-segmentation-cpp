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
/// TODO : to more operations
//#include "update.hpp"
//#include "delete.hpp"


template <class T_REAL>
bool
PMF<T_REAL> :: SaveConfiguration (char * filename)
{
    std::cerr << std::endl <<"[ SAVE ] : saving configuration to a file '" << filename << "'" << std::endl;
    ofstream fout(filename);
    pmfConf->set_points_ids();
    pmfConf->save_configuration(fout);
    fout.close();
    return true;
}

