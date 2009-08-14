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
        REAL __engH = CalculateHamiltonian();
        REAL __newH;
        while (CheckRunningCondition())
        {
            PreIteration ();

            MakeModification ();
            bool __apply = CheckApplyCondition();
            if (__apply)
            {
                __newH = CalculateHamiltonian();
                REAL __deltaH = __newH - __engH;
                if (__deltaH > 0.0)
                {
                    REAL  __limit = exp(-__deltaH);
                    REAL __chance = Probability::Uniform(0.0, 1.0);
                    if (__chance > __limit)  __apply = false;
                }
            }

            if (__apply)
            {
                ApplyModification ();
                __engH = __newH;
            }
            else CancelModification ();

            PostIteration ();
        }
        Finish ();
    }

}

#endif // SIMULATION_HPP_INCLUDED
