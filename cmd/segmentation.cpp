#include "segmentation.h"
#include "probability.hpp"
#include "pmf.hpp"

#define SAVE_PMR 1

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
		{
            pmf->LoadPMF (initialFile);
            pmf->RotatePoints2 (0., 1.);
		}
		else
            pmf->GenerateField ();

        out.rdbuf(cout.rdbuf());
        pmf->SavePMF("output/_first-conf.ggb", GeoGebraFile);
        scanf("%*c");
        cout << "[ SEGM ] : ctor.end()" << endl;
#if SAVE_PMR
        FILE * fp = fopen("output/pmr.txt", "w");
        fprintf(fp, "Picture : %s\n", pictureFile);
        fprintf(fp, "   Seed : %li\n", seed);
        fclose(fp);
#endif
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

        double beta_1 = 20. + 0.009 * (loopIteration + 200000);
        double beta_2 = 0.0;
        double result = 0.0;

        double tmpArea, tmpElen;
        tmpArea = storedArea = pmf->CalculateEnergy(img);
        tmpElen = storedElen = 0.0;

        result = beta_1 * storedArea + beta_2 * storedElen;
        fprintf(stderr, "[ENERGY] : %lf  (%.2lf)\n", result, tmpArea);
#if SAVE_PMR
        if (!loopIteration)
        {
            FILE * fp = fopen("output/pmr.txt", "a");
            fprintf(fp, "%li;%.21lf\n", loopIteration, storedArea);
            fclose(fp);
        }
#endif
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


/// TODO (Rafal#9#): check if old birth and old death are on vertical line



        // * Determinig limits for random move. *
        double   noOfBirths = stats.GetNumberOfBirths();
        double    noOfTurns = stats.GetNumberOfUpdates();
        double denominatorZ = 1.0 / (areaOfPMF + noOfBirths + noOfTurns);

        double limit1 = areaOfPMF * denominatorZ;
        double limit2 = (areaOfPMF + noOfBirths) * denominatorZ;

        ofstream fout2("output/_iteration-modification.txt");
        out.rdbuf(fout2.rdbuf());
        fout3.close();
            pmf->GetCf()->SaveConfigurationAsGGB("output/rotated-before.ggb");

        // * Applying random operation. *
        double chance = Uniform(0.0, 1.0);
        if (chance < limit1)
        {
            double x, y;
            while (true)
            {
                x = Uniform(0.0 + 2. * EPSILON, pmf->GetWidth() - 2. * EPSILON);
                y = Uniform(0.0 + 2. * EPSILON, pmf->GetHeight() - 2. * EPSILON);

                if (pmf->AddBirthPoint (x, y, sinL, cosL)) break;

                out << "BANG" << endl;
            }
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

        pmf->GetCf()->SaveConfigurationAsGGB("output/rotated-after.ggb");
        cout << "CHECK 1" << endl;
        pmf->RotatePoints2 (0., 1.);
        cout << "CHECK 2" << endl;

        cout << "[ SEGM ] : modification.end()" << endl;
    }

    void
    BinarySegmentation::ApplyModification()
    {
        cout << "[ SEGM ] : applying modification" << endl;
#if SAVE_PMR
        FILE * fp = fopen("output/pmr.txt", "a");
        fprintf(fp, "%li;%.21lf\n", loopIteration, storedArea);
        fclose(fp);
#endif
    }

    void
    BinarySegmentation::CancelModification()
    {
        cout << "[ SEGM ] : canceling modification" << endl;

        std::swap(pmf, clone);
#if SAVE_PMR
        FILE * fp = fopen("output/pmr.txt", "a");
        fprintf(fp, "%li;0\n", loopIteration);
        fclose(fp);
#endif
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
        //int iterNum = 27270;
        int iterNum = 22900;

        //if (loopIteration >= 17744) Geometry::qq = true;
        //if (loopIteration >= iterNum) pmf->EraseSmallPolygons(0.0001);

        if (loopIteration < iterNum)
            sprintf(filename, "output/pre.ggb");
        else
            sprintf(filename, "output/pre%li.ggb", loopIteration);
        pmf->SavePMF(filename);

        if (loopIteration < iterNum)
            sprintf(filename, "output/pre.ggb");
        else
            sprintf(filename, "output/pre%li.ggb", loopIteration);
        pmf->SavePMF(filename, GeoGebraFile);

        clone = pmf->Clone();
        apply = true;

        cout << "[ SEGM ] :  pre-iteration.end()" << endl;
    }

    void
    BinarySegmentation::PostIteration()
    {
        cout << "[ SEGM ] :  post-iteration.begin()" << endl;

        //pmf->SavePMF("output/post-before-check-0.ggb", GeoGebraFile);
        cout << " post check 0" << endl;
        assert( pmf->TestPointsCoincidence() );

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
            pmf->SavePMF ("output/sim-result.ggb", GeoGebraFile);

            //pmf->EraseSmallPolygons();
        }
        delete pmf;

        cout << "[ SEGM ] : finish.end()" << endl;
    }
}
