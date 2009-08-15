#ifndef STATISTICS_H
#define STATISTICS_H

#include "../headers/macros.hpp"

namespace pmf
{
    class Statistics
    {
        public:
            Statistics ();
            //Statistics (const Statistics & stats);
            virtual ~Statistics();

            unsigned int GetNumberOfBirths();
            unsigned int GetNumberOfUpdates();
            unsigned int GetNumberOfDeaths();

            void IncrementBirth();
            void IncrementUpdate();
            void IncrementDeath();

            friend std::ostream & operator << (std::ostream & out, pmf::Statistics & stats);

        protected:

        private:
            unsigned int numberOfBirths;
            unsigned int numberOfUpdates;
            unsigned int numberOfDeaths;
    };
}


#endif // STATISTICS_H
