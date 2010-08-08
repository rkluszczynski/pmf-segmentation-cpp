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

        bool IsRoot (unsigned id) { return parent[id] == id; }

    protected:

    private:
        std::vector<unsigned> parent;
        std::vector<unsigned> rank;
        std::vector<unsigned> size;
};

#endif // FINDUNIONCLASS_H
