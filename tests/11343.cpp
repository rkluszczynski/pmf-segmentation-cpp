#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <set>
#include <cstdio>

using namespace std;

#define SIZE(X)     ((int)(X).size())
#define FOR(X,B,E)  for(int X = B; X <= (E); ++X)
#define REP(X,N)    for(int X = 0; X < (N); ++X)

#define ST first
#define ND second
#define PB push_back

#define TYPE double


template <class Real = double>
struct Point
{
    private :
        Real x, y;

    public :
        Point() : x(-1), y(-1) { }
        Point(Real xx, Real yy) : x(xx), y(yy) { }

        const Real GetX() const { return x; }
        const Real GetY() const { return y; }

        //bool operator ==(const Point<Real> & pt) const { return pt.x == x && pt.y == y; }
        bool operator < (const Point<Real> & pt) const { return x < pt.x || (pt.x == x  &&  y < pt.y); }

        friend ostream & operator << (ostream & out, const Point<Real> pt)
        {
            out << "( " << pt.GetX() << " , " << pt.GetY() << " )";
            return out;
        }
};


template <class Real = double>
struct Segment
{
    private :
        Point<Real> p1, p2;

    public :
        Segment() { }
        Segment(Point<Real> pp1, Point<Real> pp2) : p1(pp1), p2(pp2) { }

        const Point<Real> GetP1() const { return p1; }
        const Point<Real> GetP2() const { return p2; }

        friend ostream & operator << (ostream & out, const Segment<Real> seg)
        {
            out << " " << seg.GetP1() << "-" << seg.GetP2() << " ";
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

        void SetCoord(double x)
        {
            m_scan_pos = x;
            //ReOrder();
        }
        double GetCoord() const { return m_scan_pos; }

        // methods: begin, end

        // methods: insert, erase, find
        void Insert(ENTRY & ent) { m_set.insert(ent); }
        void Erase(Iterator it) { m_set.erase(it); }
        Iterator Find(ENTRY & ent) { return m_set.find(ent); }

    private :
        std::set<ENTRY, fCompare> m_set;
        double m_scan_pos;


        class MutableIter : public SegSet::iterator
        {
            public:
                typedef SegSet::iterator BaseIter;

                // constructors

                ENTRY & operator*() { return const_cast<ENTRY&>(this->BaseIter::operator*()); }
                /*
                MutableIter& operator++();
                const MutableIter operator++(int);
                MutableIter& operator--();
                const MutableIter operator--(int);
                bool operator==( const MutableIter& rhs);
                bool operator!=( const MutableIter& rhs);
                Segment* operator->();
                //*/
        };
        /*
        void ReOrder();
        void Reverse(Iterator first, Iterator last);
        Iterator GetIntersectingSegs(Iterator first, Iterator last);
        //*/
};

bool fCompare::operator() (const ENTRY & lhs, const ENTRY & rhs) const
{
    double x_scan_pos = m_set.GetCoord();

    double tgL1 = (lhs.GetP2().GetX() - lhs.GetP1().GetX()) / (lhs.GetP2().GetY() - lhs.GetP1().GetY());
    double b1 = lhs.GetP2().GetY() - tgL1 * lhs.GetP2().GetX();

    double ly_scan_pos = tgL1 * x_scan_pos + b1;

    double tgL2 = (rhs.GetP2().GetX() - rhs.GetP1().GetX()) / (rhs.GetP2().GetY() - rhs.GetP1().GetY());
    double b2 = rhs.GetP2().GetY() - tgL2 * rhs.GetP2().GetX();

    double ry_scan_pos = tgL2 * x_scan_pos + b2;

    return ly_scan_pos < ry_scan_pos;
};


class SegmentsIntersetions
{
    typedef Point<TYPE> POINT;
    typedef Segment<TYPE> SEGMENT;

    private :
        vector<SEGMENT> _segs;

    public :
        SegmentsIntersetions(int n = 0) : _segs(n) { }
        ~SegmentsIntersetions() { }

        SEGMENT & operator[] (const int index) { return _segs[index]; }

        void PushBack(TYPE x1, TYPE y1, TYPE x2, TYPE y2)
        {
            POINT p1(x1, y1), p2(x2, y2);
            SEGMENT seg(p1, p2);
            _segs.push_back(seg);
        }

        void Execute()
        {
            set<pair<POINT,pair<int, int> > > ptset;
            vector<bool> started;

            REP(i, SIZE(_segs))
            {
                ptset.insert( pair<POINT, pair<int, int> >(_segs[i].GetP1(), pair<int, int>(i, -1)) );
                ptset.insert( pair<POINT, pair<int, int> >(_segs[i].GetP2(), pair<int, int>(i, -1)) );
            }
            fill(started.begin(), started.end(), false);

            while (ptset.begin() != ptset.end())
            {
                POINT pt = ptset.begin()->ST;
                int segno1 = ptset.begin()->ND.ST;
                int segno2 = ptset.begin()->ND.ND;
                cout << "[" << segno1 << "] : " << pt << endl;

                ptset.erase(ptset.begin());
            }
        }

        void WriteResult()
        {
            cout << "[ DONE ]" << endl;
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
        REP(i,n) cout << si[i] << endl;
        si.Execute();
        si.WriteResult();
    }
    return 0;
}
