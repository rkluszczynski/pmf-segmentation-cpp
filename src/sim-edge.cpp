
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


    return;
}
