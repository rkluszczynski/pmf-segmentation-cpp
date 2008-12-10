#include <iostream>

#include "avl.hpp"

int main()
{
    AVL<int> avl;
    avl.insert( 1 );
    avl.insert( 8 );
    avl.insert( 3 );
    avl.insert( 6 );
    avl.insert( 17 );
    avl.insert( 11 );
    avl.insert( 21 );

    avl.print();
    avl.print_node( avl.find(17) );
    avl.print_node( avl.get_min() );

    avl.remove( 17 );
    avl.remove( 21 );
    avl.remove( 11 );

    avl.print();  avl.remove( 8 );

    avl.print();
    avl.print_node( avl.findLE(0) );

    return 0;
}
