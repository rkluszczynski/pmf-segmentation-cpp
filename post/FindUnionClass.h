#ifndef FINDUNIONCLASS_H
#define FINDUNIONCLASS_H

#include <vector>

class FindUnionClass
{
    public:
        FindUnionClass();
        virtual ~FindUnionClass();

        unsigned MakeNewSet ();
        unsigned Find (unsigned);
        void Union (unsigned, unsigned);

        bool IsRoot (unsigned id) { return _parent[id] == id; }
        std::pair<unsigned, unsigned> & GetCounterOf (unsigned id) { return _counter[id]; }

        unsigned size () { return _parent.size(); }

    protected:

    private:
        std::vector<unsigned> _parent;
        std::vector<unsigned> _rank;
        std::vector<std::pair<unsigned, unsigned> > _counter;
};

#endif // FINDUNIONCLASS_H
