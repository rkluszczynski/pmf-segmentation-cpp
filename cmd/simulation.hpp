#ifndef SIMULATION_HPP_INCLUDED
#define SIMULATION_HPP_INCLUDED

#include "geometry.hpp"
#include "probability.hpp"

namespace pmf
{

    class SimulatedAnnealingSimulation
    {
        public:
            typedef double REAL;

            SimulatedAnnealingSimulation() {}
            virtual ~SimulatedAnnealingSimulation() {}

            void Prepare () {}
            void     Run ();
            void  Finish () {}


        protected:
            virtual void           PreIteration() {}
            virtual void          PostIteration() {}

            virtual bool  CheckRunningCondition() const = 0;
            virtual bool    CheckApplyCondition() { return true; }
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
        REAL newH;
        while (CheckRunningCondition())
        {
            PreIteration ();

            MakeModification ();
            bool apply = CheckApplyCondition();
            if (apply)
            {
                newH = CalculateHamiltonian();
                REAL deltaH = newH - engH;
                if (deltaH > 0.0)
                {
                    REAL  limit = exp(-deltaH);
                    REAL chance = Probability::Uniform(0.0, 1.0);
                    if (chance > limit)  apply = false;
                }
            }

            if (apply)
            {
                ApplyModification ();
                engH = newH;
            }
            else CancelModification ();

            PostIteration ();
        }
        Finish ();
    }

}

#endif // SIMULATION_HPP_INCLUDED
