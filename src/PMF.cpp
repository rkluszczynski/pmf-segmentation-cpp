#include <iostream>
#include <fstream>


template <class T_REAL>
PMF<T_REAL> :: PMF (T_REAL fWidth, T_REAL fHeight)
{
    fieldWidth  = fWidth;
    fieldHeight = fHeight;
    seed        = 0;
    pmfConf     = new ConfigurationList<T_REAL> (fWidth, fHeight);//NULL;
    blocksLists = NULL;
    //std::cerr << "[ PMF ] : class created" << std::endl;
}


template <class T_REAL>
PMF<T_REAL> :: ~PMF ()
{
    delete pmfConf;
    if (blocksLists) delete blocksLists;
    //std::cerr << "[ PMF ] : class deleted" << std::endl;
}


template <class T_REAL>
void
PMF<T_REAL> :: SetSeed (time_t sseed) { seed = sseed; }


template <class T_REAL>
PMF<T_REAL> *
PMF<T_REAL> :: Clone () {
    PMF<T_REAL> * newPMF = new PMF<T_REAL> (fieldWidth, fieldHeight);
    newPMF->SetSeed(seed);

    //SaveConfiguration("output/tmp.txt");
    //newPMF->LoadConfiguration("output/tmp.txt");
    newPMF->GetPMFConfiguration()->clone_from( pmfConf );
    DetermineTypesFromLeftToRight();
    return newPMF;
}

#include "generate.hpp"
#include "rotate.hpp"

#include "prepare.hpp"
#include "evolve.hpp"

#include "adding.hpp"
#include "update.hpp"
#include "remove.hpp"

#include "segment.hpp"


template <class T_REAL>
bool
PMF<T_REAL> :: IsThereIntersection()
{
    Element<pmf_point<T_REAL> > * iter1 = pmfConf->getHead();
    Element<pmf_point<T_REAL> > * iter2;
    while (iter1) {
        pmf_point<T_REAL> * pt1 = iter1->data;
        bool search1 = false;
        bool search2 = false;
        if (pt1->n1  &&  pt1->n1->x < pt1->x) search1 = true;
        if (pt1->n2  &&  pt1->n2->x < pt1->x) search2 = true;
        if (search1 || search2) {
            iter2 = pmfConf->getHead();
            while (iter2) {
                if (iter2 != iter1) {
                    pmf_point<T_REAL> * pt2 = iter2->data;
                    if (search1) {
                        if (pt2->n1  &&  pt2->n1->x <= pt2->x) {
                            if (cross3(pt1->x, pt1->y, pt1->n1->x, pt1->n1->y, pt2->x, pt2->y, pt2->n1->x, pt2->n1->y) == 1)
                                return true;
                        }
                        if (pt2->n2  &&  pt2->n2->x <= pt2->x) {
                            if (cross3(pt1->x, pt1->y, pt1->n1->x, pt1->n1->y, pt2->x, pt2->y, pt2->n2->x, pt2->n2->y) == 1)
                                return true;
                        }
                    }
                    if (search2) {
                        if (pt2->n1  &&  pt2->n1->x <= pt2->x) {
                            if (cross3(pt1->x, pt1->y, pt1->n2->x, pt1->n2->y, pt2->x, pt2->y, pt2->n1->x, pt2->n1->y) == 1)
                                return true;
                        }
                        if (pt2->n2  &&  pt2->n2->x <= pt2->x) {
                            if (cross3(pt1->x, pt1->y, pt1->n2->x, pt1->n2->y, pt2->x, pt2->y, pt2->n2->x, pt2->n2->y) == 1)
                                return true;
                        }
                    }
                }
                iter2 = iter2->next;
            }
        }
        iter1 = iter1->next;
    }
    return false;
}


template <class T_REAL>
void
PMF<T_REAL> :: BorderArtefactsRemover()
{
    Element<pmf_point<T_REAL> > * iter = pmfConf->getHead();
    stack<long> st;
    while (iter) {
        pmf_point<T_REAL> * pt = iter->data;
        if (pt->n1 != NULL  &&  pt->n2 == NULL) {
            pmf_point<T_REAL> * n1pt = pt->n1;
            if (n1pt->n1 != NULL  &&  n1pt->n2 == NULL) {
                if (pt->n1->id == n1pt->id  &&  n1pt->n1->id == pt->id) {
                    if (pt->id < n1pt->id) {
                        st.push(pt->id);
                        st.push(n1pt->id);
                    }
                }
            }
        }
        iter = iter->next;
    }
    while (! st.empty()) {
        pmfConf->remove_point_with_id(st.top());
        st.pop();
    }
    pmfConf->set_points_ids();
}


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
pmf_point<T_REAL> *
PMF<T_REAL> :: GetPointWithId(long ptid)
{
    Element<pmf_point<T_REAL> > * iter = pmfConf->getHead();
    pmf_point<T_REAL> * result = NULL;

    while (iter) {
        if (iter->data->id == ptid) {
            result = iter->data;
            break;
        }
        iter = iter->next;
    }
    return result;
}


template <class T_REAL>
bool
PMF<T_REAL> :: LoadConfiguration (const char * filename)
{
    std::cerr << std::endl <<"[ LOAD ] : loading configuration from a file '" << filename << "'" << std::endl;
    ifstream fin(filename);
    pmfConf->set_points_ids();
    pmfConf->load_configuration(fin);
    fin.close();
    DetermineTypesFromLeftToRight();
    fieldHeight = pmfConf->get_field_height();
    fieldWidth  = pmfConf->get_field_width();
    return true;
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
PMF<T_REAL> :: SaveConfigurationAsSVG (const char * filename, double scale, double strokeWidth)
{
    std::cerr << std::endl <<"[ SAVE ] : saving SVG graphics to a file '" << filename << "'" << std::endl;
    ofstream fout(filename);
    pmfConf->set_points_ids();
    pmfConf->save_svg(fout, scale, strokeWidth);
    fout.close();
    return true;
}

