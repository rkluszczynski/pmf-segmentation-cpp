#include <cstdio>
#include <iostream>

#include "templatelist.hpp"

#define N 10

using namespace std;

int main()
{
    TemplateList<int> * tl = new TemplateList<int>();
    int * tab = new int[N];
    int   tmp = 7;

    for (int i = 0; i < N; i++) {
        tab[i] = i * 2;
        tl->push_in_order(tab+i);
    }
    tl->push_in_order(&tmp);

    cout << tl << endl;
    delete(tl);
    return(0);
}
