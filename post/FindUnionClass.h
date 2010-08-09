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
        unsigned GetSizeOf (unsigned id) { return _size[id]; }

        unsigned size () { return _parent.size(); }

    protected:

    private:
        std::vector<unsigned> _parent;
        std::vector<unsigned> _rank;
        std::vector<unsigned> _size;
};

#endif // FINDUNIONCLASS_H
