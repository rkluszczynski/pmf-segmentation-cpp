#ifndef STATISTICS_HPP_INCLUDED
#define STATISTICS_HPP_INCLUDED

#include "configuration.hpp"


namespace pmf
{

    template <class REAL>
    class Statistics
    {
        public:
            Statistics (Configuration<REAL> * cf);
            virtual ~Statistics();

            int GetNumberOfBirths()  { return numberOfBirths; }
            int GetNumberOfUpdates() { return numberOfUpdates; }
            int GetNumberOfDeaths()  { return numberOfDeaths; }

        protected:

        private:
            int numberOfBirths;
            int numberOfUpdates;
            int numberOfDeaths;
    };


    template <class REAL>
    Statistics<REAL> :: Statistics (Configuration<REAL> * cf) : numberOfBirths(0), numberOfUpdates(0), numberOfDeaths(0)
    {
        FOREACH(it, *cf)
        {
            Point<REAL> * pt = *it;
            switch (pt->type)
            {
                case PT_BirthOnBorder :
                case PT_BirthInField  :
                                    ++numberOfBirths;
                                    break;;
                case PT_Update        :
                                    ++numberOfUpdates;
                                    break;;
                case PT_Collision     :
                case PT_DeathOnBorder :
                                    ++numberOfDeaths;
                                    break;;
                default :
                        assert("WRONG POINT TYPE DURING GETTING STATISTICS" && false);
            }
        }
        assert( numberOfBirths + numberOfDeaths + numberOfUpdates == cf->GetPointsCount());
    }


    typedef Statistics<double> DoubleStatistics;
}


#endif // STATISTICS_HPP_INCLUDED
