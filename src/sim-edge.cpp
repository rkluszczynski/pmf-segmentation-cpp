
#include "PMF.hpp"
#include "edgepoints.hpp"

void SimulateAddingEdges (
                            char * edgesFile,
                            PMF<double> * pmf
                        )
{
    EdgePoints<double> ep;
    ep.LoadData(edgesFile);
    ep.PrintData();

    PMF<double> * pmf2 = pmf->Clone();
    pmf2->SaveConfiguration("output/pmf2.txt");
    delete pmf2;

    char plik[256];
    for (int i = 0; i < ep.getPointsNumber(); i++)
    {
        cerr << "[ EDGE ] : point " << i << "   ";
        cerr << "( " << ep.get(i)->x << " ; " << ep.get(i)->y << " )   ";
        cerr << ep.get(i)->angle << " ~ " << degree2radians(ep.get(i)->angle) << endl;

        sprintf(plik, "output/edge-iter-%d.txt", i);
        cerr << plik << endl;
        pmf->SaveConfiguration(plik);
    }

    return;
}