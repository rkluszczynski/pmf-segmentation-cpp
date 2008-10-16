#include <iostream>

#define CHECK_ASSERTIONS 1

#include "grayscaleimage.hpp"

void printStatistics(PMF<double> & pmf, double angle = 0.0)
{
    pmf.RotatePointTypes(sin(angle), cos (angle));
    std::cout << std::endl << "[ STATISTICS ]" << std::endl;
    int stats[PT_MAX_NUMBER];
    pmf.GetPMFConfiguration()->calculate_statistics(stats);
    for (int i = 0; i < PT_MAX_NUMBER; i++)  std::cout << "  " << "[" << i << "]=" << stats[i];
    std::cout << std::endl;
}

int main()
{
    GrayScaleImage gsimg("input/shape.bmp");

    std::cout << "ROZMIAR : ";
    std::cout << gsimg.GetWidth() << " x " << gsimg.GetHeight() << std::endl;
    //*
    std::cout << gsimg.GetPixel(1, 1) << std::endl;
    std::cout << gsimg.GetPixel(1, 179) << std::endl;
    std::cout << gsimg.GetPixel(179, 1) << std::endl;
    std::cout << gsimg.GetPixel(179, 179) << std::endl;
    //*/

    PMF<double> pmf(7.0, 7.0);
    pmf.LoadConfiguration("input/test.cf");

    //gsimg.ScanVerticalLine(&pmf, 0.6);
    //gsimg.ScanVerticalLine(&pmf, 6.0);

    std::cout << std::endl << "[ ENERGY ] : " << gsimg.CalculateScanLinesEnergy(&pmf) << std::endl;

    printStatistics(pmf);
    printStatistics(pmf, M_PI);

    std::pair<int,int> a(1,2), b(1,1), c(2,1);
    std::cout << std::endl;
    std::cout << ((a<b) ? "TAK" : "NIE") << std::endl;
    std::cout << ((a<c) ? "TAK" : "NIE") << std::endl;
    std::cout << ((b<c) ? "TAK" : "NIE") << std::endl;

    return 0;
}
