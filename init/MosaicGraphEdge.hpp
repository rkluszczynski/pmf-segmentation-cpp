#ifndef MOSAICGRAPHEDGE_HPP
#define MOSAICGRAPHEDGE_HPP

#include <list>


class MosaicGraphNode;

class MosaicGraphEdge
{
    typedef std::list<MosaicGraphEdge *>::const_iterator Iterator;

    public:
        MosaicGraphEdge(unsigned int, int);
        virtual ~MosaicGraphEdge();

        unsigned int GetId() { return id; }
        //void SetId(unsigned int val) { id = val; }

        Iterator GetOther() { return other; }
        void SetOther(Iterator val) { other = val; }

        int GetDegree() { return degree; }
        void SetDegree(int val) { degree = val; }

    protected:
    private:
        unsigned int id;
        Iterator other;
        int degree;
        MosaicGraphNode * node;
};

#endif // MOSAICGRAPHEDGE_HPP
