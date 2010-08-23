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
    _counter.push_back( std::make_pair(0u, 0u) );
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
    if (id1 > id2)  std::swap(id1, id2);

    unsigned root1 = Find(id1);
    unsigned root2 = Find(id2);

    if (_rank[root1] > _rank[root2])
    {
         _parent[root2] = root1;
         _counter[root1].first += _counter[root2].first;
         _counter[root1].second += _counter[root2].second;
    }
    else if (_rank[root1] < _rank[root2])
    {
         _parent[root1] = root2;
         _counter[root2].first += _counter[root1].first;
         _counter[root2].second += _counter[root1].second;
    }
    else if (root1 != root2)
    {
         _parent[root2] = root1;
         ++_rank[root1];
         _counter[root1].first += _counter[root2].first;
         _counter[root1].second += _counter[root2].second;
    }
}
