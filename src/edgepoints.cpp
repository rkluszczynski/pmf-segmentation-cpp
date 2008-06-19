

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
    /*
    std::cerr << std::endl <<"[ LOAD ] : loading configuration from a file '" << filename << "'" << std::endl;
    ifstream fin(filename);
    pmfConf->set_points_ids();
    pmfConf->load_configuration(fin);
    fin.close();
    DetermineTypesFromLeftToRight();
    fieldHeight = pmfConf->get_field_height();
    fieldWidth  = pmfConf->get_field_width();
    */
    return true;
}

