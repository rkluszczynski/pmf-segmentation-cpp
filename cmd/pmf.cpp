

template <class REAL>
PMF<REAL> :: PMF (REAL fWidth, REAL fHeight) : out(cout.rdbuf())
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




#define Macro_DeterminePointId(PT,POPID)  \
                if (PT->n1  and  PT->n1->id == POPID) \
                { \
                    POPID = PT->id; \
                    PT = PT->n2; \
                } \
                else if (pt->n2  and  pt->n2->id == POPID) \
                { \
                    POPID = PT->id; \
                    PT = PT->n1; \
                } \
                else \
                    assert(false and "SUPPOSE NOT TO HAPPEN");

template <class REAL>
void
PMF<REAL> :: EraseSmallPolygons (REAL cutoff)
{
    PMFLog("[ enter ] :: EraseSmallPolygons ()");
    if (!cf) return;
    cf->SetPointsIDs ();
    int amount = GetCount();
    vector<bool> leaveit(amount+1, true);

    Point<REAL> * border[5];
    border[0] = new Point<REAL>(0., 0., 0., 0., -1);
    border[1] = new Point<REAL>(cf->GetFieldWidth(), 0., 0., 0., -2);
    border[2] = new Point<REAL>(cf->GetFieldWidth(), cf->GetFieldHeight(), 0., 0., -3);
    border[3] = new Point<REAL>(0., cf->GetFieldHeight(), 0., 0., -4);
    border[4] = border[0];

    vector<bool> wasit(amount+1, false);
    FOREACH(cit, *cf)
        if (not wasit[(*cit)->id])
        {
            Point<REAL> * start = *cit;

            std::list<Point<REAL> *> polygon;
            polygon.push_back(start);
            wasit[start->id] = true;

            int popid = start->id;
            Point<REAL> * pt = start->n1;
            while (pt  and  pt->id != start->id)
            {
                assert(pt->n1->id == popid  or  pt->n2->id == popid);
                wasit[pt->id] = true;
                polygon.push_back(pt);
                Macro_DeterminePointId(pt, popid);
            }
            if (pt == NULL)
            {
                polygon.push_back(NULL);
                popid = start->id;
                pt = start->n2;
                while (pt)
                {
                    assert(pt->n1->id == popid  or  pt->n2->id == popid);
                    wasit[pt->id] = true;
                    polygon.push_front(pt);
                    Macro_DeterminePointId(pt, popid);
                }
                polygon.push_front(NULL);
            }
            else
            {
                polygon.push_back(start);
            }
            //FOREACH(it, polygon)  cout << " " << ((*it) ? (*it)->id : 0);  cout << endl;

            if (not polygon.front())
            {
                cout << "chopped off" << endl;
                Point<REAL> * end1 = *(++polygon.begin());
                Point<REAL> * end2 = *(++polygon.rbegin());

                int i1 = 0;
                while (not Geometry::IsOnSegment(border[i1]->x, border[i1]->y, border[i1+1]->x, border[i1+1]->y, end1->x, end1->y) and i1 < 4) ++i1;
                //cout << " ######  " << i1 << "  ::: " << end1 << endl;

                int i2 = 0;
                while (not Geometry::IsOnSegment(border[i2]->x, border[i2]->y, border[i2+1]->x, border[i2+1]->y, end2->x, end2->y) and i2 < 4) ++i2;
                //cout << " ######  " << i2 << "  ::: " << end2 << endl;

                polygon.pop_back();
                if (i1 != i2)
                {
                    int i = i2 + 1;
                    while (i != i1)
                    {
                        polygon.push_back(border[i]);
                        (++i) %= 4;
                    }
                    polygon.push_back(border[i]);
                }
                polygon.pop_front();
                polygon.push_back(polygon.front());

            }
            else
            {
                cout << "inside" << endl;
            }
            //FOREACH(it, polygon)  cout << " " << ((*it) ? (*it)->id : 0);  cout << endl;

            double area = 0.;
            Point<REAL> * ppt = *polygon.begin();
            double x0 = ppt->x;
            double y0 = ppt->y;
            FOREACH(it, polygon)
            {
                if (it == polygon.begin()) continue;

                if ((++it) == polygon.end()) break;
                double x2 = (*it)->x;
                double y2 = (*it)->y;
                --it;
                double x1 = (*it)->x;
                double y1 = (*it)->y;
                double det = x0*y1 + x1*y2 + x2*y0 - y0*x1 - y1*x2 - y2*x0;
                //std::cout << " ... " << det << std::endl;
                area += det;
            }
            area = abs(0.5 * area);
            cout << "[ AREA VALUE ] : " << area << endl;

            if (area < cutoff)
            {
                cout << "to remove :" << endl;
                FOREACH(it, polygon)  cout << " " << ((*it) ? (*it)->id : 0);  cout << endl;
                cout << "--" << endl;
                FOREACH(it, polygon)  leaveit[(*it)->id] = false;
            }
        }

    FOREACH(cit, *cf)
    {
        if (not leaveit[(*cit)->id])
        {
            delete *cit;
            *cit = NULL;
        }
    }
    cf->ClearNullPointers();


    REP(i,4) delete border[i];
    PMFLog("[ leave ] :: EraseSmallPolygons ()");
}
#undef DeterminePointId


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
