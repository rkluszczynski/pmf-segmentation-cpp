#ifndef POINT_HPP_INCLUDED
#define POINT_HPP_INCLUDED


#include "../cmd/probability.hpp"


namespace pmf
{
    long long   pmf_point_counter = 0;

        typedef enum {
            PT_Unknown,
            PT_BirthInField,
            PT_BirthOnBorder,
            PT_DeathOnBorder,
            PT_Collision,
            PT_Update,
            PT_TypesCount
        }
        PointType;


    template <typename REAL> struct Point
    {
        REAL x, y;
        Point * n1, * n2;
        REAL l1, l2;
        long int id;
        long int oid;
        PointType type;


        Point ( REAL xx,  REAL yy, 	REAL ll1,  REAL ll2,  long int idi )
            : x(xx), y(yy), n1(NULL), n2(NULL), l1(ll1), l2(ll2), id(idi), type(PT_Unknown)//, block(BLOCK_UNDEFINED)
        { OnInit(); }

        Point ( REAL xx,  REAL yy,  REAL ll1,  REAL ll2,  long int idi,  PointType ttype )
            : x(xx), y(yy), n1(NULL), n2(NULL), l1(ll1), l2(ll2), id(idi), type(ttype)//, block(BLOCK_UNDEFINED)
        { OnInit(); }

        Point ( REAL xx, REAL yy, Point<REAL> * nn1, Point<REAL> * nn2, REAL ll1, REAL ll2, long int idi, PointType ttype )
            : x(xx), y(yy), n1(nn1), n2(nn2), l1(ll1), l2(ll2), id(idi), type(ttype)//, block(BLOCK_UNDEFINED)
        { OnInit(); }

        virtual ~Point()  { /* cerr << " POINT DEAD " << endl; */ --pmf_point_counter; }

        void OnInit()  { ++pmf_point_counter; }//strcpy(label, "POINT"); }

        bool operator< (const Point<REAL> * & point) const { return(x < point->x); }
        //bool operator< (const Point<REAL> & point)   const { return(x <  point.x); }


        Point<REAL> * GenerateNeighbour (int, REAL, long &, REAL, REAL, REAL);


        friend std::ostream& operator << (std::ostream& out, const Point<REAL> * pt)
        {
            //++pmf_point_counter; // To log copy constructor
            out <<  "P" << pt->id << "=";
            out << "(" << pt->x << "," << pt->y << ")";
            out << "[" << (pt->n1 ? (pt->n1)->id : 0) << "]";
            out << "[" << (pt->n2 ? (pt->n2)->id : 0) << "]";
            if (true) {
                out << "{";
                //*
                switch(pt->type) {
                    case PT_Unknown       : out << "UKN"; break;
                    case PT_BirthInField  : out << "Bi"; break;
                    case PT_BirthOnBorder : out << "Bo"; break;
                    case PT_DeathOnBorder : out << "D"; break;
                    case PT_Collision     : out << "C"; break;
                    case PT_Update        : out << "U"; break;
                    default :   out << "??";
                }
                //*/
                out << "}";
                //if(pt->block > 0)                    out << "|" << pt.block << "|";
            }
            else { out << "{" << pt->type << "}"; }
            out << "/" << pt->l1 << "/" << pt->l2 << "/";
            //out << " ";
            return out;
        }
    };


    template <typename REAL>
    Point<REAL> *
    Point<REAL>::GenerateNeighbour(int neighborNum, REAL angle, long & ptId, REAL llength = 0.0, REAL sinL = 0.0, REAL cosL = 1.0)
    {
        using Probability::Exp;
        #define X_ROTATED(XX,YY,SSIN,CCOS) ((XX)*(CCOS)-(YY)*(SSIN))
        #define Y_ROTATED(XX,YY,SSIN,CCOS) ((XX)*(SSIN)+(YY)*(CCOS))
        REAL length = (llength == 0.0) ? Exp<REAL>(2.0) : llength;
        //cerr << " LENGTH = " << length << endl;

        REAL rotx = X_ROTATED(x, y, sinL, cosL);
        REAL roty = Y_ROTATED(x, y, sinL, cosL);
        //cerr << rotx << "  " << roty << endl;

        REAL coordX = rotx + length * cos(angle);
        REAL coordY = roty + length * sin(angle);
        //cerr << coordX << "  " << coordY << endl;
        #if CHECK_ASSERTIONS
            // FIX IT :
            assert(coordX >= rotx);
        #endif

        REAL newX = X_ROTATED(coordX, coordY, -sinL, cosL);
        REAL newY = Y_ROTATED(coordX, coordY, -sinL, cosL);
        //cerr << newX << "  " << newY << endl;

        Point<REAL> * newPt = new Point<REAL>(newX, newY, this, NULL, length, 0.0, ++ptId, PT_Update);
        switch (neighborNum)
        {
            case 1 :
                        n1 = newPt;
                        l1 = length;
                        break;;
            case 2 :
                        n2 = newPt;
                        l2 = length;
                        break;;
            default :
                        assert(false);
        }
        return newPt;
        #undef Y_ROTATED
        #undef X_ROTATED
    }

}


#endif // POINT_HPP_INCLUDED