#ifndef POLYGONSGRAPH_H
#define POLYGONSGRAPH_H

namespace pmf
{
    template <class REAL> class Configuration;
}


class PolygonsGraph
{
    public :
        PolygonsGraph(const char *);
        virtual ~PolygonsGraph();

    protected :

    private :
       pmf::Configuration<double> * cf;
};

#endif // POLYGONSGRAPH_H
