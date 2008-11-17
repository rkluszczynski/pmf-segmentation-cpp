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
    GrayScaleImage gsimg("input/szary.bmp");

    std::cout << "ROZMIAR : ";
    std::cout << gsimg.GetWidth() << " x " << gsimg.GetHeight() << std::endl;
    /*
    std::cout << gsimg.GetPixelValue(1, 1) << std::endl;
    std::cout << gsimg.GetPixelValue(1, 179) << std::endl;
    std::cout << gsimg.GetPixelValue(179, 1) << std::endl;
    std::cout << gsimg.GetPixelValue(179, 179) << std::endl;
    std::cout << gsimg.GetPixelValue(279, 179) << std::endl;
    //*/

    GrayScaleImage gsimgW("input/qq-0W.bmp");
    GrayScaleImage gsimgB("input/qq-0B.bmp");

    PMF<double> pmf(7.0, 7.0);
    pmf.LoadConfiguration("input/qq.cf");

    //gsimgB.ScanVerticalLine(&pmf, 2.8);
    //gsimgW.ScanVerticalLine(&pmf, 2.8);
    //gsimg.ScanVerticalLine(&pmf, 6.0);

    std::cout << std::endl << "[ ENERGY-W ] : " << gsimgW.CalculateScanLinesEnergy(&pmf) << std::endl;
    std::cout << std::endl << "[ ENERGY-B ] : " << gsimgB.CalculateScanLinesEnergy(&pmf) << std::endl;

    printStatistics(pmf);
    printStatistics(pmf, M_PI);
    /*
    std::pair<int,int> a(1,2), b(1,1), c(2,1);
    std::cout << std::endl;
    std::cout << ((a<b) ? "TAK" : "NIE") << std::endl;
    std::cout << ((a<c) ? "TAK" : "NIE") << std::endl;
    std::cout << ((b<c) ? "TAK" : "NIE") << std::endl;
    //*/
    return 0;
}
