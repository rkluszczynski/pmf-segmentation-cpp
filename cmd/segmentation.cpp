#include "segmentation.h"
#include "probability.hpp"
#include "pmf.hpp"

namespace pmf
{

    BinarySegmentation::BinarySegmentation(double wsize, double hsize, const char * initialFile, const char * outputFile, time_t seed, const char * pictureFile, long iter, double pmr)
    : loopIteration(0), iterations(iter), rate(pmr), outputfile(outputFile)
    {
        //freopen ("output/stdout.txt", "w", stdout);
        //freopen ("output/stderr.txt", "w", stderr);

        cout << "[ SEGM ] : ctor.begin()" << endl;

        ofstream fout1("output/gen.txt");
        out.rdbuf(fout1.rdbuf());

        img = new GrayscaleImage(pictureFile);

        pmf = new DoublePMF (wsize, hsize);
        pmf->SetSeed (seed);
		if (initialFile)
            pmf->LoadPMF (initialFile);
		else
            pmf->GenerateField ();

        out.rdbuf(cout.rdbuf());

        cout << "[ SEGM ] : ctor.end()" << endl;
    }


    BinarySegmentation::~BinarySegmentation()
    {
        cout << "[ SEGM ] : dtor.begin()" << endl;
        delete img;
        cout << "[ SEGM ] : dtor.end()" << endl;
    }


    bool
    BinarySegmentation::CheckRunningCondition()
    {
        cout << "[ SEGM ] : checking running condition" << endl;

        if (iterations > 0  &&  loopIteration >= iterations) return false;
        if (rate > 0.0  &&  rate > storedArea) return false;
        return true;
    }

    bool
    BinarySegmentation::CheckApplyCondition()
    {
        cout << "[ SEGM ] : checking apply condition ... ";

        //return SimulatedAnnealingSimulation<double>::CheckApplyCondition();
        cout << (apply ? "TRUE" : "FALSE") << endl;
        return apply;
    }

    double
    BinarySegmentation::CalculateHamiltonian()
    {
        cout << "[ SEGM ] : calculating hamiltonian" << endl;

        double beta_1 = 20. + 0.009 * loopIteration;
        double beta_2 = 0.0;
        double result = 0.0;

        double tmpArea, tmpElen;
        tmpArea = storedArea = pmf->CalculateEnergy(img);
        tmpElen = storedElen = 0.0;

        result = beta_1 * storedArea + beta_2 * storedElen;
        fprintf(stderr, "[ENERGY] : %lf  (%.2lf)\n", result, tmpArea);

        return result;
    }

    inline
    void
    BinarySegmentation::MakeModification()
    {
        using Probability::Uniform;
        cout << "[ SEGM ] : modification.begin()" << endl;

        ofstream fout3("output/rot.txt");
        out.rdbuf(fout3.rdbuf());

        double angle = Uniform<double>(0.0, 2. * M_PI);
        cout << "         : rotating at angle " << angle << "  (" << Geometry::RadiansToDegree(angle) << ")" << endl;
        out << "         : rotating at angle " << angle << "  (" << Geometry::RadiansToDegree(angle) << ")" << endl;
        double sinL = sin(angle);
        double cosL = cos(angle);
        //pmf->RotatePointTypes(sinL, cosL);

        pmf->RotatePoints2(sinL, cosL);
        pmf::Statistics stats = pmf->GetStatistics();

        // * Determinig limits for random move. *
        double   noOfBirths = stats.GetNumberOfBirths();
        double    noOfTurns = stats.GetNumberOfUpdates();
        double denominatorZ = 1.0 / (areaOfPMF + noOfBirths + noOfTurns);

        double limit1 = areaOfPMF * denominatorZ;
        double limit2 = (areaOfPMF + noOfBirths) * denominatorZ;

        ofstream fout2("output/_iteration-modification.txt");
        out.rdbuf(fout2.rdbuf());
        fout3.close();
            pmf->GetCf()->SaveConfigurationAsGGB("output/rotated.zip");

        // * Applying random operation. *
        double chance = Uniform(0.0, 1.0);
        if (chance < limit1)
        {
            double x = Uniform(0.0 + 2. * EPSILON, pmf->GetWidth() - 2. * EPSILON);
            double y = Uniform(0.0 + 2. * EPSILON, pmf->GetHeight() - 2. * EPSILON);
            pmf->AddBirthPoint (x, y, sinL, cosL);
        }
        else if(chance < limit2)
        {
            int number = rand() % int(noOfBirths);
            pmf->RemoveBirthPoint (number, sinL, cosL);
        }
        else
        {
            int number = rand() % int(noOfTurns);
            pmf->UpdatePointVelocity (number, sinL, cosL);
        }
        cout << "CHECK 1" << endl;
        pmf->RotatePoints2 (0., 1.);
        cout << "CHECK 2" << endl;

        cout << "[ SEGM ] : modification.end()" << endl;
    }

    void
    BinarySegmentation::ApplyModification()
    {
        cout << "[ SEGM ] : applying modification" << endl;
    }

    void
    BinarySegmentation::CancelModification()
    {
        cout << "[ SEGM ] : canceling modification" << endl;

        std::swap(pmf, clone);
    }

    void
    BinarySegmentation::PreIteration()
    {
        cout << "_________________________________" << endl;
        cout << "[ ITER ] : " << loopIteration << endl;
        cout << "[ SEGM ] :  pre-iteration.begin()" << endl;

        ofstream fout2("output/last-iteration-save.txt");
        out.rdbuf(fout2.rdbuf());

        char filename[256];
        int iterNum = 7870999;

        if (loopIteration < iterNum)
            sprintf(filename, "output/pre.txt");
        else
            sprintf(filename, "output/pre%li.txt", loopIteration);
        pmf->SavePMF(filename);

        if (loopIteration < iterNum)
            sprintf(filename, "output/pre.zip");
        else
            sprintf(filename, "output/pre%li.zip", loopIteration);
        pmf->SavePMF(filename, GeoGebraFile);

        clone = pmf->Clone();
        apply = true;

        cout << "[ SEGM ] :  pre-iteration.end()" << endl;
    }

    void
    BinarySegmentation::PostIteration()
    {
        cout << "[ SEGM ] :  post-iteration.begin()" << endl;
        cout << " post check 1" << endl;
        delete clone;
        cout << " post check 2" << endl;
        ++loopIteration;

        cout << "[ SEGM ] :  post-iteration.end()" << endl;
    }


    void
    BinarySegmentation::Prepare()
    {
        cout << "[ SEGM ] : prepare.begin()" << endl;

        areaOfPMF = M_PI * pmf->GetHeight() * pmf->GetWidth();

        cout << "[ SEGM ] : prepare.end()" << endl;
    }

    void
    BinarySegmentation::Finish()
    {
        cout << "[ SEGM ] : finish.begin()" << endl;

        if (outputfile)
        {
            pmf->SavePMF (outputfile);
            pmf->SavePMF ("output/sim-result.zip", GeoGebraFile);
        }
        delete pmf;

        cout << "[ SEGM ] : finish.end()" << endl;
    }
}
