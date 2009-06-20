#ifndef __POINT_HPP__
#define __POINT_HPP__

#include <cstdio>
#include <iostream>

#define PT_UNKNOWN		     0
#define PT_BIRTH_NORMAL		 1
#define PT_BIRTH_LEFT		 2
#define PT_BIRTH_UP		     3
#define PT_BIRTH_DOWN		 4
#define PT_BORDER		     5
#define PT_INTERSECTION		 6
#define PT_UPDATE		     7
#define PT_MAX_NUMBER        8

#define BLOCK_UNDEFINED     -1

#include "probability.hpp"
#include "listelement.hpp"

long long pmf_point_counter = 0;

template <typename T_REAL> struct pmf_point
{
    char LABEL[8];
	T_REAL x, y;
	pmf_point * n1, * n2;
	T_REAL l1, l2;
	long int id, oid;
	int type;

	int block;
	Element<pmf_point<T_REAL> > * pointer;


	pmf_point ( T_REAL xx,  T_REAL yy, 	T_REAL ll1,  T_REAL ll2,  long int idi )
		: x(xx), y(yy), n1(NULL), n2(NULL), l1(ll1), l2(ll2), id(idi), type(PT_UNKNOWN), block(BLOCK_UNDEFINED)
	{ init(); }

	pmf_point ( T_REAL xx,  T_REAL yy,  T_REAL ll1,  T_REAL ll2,  long int idi,  int ttype )
		: x(xx), y(yy), n1(NULL), n2(NULL), l1(ll1), l2(ll2), id(idi), type(ttype), block(BLOCK_UNDEFINED)
	{ init(); }
/*
	pmf_point ( const T_REAL xx,  const T_REAL yy,  pmf_point<T_REAL> * nn1,  pmf_point<T_REAL> * nn2,
			T_REAL ll1,  T_REAL ll2,  long int idi )
		: x(xx), y(yy), n1(nn1), n2(nn2), l1(ll1), l2(ll2), id(idi), type(PT_UNKNOWN), block(BLOCK_UNDEFINED)
	{ init(); }
*/
	pmf_point ( T_REAL xx,  T_REAL yy,  pmf_point<T_REAL> * nn1,  pmf_point<T_REAL> * nn2,
            T_REAL ll1,  T_REAL ll2,  long int idi,  int ttype )
		: x(xx), y(yy), n1(nn1), n2(nn2), l1(ll1), l2(ll2), id(idi), type(ttype), block(BLOCK_UNDEFINED)
	{ init(); }

	void init()   { ++pmf_point_counter; strcpy(LABEL, "POINT"); }

	virtual ~pmf_point()  { /* cerr << " POINT DEAD " << endl; */ --pmf_point_counter; }

    //*
    #define X_ROTATED(XX,YY,SSIN,CCOS) ((XX)*(CCOS)-(YY)*(SSIN))
    #define Y_ROTATED(XX,YY,SSIN,CCOS) ((XX)*(SSIN)+(YY)*(CCOS))
    pmf_point<T_REAL> * pmf_put_new_neighbor (int neighborNum, T_REAL angle, long & ptId, T_REAL sinL, T_REAL cosL, T_REAL llength = 0.0)
    {
        T_REAL length = (llength == 0.0) ? Exp<T_REAL>(2.0) : llength;
        //cerr << " LENGTH = " << length << endl;

        T_REAL rotx = X_ROTATED(x, y, sinL, cosL);
        T_REAL roty = Y_ROTATED(x, y, sinL, cosL);
        //cerr << rotx << "  " << roty << endl;

        T_REAL coordX = rotx + length * cos(angle);
        T_REAL coordY = roty + length * sin(angle);
        //cerr << coordX << "  " << coordY << endl;

        T_REAL newX = X_ROTATED(coordX, coordY, -sinL, cosL);
        T_REAL newY = Y_ROTATED(coordX, coordY, -sinL, cosL);
        //cerr << newX << "  " << newY << endl;

        pmf_point<T_REAL> * newPt = new pmf_point<T_REAL>(newX, newY, this, NULL, length, 0.0, ++ptId, PT_UPDATE);
        if(neighborNum == 1)  { n1 = newPt; l1 = length; }
        else if(neighborNum == 2)  { n2 = newPt; l2 = length; }
        else assert(false);

        return newPt;
    }
    #undef Y_ROTATED
    #undef X_ROTATED
    //*/

	bool operator< (const pmf_point<T_REAL> & point)   const { return(x <  point.x); }
	//bool operator< (const pmf_point<REAL> * & point) const { return(x < point->x); }

    /**
     * Funkcja wypisujaca na ekran informacje o punkcie.
     **/
	friend std::ostream& operator << (std::ostream& out, const pmf_point<T_REAL> pt)
	{
	    ++pmf_point_counter; // To log copy constructor
		out << /*" " <<*/ pt.id << "`";
		out << "(" << pt.x << ";" << pt.y << ")";
		out << "[" << (pt.n1 ? (pt.n1)->id : 0) << "]";
		out << "[" << (pt.n2 ? (pt.n2)->id : 0) << "]";
        //out << "{" << pt.type << "}";
        if (true) {
            out << "{";
            switch(pt.type) {
                case PT_BIRTH_NORMAL : out << "Bn"; break;
                case PT_BIRTH_LEFT   : out << "Bl"; break;
                case PT_BIRTH_UP     : out << "Bu"; break;
                case PT_BIRTH_DOWN   : out << "Bd"; break;
                case PT_BORDER       : out << "bor"; break;
                case PT_INTERSECTION : out << "Cr"; break;
                case PT_UPDATE       : out << "U"; break;
                default :   out << "u/n";
            }
            out << "}";
            if(pt.block > 0)
                out << "|" << pt.block << "|";
        }
		out << "/" << pt.l1 << "/" << pt.l2 << "/";
		//out << " ";
		return out;
	}
};

#endif  /* __POINT_HPP__ */
