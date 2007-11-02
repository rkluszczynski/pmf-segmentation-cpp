#ifndef __POINT_HPP__
#define __POINT_HPP__

#include <cstdio>
#include <iostream>

#define PT_UNKNOWN		    0
#define PT_BIRTH_NORMAL		1
#define PT_BIRTH_LEFT		2
#define PT_BIRTH_UP		    3
#define PT_BIRTH_DOWN		4
#define PT_BORDER		    5
#define PT_INTERSECTION		6
#define PT_UPDATE		    7

template <typename REAL> struct pmf_point
{
	REAL x, y;
	pmf_point * n1, * n2;
	REAL l1, l2;
	long int id;
	int type;

	pmf_point ( REAL xx,  REAL yy, 	REAL ll1,  REAL ll2,  long int idi )
		: x(xx), y(yy), n1(NULL), n2(NULL), l1(ll1), l2(ll2), id(idi), type(PT_UNKNOWN)
	{}

	pmf_point ( REAL xx,  REAL yy,  REAL ll1,  REAL ll2,  long int idi,  int ttype )
		: x(xx), y(yy), n1(NULL), n2(NULL), l1(ll1), l2(ll2), id(idi), type(ttype)
	{}

	pmf_point ( const REAL xx,  const REAL yy,  pmf_point * nn1,  pmf_point * nn2,
			REAL ll1,  REAL ll2,  long int idi )
		: x(xx), y(yy), n1(nn1), n2(nn2), l1(ll1), l2(ll2), id(idi), type(PT_UNKNOWN)
	{}

	bool operator< (const pmf_point<REAL> & point)   const { return(x <  point.x); }
	//bool operator< (const pmf_point<REAL> * & point) const { return(x < point->x); }

    /**
     * Funkcja wypisujaca na ekran informacje o punkcie, dziala na wskazniku.
     **/
	friend std::ostream& operator << (std::ostream& out, const pmf_point<REAL> pt)
	{
		out << /*" " <<*/ pt.id << "`";
		out << "(" << pt.x << ";" << pt.y << ")";
		out << "[" << (pt.n1 ? (pt.n1)->id : 0) << "]";
		out << "[" << (pt.n2 ? (pt.n2)->id : 0) << "]";
		out << "{" << pt.type << "}";
		//out << " ";
		return out;
	}
};

#endif  /* __POINT_HPP__ */
