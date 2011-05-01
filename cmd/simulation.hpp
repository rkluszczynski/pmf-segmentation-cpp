#ifndef SIMULATION_HPP_INCLUDED
#define SIMULATION_HPP_INCLUDED

#include "geometry.hpp"
//#include "probability.hpp"
#include "DoublePRNG.h"

namespace pmf
{

    template <typename REAL = double>
    class SimulatedAnnealingSimulation
    {
        public:
            //typedef double REAL;

            SimulatedAnnealingSimulation() {}
            virtual ~SimulatedAnnealingSimulation() {}

            virtual void     PrepareSimulation ();
            virtual void      FinishSimulation ();

            virtual
             inline void      RunTheSimulation ();
            virtual bool CheckRunningCondition () = 0;
            virtual void           RunNextStep ();


        protected:
            virtual void               Prepare () {}
            virtual void                Finish () {}

            virtual void           PreIteration() {}
            virtual void          PostIteration() {}

            virtual bool    CheckApplyCondition() { return true; }
            virtual REAL   CalculateHamiltonian() = 0;
            virtual void       MakeModification() = 0;
            virtual void      ApplyModification() = 0;
            virtual void     CancelModification() = 0;

            virtual double         GetUniform01() = 0;

        private:
            REAL __engH, __newH;
    };


    template <typename REAL>
    void
    SimulatedAnnealingSimulation<REAL> :: PrepareSimulation ()
    {
        Prepare ();
        __engH = CalculateHamiltonian();
    }


    template <typename REAL>
    void
    SimulatedAnnealingSimulation<REAL> :: FinishSimulation ()
    {
        Finish ();
    }


    template <typename REAL>
    void
    SimulatedAnnealingSimulation<REAL> :: RunNextStep ()
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
                //REAL __chance = Probability::Uniform(0.0, 1.0);
                //REAL __chance = pmf::PRNG->GetUniform();
                REAL __chance = GetUniform01();
                if (__chance > __limit)
                    __apply = false;
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


    template <typename REAL>
    inline
    void
    SimulatedAnnealingSimulation<REAL> :: RunTheSimulation ()
    {
        PrepareSimulation();
        while (CheckRunningCondition())  RunNextStep();
        FinishSimulation();
    }

}

#endif // SIMULATION_HPP_INCLUDED
