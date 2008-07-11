

template <class T_REAL>
EdgePoints<T_REAL> :: EdgePoints ()
{
    ;
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
    std::cerr << std::endl <<"[ LOAD ] : loading points' edges from a file '" << filename << "'" << std::endl;
    ifstream fin(filename);

    fin >> size_x;
    fin >> size_y;
    fin >> PointsNumber;

    fin >> angleType;

    for (int i = 0; i < PointsNumber; i++) {
        double xx, yy;
        fin >> xx;
        fin >> yy;
        pmf_point * pp = new pmf_point(xx, yy, 0.0, 0.0, i);

        PointsVector.push_back(pp);
    }
    fin.close();

    return true;
}

