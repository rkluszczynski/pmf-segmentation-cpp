#include <iostream>

#define CHECK_ASSERTIONS 1

#include "grayscaleimage.hpp"

int main()
{
    GrayScaleImage gsimg("input/szary.bmp");

    std::cout << "ROZMIAR : ";
    std::cout << gsimg.GetWidth() << " x " << gsimg.GetHeight() << std::endl;
    //*
    std::cout << gsimg.GetPixel(1, 1) << std::endl;
    std::cout << gsimg.GetPixel(1, 179) << std::endl;
    std::cout << gsimg.GetPixel(179, 1) << std::endl;
    std::cout << gsimg.GetPixel(179, 179) << std::endl;
    std::cout << std::endl;
    //*/

    std::pair<int,int> a(1,2), b(1,1), c(2,1);
    std::cout << ((a<b) ? "TAK" : "NIE") << std::endl;
    std::cout << ((a<c) ? "TAK" : "NIE") << std::endl;
    std::cout << ((b<c) ? "TAK" : "NIE") << std::endl;

    return 0;
}