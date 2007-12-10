#include <iostream>
#include "heap.hpp"

using namespace std;


int main()
{
    cout << "Testing HEAP ..." << endl;
    Heap<int> * heap = new Heap<int>();

    int ile = 17;
    cout << " DODAWANO " << ile << " : ";
    for (int i = 0; i < ile; i++) {
        int value = rand() % 100 + 1;
        cout << " " << value;
        heap->heap_insert(value);
    }
    cout << endl;

    cout << " MINIMALNE " << ile << " : ";
    for (int i = 0; i < ile; i++)
        cout << " " << heap->heap_extract_min();
    cout << endl;

    delete heap;
    return 0;
}
