#include <cstdio>
#include <iostream>

#include "MosaicPMF.hpp"
#include "MosaicGraph.hpp"

using namespace std;


int main(int argc, char ** argv)
{
    double  width = 3.0;
    double height = 3.0;

    unsigned lines = 3;
    ///srand(13);
    //srand(time(NULL));

    MosaicPMF * ipmf = new MosaicPMF(width, height, lines);

    cout << ipmf << endl;

    /*
    MosaicGraph g;
    g.CreateNewNode(0., 1.);
    g.CreateNewNode(0., 2.);
    g.CreateNewNode(0., 3.);
    g.CreateNewNode(0., 4.);
    g.CreateNewNode(0., 5.);
    g.CreateNewNode(0., 6.);
    g.AddEdge(1, 5);
    g.AddEdge(1, 4);
    g.AddEdge(1, 3);
    g.AddEdge(1, 2);
    g.AddEdge(1, 1);
    g.AddEdge(1, 0);
    g.AddEdge(2, 4);

    cout << g << endl;
    // */
    return 0;
}
