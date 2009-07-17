#ifndef SIMULATION_HPP_INCLUDED
#define SIMULATION_HPP_INCLUDED

#include "geometry.hpp"

namespace pmf
{

    class SimulatedAnnealingSimulation
    {
        public:
            typedef double REAL;

            SimulatedAnnealingSimulation() {}

            void Prepare () {}
            void     Run ();
            void  Finish () {}


        protected:
            virtual void           PreIteration() {}
            virtual void          PostIteration() {}

            virtual bool  CheckRunningCondition() const = 0;
            virtual REAL   CalculateHamiltonian() const = 0;
            virtual void       MakeModification() const = 0;
            virtual void      ApplyModification() const = 0;
            virtual void     CancelModification() const = 0;

    };


    void
    SimulatedAnnealingSimulation :: Run ()
    {
        Prepare ();
        REAL engH = CalculateHamiltonian();

        while (CheckRunningCondition())
        {
            PreIteration ();
            MakeModification ();

            REAL newH = CalculateHamiltonian();

            REAL deltaH = newH - engH;
            if (deltaH > 0.0)
            {
                REAL  limit = exp(- (newH - engH));
                REAL chance = Probability::Uniform(0.0, 1.0);
                if (chance < limit)
                {
                    ApplyModification ();
                }
                else {
                    CancelModification ();
                }
            }
            else  ApplyModification ();

            PostIteration ();
        }
        Finish ();
    }

}

#endif // SIMULATION_HPP_INCLUDED
