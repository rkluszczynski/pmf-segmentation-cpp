

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

#include "testing.hpp"

#include "energy.hpp"


template <class REAL>
void
PMF<REAL> :: EraseSmallPolygons ()
{
    PMFLog("[ enter ] :: EraseSmallPolygons ()");
    if (!cf) return;
    cf->SetPointsIDs ();
    int amount = GetCount();
    vector<bool> wasit(amount+1, false);
    FOREACH(it, *cf)
    {
        Point<REAL> * pt = *it;
        if (not wasit[pt->id])
        {
            Point<REAL> * start = pt;

            std::list<int> polygon;
            polygon.push_back(start->id);
            wasit[start->id] = true;

            int popid = start->id;
            pt = start->n1;

            while (pt  and  pt->id != start->id)
            {
                assert(pt->n1->id == popid  or  pt->n2->id == popid);

                if (pt->n1  and  pt->n1->id == popid)
                {
                    wasit[pt->id] = true;
                    polygon.push_back(pt->id);
                    popid = pt->id;
                    pt = pt->n2;
                }
                else if (pt->n2  and  pt->n2->id == popid)
                {
                    wasit[pt->id] = true;
                    polygon.push_back(pt->id);
                    popid = pt->id;
                    pt = pt->n1;
                }
                else
                    assert(false and "SUPPOSE NOT TO HAPPEN");
            }
            if (pt == NULL)
            {
                polygon.push_back(0);

                popid = start->id;
                pt = start->n2;
                while (pt)
                {
                    assert(pt->n1->id == popid  or  pt->n2->id == popid);

                    if (pt->n1  and  pt->n1->id == popid)
                    {
                        wasit[pt->id] = true;
                        polygon.push_front(pt->id);
                        popid = pt->id;
                        pt = pt->n2;
                    }
                    else if (pt->n2  and  pt->n2->id == popid)
                    {
                        wasit[pt->id] = true;
                        polygon.push_front(pt->id);
                        popid = pt->id;
                        pt = pt->n1;
                    }
                    else
                        assert(false and "SUPPOSE NOT TO HAPPEN 2");
                }
                polygon.push_front(0);
            }
            else
            {
                polygon.push_back(start->id);
            }
            /*
            if (!pt and !start->n2)
            {
                popid = start->id;
                pt = start->n2;

                while (pt  and  pt->id != start->id)
                {
                    if (pt->n1  and  pt->n1->id == popid)
                    {
                        wasit[pt->id] = true;
                        polygon.push_front(pt->id);
                        pt = pt->n2;
                    }
                    else if (pt->n2  and  pt->n2->id == popid)
                    {
                        wasit[pt->id] = true;
                        polygon.push_front(pt->id);
                        pt = pt->n1;
                    }
                    else
                        assert(false and "SUPPOSE NOT TO HAPPEN 2");
                }
            }
            // */
            FOREACH(it, polygon)  cout << " " << *it;
            cout << endl;
        }

    }

    PMFLog("[ leave ] :: EraseSmallPolygons ()");
}


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

    //newPMF->RotatePoints2 ();
    return newPMF;
}
