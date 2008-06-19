

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

    fin.close();

    return true;
}

