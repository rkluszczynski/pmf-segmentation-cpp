

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

    fin >> size_x >> size_y >> amount;
    long angleType;
    //fin >> angleType;

    for (int i = 0; i < amount; i++) {
        double xxx, yyy, aaa;
        fin >> xxx >> yyy >> aaa;
        edge_element<T_REAL> * nee = new edge_element<T_REAL>(xxx, yyy, aaa);
        PointsVector.push_back(nee);
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
        out.width(4);
        out << i << "  :  " << PointsVector[i] << endl;
    }
}
