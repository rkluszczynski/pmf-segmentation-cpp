

template <class REAL>
PMF<REAL> :: PMF (REAL fWidth, REAL fHeight)
{ //ctor
    seed = time(NULL);
    cf   = new Configuration<REAL> (fWidth, fHeight);
    srand(seed);
}


template <class REAL>
PMF<REAL> :: ~PMF ()
{ //dtor
    delete cf;
}


template <class REAL>
void
PMF<REAL> :: SetSeed(time_t _seed)
{
    seed = _seed;
    srand(seed);
}


#include "generate.hpp"
#include "rotate.hpp"


template <class REAL>
bool
PMF<REAL> :: LoadPMF (const char * filename)
{
    wxLogMessage(wxString::Format(_("[ LOAD ] : loading configuration from a file '%s'"), filename));
    ifstream fin(filename);
    cf->LoadConfiguration(fin);
    fin.close();
    return true;
}

