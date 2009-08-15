

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
    PMF<REAL> * newPMF = new PMF<REAL> (GetWidth(), GetHeight());
    newPMF->SetSeed(seed);

    //SaveConfiguration("output/tmp.txt");
    //newPMF->LoadConfiguration("output/tmp.txt");
    //newPMF->GetPMFConfiguration()->clone_from( pmfConf );
    //DetermineTypesFromLeftToRight();

    /*
    if (originalPMF == NULL) return;
    if (TemplateList<pmf_point<T_REAL> >::get_size() > 0)  destroy();

    TemplateList<pmf_point<T_REAL> >::head = NULL;
    TemplateList<pmf_point<T_REAL> >::tail = NULL;
    TemplateList<pmf_point<T_REAL> >::size = 0;

    originalPMF->set_points_ids();
    fieldHeight = originalPMF->get_field_height();
    fieldWidth  = originalPMF->get_field_width();
    int  amount = originalPMF->get_size();

    pmf_point<T_REAL> ** ptTab = new pmf_point<T_REAL> * [amount+1];
    long *  firstIds = new long[amount+1];
    long * secondIds = new long[amount+1];

    Element<pmf_point<T_REAL> > * iter = originalPMF->getHead();
    while (iter) {
        pmf_point<T_REAL> * opt = iter->data;
        firstIds[opt->id]  = ((opt->n1) ? opt->n1->id : 0);
        secondIds[opt->id] = ((opt->n2) ? opt->n2->id : 0);
        //ptTab[opt->id] = new pmf_point<T_REAL>(opt->x, opt->y, NULL, NULL, opt->l1, opt->l2, opt->id, PT_UNKNOWN);
        ptTab[opt->id] = new pmf_point<T_REAL>(opt->x, opt->y, NULL, NULL, opt->l1, opt->l2, opt->id, opt->type);
        iter = iter->next;
    }

    for (int i = 1; i <= amount; i++)
    {
        ptTab[i]->n1 = (firstIds[i] > 0) ? ptTab[firstIds[i]] : NULL;
        ptTab[i]->n2 = (secondIds[i] > 0) ? ptTab[secondIds[i]] : NULL;
    }
    for (int i = 1; i <= amount; i++)
        TemplateList<pmf_point<T_REAL> >::push_back(ptTab[i]);

    delete[] secondIds;
    delete[] firstIds;
    delete[] ptTab;
    //*/
    return newPMF;
}
