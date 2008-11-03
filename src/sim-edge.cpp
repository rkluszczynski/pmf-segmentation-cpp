#include "PMF.hpp"
#include "edgepoints.hpp"

#define EDGE_STRATEGY_ONE_SHOT                1
#define EDGE_STRATEGY_TRY_FIRST_WRONG_FOREVER 2

void SimulateEdgesApplier (
                            char * edgesFile,
                            PMF<double> * pmf,
                            double bSize = 0.0,
                            int strategy = EDGE_STRATEGY_ONE_SHOT
                        )
{
    EdgePoints<double> ep;
    ep.LoadData(edgesFile);
    ep.PrintData();

    if (strategy == EDGE_STRATEGY_ONE_SHOT)
    {
        pmf->SaveConfiguration("output/before-edges.txt");

        char plik[256];
        for (int i = 0; i < ep.getPointsNumber(); i++)
        {
            cerr << "[ EDGE ] : point " << i << "   ";
            cerr << "( " << ep.get(i)->x << " ; " << ep.get(i)->y << " )   ";
            cerr << ep.get(i)->angle << " ~ " << degree2radians(ep.get(i)->angle) << endl;

            pmf->AddBirthSegment( ep.get(i)->x, ep.get(i)->y, degree2radians(ep.get(i)->angle), &ep );

            cerr << "[ EDGE ] : checking ... ";
            for (int k = 0; k < i; k++) {
                cerr << k;
                if (ep.get(k)->pt == NULL)  cerr << "-ERR";
                cerr << " ... ";
            }
            cerr << " done" << endl;

            sprintf(plik, "output/edge-iter-%d.txt", i);
            cerr << plik << endl;
            pmf->SaveConfiguration(plik);
        }
    }
    else if (strategy == EDGE_STRATEGY_TRY_FIRST_WRONG_FOREVER)
    {
        pmf->SaveConfiguration("output/before-edges.txt");
        char plik[256];
        int fileCounter = 0;
        while (true)
        {
            int i = -1;
            for (int k = 0; k < ep.getPointsNumber(); k++)
                if (ep.get(k)->pt == NULL)  { i = k; break; }
            if (i < 0) break;

            cerr << "[ EDGE ] : point " << i << "   ";
            cerr << "( " << ep.get(i)->x << " ; " << ep.get(i)->y << " )   ";
            cerr << ep.get(i)->angle << " ~ " << degree2radians(ep.get(i)->angle) << endl;

            pmf->AddBirthSegment( ep.get(i)->x, ep.get(i)->y, degree2radians(ep.get(i)->angle), &ep );

            cerr << "[ EDGE ] : checking ... ";
            for (int k = 0; k < ep.getPointsNumber(); k++) {
                cerr << k;
                if (ep.get(k)->pt == NULL)  {
                    cerr << "-ERR";
                }
                cerr << " ... ";
            }
            cerr << " done" << endl;

            sprintf(plik, "output/edge-iter-%d.txt", ++fileCounter);
            cerr << plik << endl;
            pmf->SaveConfiguration(plik);
        }
    }
    return;
}
