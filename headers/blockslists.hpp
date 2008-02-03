#ifndef BLOCKSLISTS_HPP_INCLUDED
#define BLOCKSLISTS_HPP_INCLUDED

#include <iostream>
#include <vector>
#include <assert.h>

#include "blocklist.hpp"


template <class T_REAL>
class BlocksLists
{
    private :
        T_REAL fieldHeight, fieldWidth;
        T_REAL blockHeight, blockWidth;

        std::vector<T_REAL> hLimit, vLimit;
        long blocksCount;
        std::vector<BlockList<T_REAL> > blocks;

    public :
        BlocksLists(T_REAL, T_REAL, T_REAL);
        ~BlocksLists();

        int determine_point_block (pmf_point<T_REAL> *);
        void push (pmf_point<T_REAL> *);
        void print_lists ();
        void pop (pmf_point<T_REAL> *);

        int    up_from (int block) { return (block/hLimit.size() == 0) ? BLOCK_UNDEFINED : block-hLimit.size(); }
        int  down_from (int block) { return (block/hLimit.size() == vLimit.size()-1) ? BLOCK_UNDEFINED : block+hLimit.size(); }
        int  left_from (int block) { return (block%hLimit.size() == 0) ? BLOCK_UNDEFINED : block-1; }
        int right_from (int block) { return (block%hLimit.size() == hLimit.size()-1) ? BLOCK_UNDEFINED : block+1; }

        BlockList<T_REAL> * getBlockList (int block)  { return (&(blocks[block])); }

		friend std::ostream & operator << (std::ostream & out, const BlocksLists<T_REAL> * blists)
		{
			out << std::endl << "[ BLOCKS ] :> ";
			out << " blocksCount = " << blists->blocksCount;
			out << "  ( " << blists->blocks.size() << " )  ";
			out << std::endl;
			out << "      horizontal : ";
			for (unsigned int i = 0; i < blists->hLimit.size(); i++)  std::cout << " " << blists->hLimit[i];
			out << std::endl;
			out << "        vertical : ";
			for (unsigned int i = 0; i < blists->vLimit.size(); i++)  std::cout << " " << blists->vLimit[i];
			out << std::endl;
            for (int i = 0; i < blists->blocksCount; i++)  std::cout << "# " << i << " : " << &(blists->blocks[i]);
			return out;
		}
};


template <class T_REAL>
void BlocksLists<T_REAL>::pop (pmf_point<T_REAL> * pt)
{
    assert(pt->block != BLOCK_UNDEFINED);
    blocks[pt->block].pop_point(pt);
    pt->pointer = NULL;
}


template <class T_REAL>
void BlocksLists<T_REAL>::print_lists ()
{
    for (int i = 0; i < blocksCount; i++) {
        std::cout << i << " : ";
        blocks[i].print_points();
    }
}


template <class T_REAL>
int BlocksLists<T_REAL>::determine_point_block (pmf_point<T_REAL> * pt)
{
    int v = 0, h = 0;
    while (hLimit[h] < pt->x) h++;
    while (vLimit[v] < pt->y) v++;
    return v * hLimit.size() + h;
}


template <class T_REAL>
void BlocksLists<T_REAL>::push (pmf_point<T_REAL> * pt)
{
    assert(pt->block != BLOCK_UNDEFINED);
    if (pt->block != BLOCK_UNDEFINED)  blocks[pt->block].push_back(pt);
    else  std::cerr << "[ ERROR ] : pushing to blocks list without block number defined" << std::endl;
}


/**
 * Konstruktor klasy.
 **/
template <class T_REAL>
BlocksLists<T_REAL>::BlocksLists (T_REAL fwidth, T_REAL fheight, T_REAL bsize)
: fieldWidth(fwidth), fieldHeight(fheight), blockHeight(bsize), blockWidth(bsize)
{
	assert(blockHeight <= fieldHeight && blockWidth <= fieldWidth);

	T_REAL tmp = 0.0;
	while ((tmp += blockWidth) < fieldWidth)  hLimit.push_back(tmp);
    hLimit.push_back(fieldWidth);
	tmp = 0.0;
	while ((tmp += blockHeight) < fieldHeight)  vLimit.push_back(tmp);
    vLimit.push_back(fieldHeight);

    blocksCount = hLimit.size() * vLimit.size();
    BlockList<T_REAL> empty;
    for (int i = 0; i < blocksCount; i++)  blocks.push_back(empty);

	std::cout << "[ BLOCKS ]  created"; // << std::endl;
    std::cout << " ...  " << hLimit.size() << " x " << vLimit.size() << std::endl;
};


template <class T_REAL>
BlocksLists<T_REAL>::~BlocksLists ()
{
	std::cout << "[ BLOCKS ]  destroyed" << std::endl;
};


#endif // BLOCKSLISTS_HPP_INCLUDED
