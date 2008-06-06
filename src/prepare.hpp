#ifndef PREPARE_HPP_INCLUDED
#define PREPARE_HPP_INCLUDED


template <class T_REAL>
inline
void
PMF<T_REAL> :: PrepareEvolution (
                            BirthsHeap<T_REAL> * bHeap,
                            T_REAL alpha,
                            T_REAL sinL,
                            T_REAL cosL
                        )
{
    //pmfConf->set_points_ids();

#if pmf_LOG_ADD
    out << "[ alfa ] : " << alpha << "  ~  " << radians2degree(alpha) << std::endl;
    out << "[  sin ] : " << sinL << std::endl;
    out << "[  cos ] : " << cosL << std::endl;
#endif

    //if (alpha != 0.0) {
        RotatePointTypes(sinL, cosL);
#if pmf_LOG_ADD
        out << "[ SAVE ] : saving rotated configuration to a file" << std::endl;
        SaveConfiguration("output/PMF-R.txt");
#endif
    //}
#if CHECK_ASSERTIONS
    //assert(sinL*sinL + cosL*cosL == 1.0);
    assert( abs(sinL*sinL + cosL*cosL - 1.0) < EPSILON );
#endif

    pmf_point<T_REAL> * pt;
    while (! pmfConf->empty())
    {
        pt = pmfConf->front();
        pmfConf->pop_front();
        bHeap->insert(pt);
    }
}


#endif // PREPARE_HPP_INCLUDED