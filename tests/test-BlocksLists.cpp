#include <iostream>
#include <algorithm>

using namespace std;

#include "blockslists.hpp"
#include "probability.hpp"

#define TEST_SIZE 12


int main()
{
    double max_x = 4.0, max_y = 4.0, max_b = 1.4;
    cout << "test" << endl;
    BlocksLists<double> * bl = new BlocksLists<double>(max_x, max_y, max_b);
    pmf_point<double> * pts[TEST_SIZE];

    for (int i = 0; i < TEST_SIZE; i++)
    {
        double xx = Uniform<double> (max_x, 0.0);
        double yy = Uniform<double> (0.0, max_y);
        pts[i] = new pmf_point<double> (xx, yy, 0.0, 0.0, i);
        pts[i]->block = bl->determine_point_block(pts[i]);

        //cout << *pt << "    <=>   " << pt->block << endl;
        bl->push (pts[i]);
    }
    cout << bl << endl;
    bl->print_lists();

    cout << endl;
    int count = TEST_SIZE;
//    for (int i = 0; i < TEST_SIZE; i++)
    for (int i = 0; i < 1; i++)
    {
        int los = rand() % count;
        los = 9;
        --count;
        cout << endl << " _ " << los << "-{" << pts[los]->id << "} < " << endl;
        swap(pts[los], pts[count]);

        bl->pop(pts[count]);
        cout << bl << endl;
        bl->print_lists();

        for (int j = 0; j < TEST_SIZE; j++)  cout << "  " << pts[j]->id;  cout << endl;
        delete pts[count];
        pts[count] = NULL;
    }
    cout << endl;
/*
    int b = 3;
    cout << b << " :  up=" << bl->up_from(b) << "  down=" << bl->down_from(b);
    cout << "  left=" << bl->left_from(b) << "  right=" << bl->right_from(b) << endl;
*/
    delete bl;
    return 0;
}
