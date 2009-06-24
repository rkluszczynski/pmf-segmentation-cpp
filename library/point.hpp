#ifndef POINT_HPP_INCLUDED
#define POINT_HPP_INCLUDED

#include <cstdio>

namespace pmf
{
    /*
    const int PT_UNKNOWN      = 0;
    const int PT_BIRTH_NORMAL = 1;
    const int PT_BIRTH_LEFT   = 2;
    //const int PT_BIRTH_UP	    = 3;
    //const int PT_BIRTH_DOWN   = 4;
    const int PT_BORDER       = 5;
    const int PT_INTERSECTION = 6;
    const int PT_UPDATE       = 7;
    const int PT_MAX_NUMBER   = 8;

    const int BLOCK_UNDEFINED = -1;
    //*/

    template <typename REAL> struct Point
    {
        enum {
            PT_UNKNOWN,
            PT_BIRTH_NORMAL,
            PT_BIRTH_LEFT,
            PT_BORDER,
            PT_INTERSECTION,
            PT_UPDATE,
            PT_MAX_NUMBER
        };

        REAL x, y;
        Point * n1, * n2;
        REAL l1, l2;
        long int id, oid;
        int type;

        Point ()  { init(); }

        Point ( REAL xx,  REAL yy, 	REAL ll1,  REAL ll2,  long int idi )
            : x(xx), y(yy), n1(NULL), n2(NULL), l1(ll1), l2(ll2), id(idi), type(PT_UNKNOWN)//, block(BLOCK_UNDEFINED)
        { init(); }

        Point ( REAL xx,  REAL yy,  REAL ll1,  REAL ll2,  long int idi,  int ttype )
            : x(xx), y(yy), n1(NULL), n2(NULL), l1(ll1), l2(ll2), id(idi), type(ttype)//, block(BLOCK_UNDEFINED)
        { init(); }

        Point ( REAL xx, REAL yy, Point<REAL> * nn1, Point<REAL> * nn2, REAL ll1, REAL ll2, long int idi, int ttype )
            : x(xx), y(yy), n1(nn1), n2(nn2), l1(ll1), l2(ll2), id(idi), type(ttype)//, block(BLOCK_UNDEFINED)
        { init(); }

        void init()  {} // { ++pmf_point_counter; strcpy(label, "POINT"); }

        virtual ~Point()  {} //{ /* cerr << " POINT DEAD " << endl; */ --pmf_point_counter; }

        /*
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

        bool operator< (const Point<REAL> & point)   const { return(x <  point.x); }
        //bool operator< (const pmf_point<REAL> * & point) const { return(x < point->x); }

        /**
         * Funkcja wypisujaca na ekran informacje o punkcie.
         **/
         /*
        friend std::ostream& operator << (std::ostream& out, const pmf_point<T_REAL> pt)
        {
            ++pmf_point_counter; // To log copy constructor
            out <<  pt.id << "`";
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
        //*/
    };

};


#endif // POINT_HPP_INCLUDED
