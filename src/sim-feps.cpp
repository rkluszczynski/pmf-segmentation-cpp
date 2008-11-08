
#include "grayscaleimage.hpp"

inline
void
ApplyFixedEdgePointsToConfiguration ( PMF<double> * & pmf, EdgePoints<double> * ep)
{
    char plik[256];
    int fileCounter = 0;
#if pmf_LOG_ADD
    ofstream rkout("output/rk.txt");
#endif
    /*
    for (int i = 0; i < ep->getPointsNumber(); i++)
    {
        cerr << "[ EDGE ] : point " << i << "   ";
        cerr << "( " << ep->get(i)->x << " ; " << ep->get(i)->y << " )   ";
        cerr << ep->get(i)->angle << " ~ " << degree2radians(ep->get(i)->angle) << endl;

        bool isOk = true;
        do {
            PMF<double> * clone = pmf->Clone();
            //pmf->AddBirthSegment( ep->get(i)->x, ep->get(i)->y, degree2radians(ep->get(i)->angle), ep );
            pmf->AddBirthSegment( ep->get(i)->x, ep->get(i)->y, ep->get(i)->angle, ep );

            cerr << "[ EDGE ] : checking ... ";
            isOk = true;
            for (int k = 0; k < i; k++) {
                cerr << k;
                if (ep->get(k)->pt == NULL)  { cerr << "-ERR"; isOk = false; }
                cerr << " ... ";
            }
            cerr << " checked" << endl;

            if (! isOk) {
                cerr << " swaping" << endl;
                swap(pmf, clone);
                cerr << " swaped" << endl;
            }
            delete clone;
        }
        while (! isOk);

        cerr << " saving " << endl;
        sprintf(plik, "output/edge-iter-%d.txt", ++fileCounter);
        cerr << plik << endl;
        pmf->SaveConfiguration(plik);
    }
    return;
    //*/
    while (true)
    {
#if pmf_LOG_ADD
        if (fileCounter == 13) { break; }
        if (fileCounter == 12)
        {
            out.rdbuf(rkout.rdbuf());
        }
        out << "##############################################################" << endl;
        out << "################################## POINT " << fileCounter << endl;
#endif
        int i = -1;
        for (int k = 0; k < ep->getPointsNumber(); k++)
            if (ep->get(k)->pt == NULL)  { i = k; break; }
        if (i < 0) break;

        cerr << "[ EDGE ] : point " << i << "   ";
        cerr << "( " << ep->get(i)->x << " ; " << ep->get(i)->y << " )   ";
        cerr << ep->get(i)->angle << " ~ " << degree2radians(ep->get(i)->angle) << endl;

        pmf->AddBirthSegment( ep->get(i)->x, ep->get(i)->y, ep->get(i)->angle, ep );

        cerr << "[ EDGE ] : checking ... ";
        for (int k = 0; k < ep->getPointsNumber(); k++) {
            cerr << k;
            if (ep->get(k)->pt == NULL)  cerr << "-ERR";
            cerr << " ... ";
        }
        cerr << " done" << endl;

        sprintf(plik, "output/edge-iter-%d.txt", ++fileCounter);
        cerr << plik << endl;
        pmf->SaveConfiguration(plik);
    }
}


#include <cstdio>
void SimulateFixedEdgePointsSegmentation (
                            char * imageFileName,
                            PMF<double> * & pmf,
                            long iterations,
                            double PMFRate,
                            double bSize
                        )
{
    //* Initializing some variables. *
    GrayScaleImage gsimg(imageFileName);

    cerr << "getting ep " << endl;
    EdgePoints<double> * ep = gsimg.GetRandomEdgePixels(10, 10, pmf->GetPMFWidth(), pmf->GetPMFHeight());

    ep->PrintData(cerr);

    ApplyFixedEdgePointsToConfiguration(pmf, ep);
    pmf->SaveConfiguration("output/edgepoints.cf");



    // * ------------------------- *
    // *  End of simulation loop.  *
    // * ------------------------- *
    return;
}
