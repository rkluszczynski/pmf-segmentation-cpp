#ifndef MOSAICGRAPHEDGE_HPP
#define MOSAICGRAPHEDGE_HPP

#include <cassert>

#include <list>
#include <iostream>


class MosaicGraphNode;

class MosaicGraphEdge
{
    typedef std::list<MosaicGraphEdge *>::iterator Iterator;

    public:
        MosaicGraphEdge(unsigned int, int);
        MosaicGraphEdge(const MosaicGraphEdge &);
        virtual ~MosaicGraphEdge();

        unsigned int GetId() { return id; }
        void SetId(unsigned int val) { id = val; }

        Iterator GetOther() { return other; }
        void SetOther(Iterator val) { other = val; }

        int GetDegree() { return degree; }
        void SetDegree(int val) { degree = val; }

        friend std::ostream & operator << (std::ostream &, const MosaicGraphEdge *);

    protected:

    private:
        unsigned int id;
        Iterator other;
        int degree;
        MosaicGraphNode * node;
};

#endif // MOSAICGRAPHEDGE_HPP
