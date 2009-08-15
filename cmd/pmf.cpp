

template <class REAL>
PMF<REAL> :: PMF (REAL fWidth, REAL fHeight)
{ //ctor
    seed = time(NULL);
    cf   = new Configuration<REAL> (fWidth, fHeight);
    //srand(seed);
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


#include "common.hpp"
#include "generate.hpp"

#include "rotate.hpp"
#include "prepare.hpp"
#include "evolve.hpp"

#include "adding.hpp"
#include "update.hpp"
#include "remove.hpp"
#include "addseg.hpp"

#include "energy.hpp"


template <class REAL>
bool
PMF<REAL> :: LoadPMF (const char * filename)
{
    PMFLog("[ LOAD ] : configuration from a file '%s'", wxString::FromAscii(filename).c_str());
    ifstream fin(filename);
    cf->LoadConfiguration(fin);
    fin.close();
    return true;
}


template <class REAL>
bool
PMF<REAL> :: SavePMF (const char * filename, PMFFileType mode = TextFile)
{
    PMFLog("[ SAVE ] : configuration to a file '%s'", wxString::FromAscii(filename).c_str());
    cf->SetPointsIDs();
    switch (mode)
    {
        case GeoGebraFile :
        {
            cf->SaveConfigurationAsGGB(filename);
            break;;
        }
        case GeoGebraCmds :
        {
            ofstream fout(filename);
            cf->SaveConfigurationAsGGB(fout);
            fout.close();
            break;;
        }
        default :
        {
            ofstream fout(filename);
            cf->PrintConfiguration(fout);
            fout.close();
        }
    }
    return true;
}


template <class REAL>
PMF<REAL> *
PMF<REAL> :: Clone ()
{
    if (!cf) return NULL;

    PMF<REAL> * newPMF = new PMF<REAL> (GetWidth(), GetHeight());
    //newPMF->SetSeed(seed);

    cf->SetPointsIDs ();
    int amount = GetCount();

    Point<REAL> ** pts = new Point<REAL> * [amount+1];
    long *    firstIds = new long[amount+1];
    long *   secondIds = new long[amount+1];

    FOREACH(it, *cf)
    {
        Point<REAL> * opt = *it;

        firstIds[opt->id]  = ((opt->n1) ? opt->n1->id : 0);
        secondIds[opt->id] = ((opt->n2) ? opt->n2->id : 0);
        pts[opt->id] = new Point<REAL>(opt->x, opt->y, NULL, NULL, opt->l1, opt->l2, opt->id, opt->type);
    }
    FOR(i, 1, amount)
    {
        pts[i]->n1 =  (firstIds[i] > 0) ? pts[firstIds[i]] : NULL;
        pts[i]->n2 = (secondIds[i] > 0) ? pts[secondIds[i]] : NULL;
    }
    FOR(i, 1, amount)  newPMF->PushBack(pts[i]);

    delete[] secondIds;
    delete[] firstIds;
    delete[] pts;

    newPMF->RotatePoints2 ();
    return newPMF;
}
