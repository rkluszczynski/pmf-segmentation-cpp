#ifndef MOSAICGRAPHEDGE_HPP
#define MOSAICGRAPHEDGE_HPP

#include <list>

class MosaicGraphEdge
{
    typedef std::list<MosaicGraphEdge *>::iterator Iterator;

    public:
        MosaicGraphEdge();
        virtual ~MosaicGraphEdge();

        unsigned int GetId() { return id; }
        void SetId(unsigned int val) { id = val; }

        Iterator GetIt() { return it; }
        void SetIt(Iterator val) { it = val; }

    protected:
    private:
        unsigned int id;
        Iterator it;
};

#endif // MOSAICGRAPHEDGE_HPP
