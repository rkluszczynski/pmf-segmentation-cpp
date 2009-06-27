#ifndef GENERATE_HPP_INCLUDED
#define GENERATE_HPP_INCLUDED

#include "../cmd/point.hpp"
#include "../cmd/geometry.hpp"
#include "../cmd/probability.hpp"
#include "../cmd/schedule.hpp"


template <class REAL>
inline
long
PMF<REAL> :: GenerateInitialBirths (EventsSchedule & evts)
{
    using Probability::Exp;
    using Probability::Uniform;

    REAL halfPI = M_PI_2;
    REAL fieldHeight = GetHeight();
    REAL fieldWidth  = GetWidth();
    long id = 0;

    // Generating left birth sites ...
    PMFLogV("[ INFO ] : Generating boundary left birth sites ... ");
    REAL vertical = Exp<REAL>(2.0);
    while (vertical < fieldHeight) {
        //REAL angle = Uniform<REAL> ( -polowaPI, polowaPI);
        Point<REAL> * pt = new Point<REAL>(0.0, vertical, 0.0, 0.0, ++id, PT_BirthOnBorder);
        evts.InsertBirthEvent(pt);
        vertical += Exp<REAL>(2.0);
    }

    // Generating upper birth sites ...
    PMFLogV("[ INFO ] : Generating upper birth sites ...  ");
    REAL horizontal = Exp<REAL>(2.0);
    while (horizontal <= fieldWidth) {
        REAL angle = Uniform<REAL> ( -halfPI, halfPI);
        if (angle < 0.0) {
            Point<REAL> * pt = new Point<REAL>(horizontal, 0.0, 0.0, 0.0, ++id, PT_BirthOnBorder);
            evts.InsertBirthEvent(pt);
        }
        horizontal += Exp<REAL>(2.0);
    }
    // Generating lower birth sites ...
    PMFLogV("[ INFO ] : Generating lower birth sites ...  ");
    horizontal = Exp<REAL>(2.0);
    while (horizontal <= fieldWidth) {
        REAL angle = Uniform<REAL> ( -halfPI, halfPI);
        if (angle > 0.0) {
            Point<REAL> * pt = new Point<REAL>(horizontal, fieldWidth, 0.0, 0.0, ++id, PT_BirthOnBorder);
            evts.InsertBirthEvent(pt);
        }
        horizontal += Exp<REAL>(2.0);
    }
    return id;
}


template <class REAL>
void
PMF<REAL> :: GenerateField ()
{
    EventsSchedule evts;

    GenerateInitialBirths(evts);
    cout << evts << endl;

    return;
}


#endif // GENERATE_HPP_INCLUDED
