#include "FindUnionClass.h"

FindUnionClass::FindUnionClass()
{
    //ctor
}

FindUnionClass::~FindUnionClass()
{
    //dtor
}

unsigned
FindUnionClass::MakeNewSet()
{
    unsigned id = parent.size();
    parent.push_back(id);
    rank.push_back(0u);
    size.push_back(1u);
    return id;
}

unsigned
FindUnionClass::Find (unsigned id)
{
    if (parent[id] == id) return id;
    parent[id] = Find (parent[id]);
    return parent[id];
}

void
FindUnionClass::Union (unsigned id1, unsigned id2)
{
    unsigned root1 = Find(id1);
    unsigned root2 = Find(id2);

    if (rank[root1] > rank[root2])
    {
         parent[root2] = root1;
         size[root1] += size[root2];
    }
    else if (rank[root1] < rank[root2])
    {
         parent[root1] = root2;
         size[root2] += size[root1];
    }
    else if (root1 != root2)
    {
         parent[root2] = root1;
         rank[root1]++;
         size[root1] += size[root2];
    }
}
