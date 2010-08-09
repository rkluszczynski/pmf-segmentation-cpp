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
    unsigned id = _parent.size();
    _parent.push_back(id);
    _rank.push_back(0u);
    _size.push_back(1u);
    return id;
}

unsigned
FindUnionClass::Find (unsigned id)
{
    if (_parent[id] == id) return id;
    _parent[id] = Find (_parent[id]);
    return _parent[id];
}

void
FindUnionClass::Union (unsigned id1, unsigned id2)
{
    unsigned root1 = Find(id1);
    unsigned root2 = Find(id2);

    if (_rank[root1] > _rank[root2])
    {
         _parent[root2] = root1;
         _size[root1] += _size[root2];
    }
    else if (_rank[root1] < _rank[root2])
    {
         _parent[root1] = root2;
         _size[root2] += _size[root1];
    }
    else if (root1 != root2)
    {
         _parent[root2] = root1;
         _rank[root1]++;
         _size[root1] += _size[root2];
    }
}
