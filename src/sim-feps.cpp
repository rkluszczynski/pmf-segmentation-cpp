
#include "grayscaleimage.hpp"


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



    // * ------------------------- *
    // *  End of simulation loop.  *
    // * ------------------------- *
    return;
}
