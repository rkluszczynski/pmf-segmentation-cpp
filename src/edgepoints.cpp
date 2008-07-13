

template <class T_REAL>
EdgePoints<T_REAL> :: EdgePoints ()
{
    PointsVector.reserve(200);
}


template <class T_REAL>
EdgePoints<T_REAL> :: ~EdgePoints ()
{
    ;
}


template <class T_REAL>
bool
EdgePoints<T_REAL> :: LoadData (const char * filename)
{
    std::cerr << std::endl <<"[ EDGE ] : loading points' edges from a file '" << filename << "'" << std::endl;
    ifstream fin(filename);
    long amount = 0;

    fin >> size_x;
    fin >> size_y;
    fin >> amount;

    long angleType;
    fin >> angleType;

    for (int i = 0; i < amount; i++) {
        double xxx, yyy, aaa;
        fin >> xxx;
        fin >> yyy;
        fin >> aaa;

        xx.push_back(xxx);
        yy.push_back(yyy);
        aa.push_back(aaa);
/*
        pmf_point<T_REAL> * pp = new pmf_point<T_REAL>(xxx, yyy, 0.0, 0.0, i);
*/
        PointsVector.push_back(NULL);
    }
    fin.close();
    return true;
}


template <class T_REAL>
void
EdgePoints<T_REAL> :: PrintData (std::ostream & out = std::cout)
{
    out << "[ EDGE ] : printing data" << endl;
    out << "[ EDGE ] : amount = " << getPointsNumber() << " ;\t";
    out.precision(3);
    out.width(5);
    out << "  size = " << size_x << " x " << size_y  << endl;

    for (int i = 0; i < getPointsNumber(); i++) {
        //*
        out.width(4);
        out << i;
        out << "  :  ";
        out.width(7);
        out.precision(3);   out << xx[i];
        out << "  ";
        out.width(7);
        out.precision(3);   out << yy[i];
        out << "  ";
        out.width(7);
        out.precision(3);   out << aa[i];
        out << endl;
        //*/
        /*
        pmf_point<T_REAL> * ptmp = PointsVector[i];
        out.width(4);
        out << i << "  :  " << ptmp << endl;
        //*/
    }
}
