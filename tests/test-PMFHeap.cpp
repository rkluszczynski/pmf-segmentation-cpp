#include <iostream>
#include <cmath>
#include "abstractheap.hpp"

using namespace std;

class TestHeap : public AbstractHeap<int>
{
    private :
        double sinL, cosL;

        virtual inline bool __less_then (int a, int b)
        {
            return (a<b);
            //return (abs(a-cosL) < abs(cosL-b));
        };

    public :
        TestHeap(double ssinL, double ccosL) : sinL(ssinL), cosL(ccosL) {};
};


int main()
{
    cout << "Testing TemplateHeap ..." << endl;
    TestHeap * th = new TestHeap(0, 31);

    int ile = 7;
    cout << " DODAWANO " << ile << " : ";
    for (int i = 0; i < ile; i++) {
        int value = rand() % 100 + 1;
        cout << " " << value;
        th->insert(value);
    }
    cout << endl;

    cout << " MINIMALNE : ";
    while (! th->empty())  cout << " " << th->extract_min();
    cout << endl;

    cout << typeid(int).name() << endl;
    cout << typeid(th).name() << endl;

    delete th;
    return 0;
}
