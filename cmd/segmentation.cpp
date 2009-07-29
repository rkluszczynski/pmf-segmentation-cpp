#include "segmentation.h"

namespace pmf
{

    BinarySegmentation::BinarySegmentation()
    {
        //ctor
    }

    BinarySegmentation::~BinarySegmentation()
    {
        //dtor
    }


    bool
    BinarySegmentation::CheckRunningCondition()
    {
        return true;
    }

    bool
    BinarySegmentation::CheckApplyCondition()
    {
        return SimulatedAnnealingSimulation<double>::CheckApplyCondition();
    }

    double
    BinarySegmentation::CalculateHamiltonian()
    {
        return 0.0;
    }

    void
    BinarySegmentation::MakeModification()
    {
    }

    void
    BinarySegmentation::ApplyModification()
    {
    }

    void
    BinarySegmentation::CancelModification()
    {
    }
}
