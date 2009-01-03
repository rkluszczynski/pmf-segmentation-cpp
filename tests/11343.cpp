#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <set>
#include <cstdio>

using namespace std;

#define VAR(V,N)        __typeof(N) V = (N)
#define SIZE(X)         ((int)(X).size())
#define FOR(X,B,E)      for(int X = B; X <= (E); ++X)
#define REP(X,N)        for(int X = 0; X < (N); ++X)
#define FOREACH(I,C)    for(VAR(I, (C).begin()); I != (C).end(); ++I)

#define ST first
#define ND second
#define PB push_back

#define TYPE double


template <class Real = double>
struct Point
{
    private :
        Real x, y, EPS;

        inline bool IsZero(double x) { return x >= -EPS && x <= EPS; }

    public :
        Point() : x(-1), y(-1), EPS(10e-9) { }
        Point(Real xx, Real yy) : x(xx), y(yy) { }

        const Real GetX() const { return x; }
        const Real GetY() const { return y; }
        /*
        Point & operator = (const Point<Real> & other)
        {
            if (this != &other)
            {
                x = other.GetX();
                y = other.GetY();
            }
            return *this;
        }
        //*/
        //bool operator ==(const Point<Real> & pt) const { return pt.x == x && pt.y == y; }
        bool operator ==(const Point<Real> & pt) const { return IsZero(pt.x-x) && IsZero(pt.y-y); }
        bool operator < (const Point<Real> & pt) const { return x < pt.x || (pt.x == x  &&  y < pt.y); }

        friend ostream & operator << (ostream & out, const Point<Real> pt)
        {
            out << "(" << pt.GetX() << ";" << pt.GetY() << ")";
            return out;
        }
};


//error: passing const as this argument of operator= discards qualifiers

template <class Real = double>
struct Segment
{
    private :
        Point<Real> p1, p2;

    public :

        int num;

        Segment() { }
        Segment(Point<Real> pp1, Point<Real> pp2) : p1(pp1), p2(pp2) { }
        /*
        Segment<Real> & operator = (const Segment<Real> & other)
        {
            if (this != &other)
            {
                Point<Real> pp1(other.GetP1().GetX(), other.GetP1().GetY());
                Point<Real> pp2(other.GetP2().GetX(), other.GetP2().GetY());
                p1 = pp1;
                p2 = pp2;
            }
            return *this;
        }
        //*/
        const Point<Real> GetP1() const { return p1; }
        const Point<Real> GetP2() const { return p2; }

        friend ostream & operator << (ostream & out, const Segment<Real> seg)
        {
            out << seg.GetP1() << "-" << seg.GetP2();
            return out;
        }
};


class MutableSegmentSet;

class fCompare
{
    //typedef pair<Point<double>, pair<int,int> > ENTRY;
    typedef Segment<double> ENTRY;
    public :
        fCompare(const MutableSegmentSet & set) : m_set(set) {}
        bool operator() (const ENTRY & lhs, const ENTRY & rhs) const;
    private :
        const MutableSegmentSet & m_set;
};

class MutableSegmentSet
{
    //typedef pair<Point<double>, pair<int,int> > ENTRY;
    typedef Segment<double> ENTRY;

    public :
        typedef std::set<ENTRY, fCompare> SegSet;
        typedef SegSet::iterator Iterator;
        typedef SegSet::const_iterator ConstInterator;

        MutableSegmentSet() : m_set(fCompare(*this)), m_scan_pos(0.0) {}

        void SetCoord(double x) { m_scan_pos = x; }
        void SetCoord(double x, ENTRY & seg1, ENTRY & seg2)
        {
            Erase(Find(seg1));
            Erase(Find(seg2));
            SetCoord(x);
            Insert(seg1);
            Insert(seg2);
        }
        double GetCoord() const { return m_scan_pos; }

        // methods: begin, end
        Iterator begin() { return m_set.begin(); }
        Iterator end() { return m_set.end(); }

        // methods: insert, erase, find
        pair<Iterator,bool> Insert(ENTRY & ent)
        {
            pair<Iterator,bool> res = m_set.insert(ent);
            return res;
        }
        void Erase(Iterator it) { m_set.erase(it); }
        Iterator Find(ENTRY & ent) { return m_set.find(ent); }

    private :
        std::set<ENTRY, fCompare> m_set;
        double m_scan_pos;
};

bool fCompare::operator() (const ENTRY & lhs, const ENTRY & rhs) const
{
    double x_scan_pos = m_set.GetCoord() + 10e-9;

    double tgL1 = (lhs.GetP2().GetY() - lhs.GetP1().GetY()) / (lhs.GetP2().GetX() - lhs.GetP1().GetX());
    double b1 = lhs.GetP2().GetY() - tgL1 * lhs.GetP2().GetX();

    double ly_scan_pos = tgL1 * x_scan_pos + b1;

    double tgL2 = (rhs.GetP2().GetY() - rhs.GetP1().GetY()) / (rhs.GetP2().GetX() - rhs.GetP1().GetX());
    double b2 = rhs.GetP2().GetY() - tgL2 * rhs.GetP2().GetX();

    double ry_scan_pos = tgL2 * x_scan_pos + b2;

    cout << " . " << ly_scan_pos << " < " << ry_scan_pos << " ?  : " << ((ly_scan_pos < ry_scan_pos) ? "TAK" : "NIE") << endl;
    return ly_scan_pos < ry_scan_pos;
};


class SegmentsIntersetions
{
    typedef Point<TYPE> POINT;
    typedef Segment<TYPE> SEGMENT;

    private :
        const double EPS;
        vector<SEGMENT> _segs;
        vector<pair<POINT,pair<int, int> > > result;

        inline bool IsZero(double x) { return x >= -EPS && x <= EPS; }
        int sgnDet(const POINT & p, const POINT & q, const POINT & r )
        {
            TYPE tmp = p.GetX()*q.GetY() + r.GetX()*p.GetY() + q.GetX()*r.GetY() - r.GetX()*q.GetY() - q.GetX()*p.GetY() - p.GetX()*r.GetY();
            if(IsZero(tmp)) return 0;
            else if(tmp > 0.0) return(1);
            else return(-1);
        }
        int isOnSegment(const POINT & p, const POINT & q, const POINT & r )
            /* Return values:
             *   1 - point r belongs to a segment p-q
             *   0 - point is outside the segment
             */
        {
            if( sgnDet(p, q, r) == 0
                &&  min(p.GetX(),q.GetX()) <= r.GetX()  &&  r.GetX() <= max(p.GetX(),q.GetX())
                &&  min(p.GetY(),q.GetY()) <= r.GetY()  &&  r.GetY() <= max(p.GetY(),q.GetY())
            )
                return(1);
            return(0);
        }
        int cross3(const SEGMENT & seg1 /* Line 1 ( p-q ) */, const SEGMENT & seg2 /* Line 2 ( r-s ) */)
            /* Returning values:
             *    0  - lines do not cross
             *    1  - lines crosses each others
             *  2..5 - end of one line belongs to other line
             */
        {
            int sgnDetPQR, sgnDetPQS, sgnDetRSP, sgnDetRSQ;

            sgnDetPQR = sgnDet(seg1.GetP1(), seg1.GetP2(), seg2.GetP1());
            sgnDetPQS = sgnDet(seg1.GetP1(), seg1.GetP2(), seg2.GetP2());
            sgnDetRSP = sgnDet(seg2.GetP1(), seg2.GetP2(), seg1.GetP1());
            sgnDetRSQ = sgnDet(seg2.GetP1(), seg2.GetP2(), seg1.GetP2());

            if( (sgnDetPQR != 0  ||  sgnDetPQS != 0)  &&
                (sgnDetRSP != 0  ||  sgnDetRSQ != 0)  &&
                sgnDetPQR == - sgnDetPQS  &&  sgnDetRSP == - sgnDetRSQ
            )
                return(1);
            if(isOnSegment(seg1.GetP1(), seg1.GetP2(), seg2.GetP1()) == 1) return(2);
            if(isOnSegment(seg1.GetP1(), seg1.GetP2(), seg2.GetP2()) == 1) return(3);
            if(isOnSegment(seg2.GetP1(), seg2.GetP2(), seg1.GetP1()) == 1) return(4);
            if(isOnSegment(seg2.GetP1(), seg2.GetP2(), seg1.GetP2()) == 1) return(5);
            return(0);
        }
        POINT crosspoint2(const POINT & p3, const POINT & p4, const POINT & p1, const POINT & p2 )
        {
            TYPE a, b, c, d, x, y;
            if( p1.GetX() != p2.GetX() )
            {
                a = (p1.GetY() - p2.GetY()) / (p1.GetX() - p2.GetX());
                b = p1.GetY() - a * p1.GetX();
                if( p3.GetX() != p4.GetX() )
                {
                    c = (p3.GetY() - p4.GetY()) / (p3.GetX() - p4.GetX());
                    d = p3.GetY() - c * p3.GetX();

                    x = (d - b)/(a - c);
                    y = a * (x) + b;
                }
                else {
                    x = p3.GetX();
                    y = a * (x) + b;
                }
            }
            else {
                c = (p3.GetY() - p4.GetY()) / (p3.GetX() - p4.GetX());
                d = p3.GetY() - c * p3.GetX();

                x = p1.GetX();
                y = c * (x) + d;
            }
            POINT cpt(x, y);
            return cpt;
        }

    public :
        SegmentsIntersetions(int n = 0) : _segs(n), EPS(10e-9) { }
        ~SegmentsIntersetions() { }

        SEGMENT & operator[] (const int index) { return _segs[index]; }

        void PushBack(TYPE x1, TYPE y1, TYPE x2, TYPE y2)
        {
            POINT p1(x1, y1), p2(x2, y2);
            SEGMENT seg(p1, p2);
            seg.num = SIZE(_segs);
            _segs.push_back(seg);
        }

        inline
        void UpdateCrossings(MutableSegmentSet::Iterator & iit1, MutableSegmentSet::Iterator & iit2, set<pair<POINT,pair<int, int> > > & pptset, double x_sweep)
        {
            cout << " ... checking " << *iit1 << " and " << *iit2;
            if (cross3(*iit1, *iit2) == 1)
            {
                POINT cpt = crosspoint2(iit1->GetP1(), iit1->GetP2(), iit2->GetP1(), iit2->GetP2());
                cout << " crossed at " << cpt;
                if (x_sweep < cpt.GetX())
                    pptset.insert( pair<POINT, pair<int, int> >(cpt, pair<int, int>(min(iit1->num, iit2->num), max(iit1->num, iit2->num))) );
                else
                    cout << " NOT TAKEN ";
            }
            else cout << " not crossed ";
            cout << endl;
        }

        void Execute()
        {
            set<pair<POINT,pair<int, int> > > ptset;
            MutableSegmentSet mset;

            REP(i, SIZE(_segs))
            {
                ptset.insert( pair<POINT, pair<int, int> >(_segs[i].GetP1(), pair<int, int>(i, -1)) );
                ptset.insert( pair<POINT, pair<int, int> >(_segs[i].GetP2(), pair<int, int>(i, -1)) );
            }

            vector<bool> started(SIZE(_segs));
            fill(started.begin(), started.end(), false);

            int qq = 0;
            int stopNum = 18;
            while (ptset.begin() != ptset.end())
            {
                if (qq++ == stopNum) break;

                POINT pt = ptset.begin()->ST;
                int segno1 = ptset.begin()->ND.ST;
                int segno2 = ptset.begin()->ND.ND;
                ptset.erase(ptset.begin());

                cout << endl << "[" << segno1 << "," << segno2 << "] : " << pt << endl;

                if (segno2 == -1 and !started[segno1])
                {
                    cout << " IF1 ";
                    mset.SetCoord(pt.GetX());
                    started[segno1] = true;

                    MutableSegmentSet::Iterator it = mset.Insert(_segs[segno1]).ST;
                    MutableSegmentSet::Iterator lprim = it, lbis = it;

                    if (it != mset.begin())
                    {
                        --lprim;
                        UpdateCrossings(lprim, it, ptset, mset.GetCoord());
                    }
                    ++lbis;
                    if (lbis != mset.end()) UpdateCrossings(lbis, it, ptset, mset.GetCoord());
                    //*/
                }
                else if (segno2 == -1 and started[segno1])
                {
                    cout << " IF2 " << _segs[segno1] << " to del  ";
                    mset.SetCoord(pt.GetX());

                    cout << " {" << mset.GetCoord() << "} : ";
                    FOREACH(it, mset)  cout << *it << " ";  cout << endl;

                    MutableSegmentSet::Iterator it = mset.Find(_segs[segno1]);
                    assert(it != mset.end());
                    MutableSegmentSet::Iterator itup = it, itdown = it;

                    if (it != mset.begin())
                    {
                        --itdown;
                        ++itup;
                        if (itup != mset.end()) UpdateCrossings(itdown, itup, ptset, mset.GetCoord());
                    }
                    mset.Erase(it);
                }
                else if (segno2 != -1)
                {
                    cout << " IF3 ";
                    result.push_back( pair<POINT, pair<int, int> >(pt, pair<int, int>(segno1, segno2)) );
                    mset.SetCoord(pt.GetX(), _segs[segno1], _segs[segno2]);

                    MutableSegmentSet::Iterator it1 = mset.Find(_segs[segno1]);
                    MutableSegmentSet::Iterator it2 = mset.Find(_segs[segno2]);

                    MutableSegmentSet::Iterator it1up = it1, it1down = it1, it2up = it2, it2down = it2;
                    ++it1up;
                    if(it1down != mset.begin()) --it1down;
                    ++it2up;
                    if(it2down != mset.begin()) --it2down;

                    if (it1up == it2  and  it1down != it1)
                    {
                        UpdateCrossings(it1, it1down, ptset, mset.GetCoord());
                    }
                    else if (it1down == it2  and  it1up != mset.end())
                    {
                        UpdateCrossings(it1, it1up, ptset, mset.GetCoord());
                    }

                    if (it2up == it1  and  it2down != it2)
                    {
                        UpdateCrossings(it2, it2down, ptset, mset.GetCoord());
                    }
                    else if (it2down == it1  and  it2up != mset.end())
                    {
                        UpdateCrossings(it2, it2up, ptset, mset.GetCoord());
                    }
                }
                cout << " {" << mset.GetCoord() << "} : ";
                FOREACH(it, mset)  cout << *it << " ";  cout << endl;
                cout << " PTS ";
                FOREACH(iit, ptset)  cout << " " << iit->ST;  cout << endl;
            }

            /*
            mset.SetCoord(_segs[0].GetP1().GetX());
            mset.SetCoord(2.9);
            mset.Insert(_segs[0]);
            mset.Insert(_segs[1]);
            //mset.Insert(_segs[2]);
            mset.Insert(_segs[3]);
            FOREACH(it, mset)  cout << *it << "  |";  cout << endl;

            mset.SetCoord(3.1, _segs[1], _segs[3]);
            mset.Insert(_segs[2]);
            FOREACH(it, mset)  cout << *it << "  |";  cout << endl;
            //*/
            cout << endl << qq << endl;
        }

        void WriteResult()
        {
            cout << "[ DONE ]" << endl;
            REP(i, SIZE(result))
                cout << " -) " << result[i].ST << "   :  " << result[i].ND.ST << "  x  " << result[i].ND.ND << endl;
        }
};


int main()
{
#ifndef ONLINE_JUDGE
    freopen("tests/in11343.txt", "r", stdin);
#endif
    int C, n;
    scanf("%i", &C);
    while(C--)
    {
        scanf("%i", &n);
        SegmentsIntersetions si;
        for (int i = 0; i < n; i++) {
            double x1, y1, x2, y2;
            scanf("%lf%lf%lf%lf", &x1, &y1, &x2, &y2);
            si.PushBack(x1, y1, x2, y2);
        }
        REP(i,n) cout << " " << si[i] << endl;
        si.Execute();
        si.WriteResult();
    }
    return 0;
}
