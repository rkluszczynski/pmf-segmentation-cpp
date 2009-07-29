#ifndef SIMULATION_HPP_INCLUDED
#define SIMULATION_HPP_INCLUDED

#include "geometry.hpp"
#include "probability.hpp"

namespace pmf
{

    template <typename REAL = double>
    class SimulatedAnnealingSimulation
    {
        public:
            //typedef double REAL;

            SimulatedAnnealingSimulation() {}
            virtual ~SimulatedAnnealingSimulation() {}

            virtual void Prepare () {}
            virtual void     Run ();
            virtual void  Finish () {}


        protected:
            virtual void           PreIteration() {}
            virtual void          PostIteration() {}

            virtual bool  CheckRunningCondition() = 0;
            virtual bool    CheckApplyCondition() { return true; }
            virtual REAL   CalculateHamiltonian() = 0;
            virtual void       MakeModification() = 0;
            virtual void      ApplyModification() = 0;
            virtual void     CancelModification() = 0;

    };


    template <typename REAL>
    void
    SimulatedAnnealingSimulation<REAL> :: Run ()
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
