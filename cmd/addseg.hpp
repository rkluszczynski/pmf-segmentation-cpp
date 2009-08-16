#ifndef ADDSEG_HPP_INCLUDED
#define ADDSEG_HPP_INCLUDED


template <class REAL>
void
PMF<REAL> :: AddBirthSegment (REAL xx, REAL yy, REAL sinL, REAL cosL)
{

    return;
}


template <class REAL>
void
PMF<REAL> :: AddBirthSegment (REAL xx, REAL yy, REAL alpha = 0.0)
{
    REAL  sinL = sin(alpha);
    REAL  cosL = cos(alpha);
    RotatePoints2 (sinL, cosL);
    AddBirthSegment (xx, yy, sinL, cosL);
    RotatePoints2 (0., 1.);
    return;
}


#endif // ADDSEG_HPP_INCLUDED
