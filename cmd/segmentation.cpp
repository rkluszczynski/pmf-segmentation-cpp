#include "segmentation.h"
#include "probability.hpp"
#include "pmf.hpp"

#define SAVE_PMR 1

namespace pmf
{

    BinarySegmentation::BinarySegmentation (SegmentationParameters params, unsigned thId)
    : parameters(params), loopIteration(0L), threadId(thId)
    {
        cout << "[ SEGM ] : ctor.begin()" << endl;
        std::string fout1name( std::string(parameters.GetOutputDirectory()) + std::string(parameters.GetOutputPrefix()) + std::string("gen.txt") );
        cout << fout1name << endl;
        ofstream fout1( fout1name.c_str() );
        //out.rdbuf(fout1.rdbuf());

        numerics = new NumericalParameters (parameters.GetPMFEpsilon());
        assert(parameters.GetPMFEpsilon() > 0.);
        prng = new DoublePRNG (parameters.GetSeed());
        img = new GrayscaleImage (parameters.GetPictureFile());
        pmf = new DoublePMF (parameters.GetFieldWidth(), parameters.GetFieldHeight(), *numerics);

        pmf->SetSeed (parameters.GetSeed());
        pmf->SetPRNG (prng);

        pmf->SetOutStream( fout1 );
        cout << "[ SEGM ] : ctor.checkpoint1()" << endl;
		if (parameters.GetInitialFile())
		{
            pmf->LoadPMF (parameters.GetInitialFile());
            pmf->RotatePoints2 (0., 1.);
		}
		else
            pmf->GenerateField ();
        cout << "[ SEGM ] : ctor.checkpoint2()" << endl;

        iterations = parameters.GetIterationsNumber();
        rate = parameters.GetPMRRate();
        outputfile = parameters.GetOutputFile();

        //out.rdbuf(cout.rdbuf());
        pmf->SetOutStream( cout );
        cout << "[ SEGM ] : ctor.end()" << endl;
    }

    BinarySegmentation::BinarySegmentation(double wsize, double hsize, const char * initialFile, const char * outputFile, time_t seed, const char * pictureFile, long iter, double pmr)
    : loopIteration(0), iterations(iter), rate(pmr), outputfile(outputFile)
    {
        //freopen ("output/stdout.txt", "w", stdout);
        //freopen ("output/stderr.txt", "w", stderr);

        cout << "[ SEGM ] : ctor.begin()" << endl;

        ofstream fout1("output/gen.txt");
        //out.rdbuf(fout1.rdbuf());
        pmf->SetOutStream( fout1 );

        img = new GrayscaleImage(pictureFile);

        numerics = new NumericalParameters(0.00000001);
        pmf = new DoublePMF (wsize, hsize, *numerics);
        pmf->SetSeed (seed);
		if (initialFile)
		{
            pmf->LoadPMF (initialFile);
            pmf->RotatePoints2 (0., 1.);
		}
		else
            pmf->GenerateField ();

        //out.rdbuf(cout.rdbuf());
        pmf->SetOutStream( cout );
        pmf->SavePMF("output/_first-conf.ggb", GeoGebraFile);
        scanf("%*c");
        cout << "[ SEGM ] : ctor.end()" << endl;
#if SAVE_PMR
        std::string pmrfile( std::string(parameters.GetOutputDirectory()) + std::string(parameters.GetOutputPrefix()) + std::string("pmr.txt") );
        FILE * fp = fopen(pmrfile.c_str(), "w");
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


    void
    BinarySegmentation::ReplacePMF(DoublePMF * npmf)
    {
        delete pmf;
        pmf = npmf->Clone();
    }


    bool
    BinarySegmentation::CheckRunningCondition()
    {
        cout << "[ SEGM ] : checking running condition  (" << storedArea << ")" << endl;

        if (iterations > 0  &&  loopIteration >= iterations) return false;
        if (rate > 0.0  &&  rate > storedArea) return false;
        return true;
    }

    bool
    BinarySegmentation::CheckApplyCondition()
    {
        cout << "[ SEGM ] : checking apply condition ... ";

        //return SimulatedAnnealingSimulation<double>::CheckApplyCondition();
        if (pmf->DetermineMinimalSquareDistance() < 0.000000001) apply = false;

        cout << (apply ? "TRUE" : "FALSE") << endl;
        return apply;
    }

    double
    BinarySegmentation::CalculateHamiltonian()
    {
        cout << "[ SEGM ] : calculating hamiltonian" << endl;

        //double beta_1 = 20. + 0.009 * (loopIteration + 70000);
        double beta_1 = 2000. + 0.01 * loopIteration;
        double beta_2 = 0.0;
        double beta_LJn = 1.0;
        double beta_LJg = 1.0;
        double result = 0.0;

        tmpArea = pmf->CalculateGrayscaleImageEnergyTerm(img);
        tmpElen = storedElen = 0.0;

        //tmpLJn = pmf->CalculateLennardJonesNeighboursEnergyTerm(.1, _sigma12, _sigma6, _rcutoff);
        tmpLJn = pmf->CalculateSmoothPotentialLogEnergyTerm (_rcutoff, 100.);
        tmpLJg = pmf->CalculateLennardJonesMinimalDistanceEnergyTerm(numerics->GetAxisEpsilon(), 4.*_sigma12, 2.*_sigma6, _rcutoff);

        tmpEnergy = result = beta_1 * tmpArea + beta_2 * tmpElen + beta_LJn * tmpLJn + beta_LJg * tmpLJg;
        fprintf(stderr, "[ENERGY-%u] : %lf  (img=%.7lf / Log=%.11lf / LJg=%.11lf)\n", threadId, result, tmpArea, tmpLJn, tmpLJg);
#if SAVE_PMR
        if (!loopIteration)
        {
            std::string pmrfile( std::string(parameters.GetOutputDirectory()) + std::string(parameters.GetOutputPrefix()) + std::string("pmr.txt") );
            FILE * fp = fopen(pmrfile.c_str(), "w");
            fprintf(fp, "%li;%.21lf;\t%.17lf;\t%.17lf\n", loopIteration, storedArea, tmpLJn, tmpLJg);
            fclose(fp);
        }
#endif
        return result;
    }

    inline
    void
    BinarySegmentation::MakeModification()
    {
        using Probability::_Uniform;
        cout << "[ SEGM ] " << parameters.GetOutputPrefix() << ": modification.begin()" << endl;

        ///ofstream fout3("output/rot.txt");
        std::string fout3name( std::string(parameters.GetOutputDirectory()) + std::string(parameters.GetOutputPrefix()) + std::string("rot.txt") );
        ofstream fout3( fout3name.c_str() );
        //out.rdbuf(fout3.rdbuf());
        pmf->SetOutStream( fout3 );

        double angle = prng->GetUniform(0.0, 2. * M_PI);
        cout << "         : rotating at angle " << angle << "  (" << Geometry::RadiansToDegree(angle) << ")" << endl;
        pmf->GetOutStream() << "         : rotating at angle " << angle << "  (" << Geometry::RadiansToDegree(angle) << ")" << endl;
        double sinL = sin(angle);
        double cosL = cos(angle);
        //pmf->RotatePointTypes(sinL, cosL);

        pmf->RotatePoints2(sinL, cosL, true);
        Statistics stats = pmf->GetStatistics();

/// TODO (Rafal#9#): check if old birth and old death are on vertical line


        // * Determinig limits for random move. *
        double   noOfBirths = stats.GetNumberOfBirths();
        double    noOfTurns = stats.GetNumberOfUpdates();
        double denominatorZ = 1.0 / (areaOfPMF + noOfBirths + noOfTurns);

        double limit1 = areaOfPMF * denominatorZ;
        double limit2 = (areaOfPMF + noOfBirths) * denominatorZ;

        ///ofstream fout2("output/_iteration-modification.txt");
        std::string fout2name( std::string(parameters.GetOutputDirectory()) + std::string(parameters.GetOutputPrefix()) + std::string("_iteration-modification.txt") );
        ofstream fout2( fout2name.c_str() );
        //out.rdbuf(fout2.rdbuf());
        pmf->SetOutStream( fout2 );
        fout3.close();

        pmf->GetOutStream() << endl << stats << endl;

            std::string cf1file( std::string(parameters.GetOutputDirectory()) + std::string(parameters.GetOutputPrefix()) + std::string("rotated-before.ggb") );
            pmf->GetCf()->SaveConfigurationAsGGB(cf1file.c_str());

        // * Applying random operation. *
        double chance = prng->GetUniform();
        if (chance < limit1)
        {
            double x, y;
            double eps = numerics->GetAxisEpsilon();
            while (true)
            {
                x = prng->GetUniform(0.0 + 2. * eps, pmf->GetWidth() - 2. * eps);
                y = prng->GetUniform(0.0 + 2. * eps, pmf->GetHeight() - 2. * eps);

                if (pmf->AddBirthPoint (x, y, sinL, cosL)) break;

                //out << "BANG" << endl;
                pmf->GetOutStream() << "BANG" << endl;
            }
        }
        else if(chance < limit2)
        {
            int number = rand() % int(noOfBirths);
            pmf->GetOutStream() << " random number = " << number << endl;
            pmf->RemoveBirthPoint (number, sinL, cosL);
        }
        else
        {
            int number = rand() % int(noOfTurns);
            pmf->UpdatePointVelocity (number, sinL, cosL);
        }

        std::string cf2file( std::string(parameters.GetOutputDirectory()) + std::string(parameters.GetOutputPrefix()) + std::string("rotated-after.ggb") );
        pmf->GetCf()->SaveConfigurationAsGGB( cf2file.c_str() );
        cout << "CHECK 1" << endl;
        pmf->RotatePoints2 (0., 1.);
        cout << "CHECK 2" << endl;

        pmf->SetOutStream( cout );
        cout << "[ SEGM ] : modification.end()" << endl;
    }

    void
    BinarySegmentation::ApplyModification()
    {
        cout << "[ SEGM ] _" << parameters.GetOutputPrefix() << ": applying modification" << endl;
        storedArea = tmpArea;
        storedEnergy = tmpEnergy;
#if SAVE_PMR
        std::string pmrfile( std::string(parameters.GetOutputDirectory()) + std::string(parameters.GetOutputPrefix()) + std::string("pmr.txt") );
        FILE * fp = fopen(pmrfile.c_str(), "a");
        fprintf(fp, "%li;%.21lf;\t%.17lf;\t%.17lf\n", loopIteration, storedArea, tmpLJn, tmpLJg);
        //fprintf(fp, "%li;%.21lf\n", loopIteration, storedArea);
        fclose(fp);
#endif
    }

    void
    BinarySegmentation::CancelModification()
    {
        cout << "[ SEGM ] _" << parameters.GetOutputPrefix() << ": canceling modification" << endl;

        std::swap(pmf, clone);
#if SAVE_PMR
        std::string pmrfile( std::string(parameters.GetOutputDirectory()) + std::string(parameters.GetOutputPrefix()) + std::string("pmr.txt") );
        FILE * fp = fopen(pmrfile.c_str(), "a");
        fprintf(fp, "%li;0\n", loopIteration);
        fclose(fp);
#endif
    }

    void
    BinarySegmentation::PreIteration()
    {
        cout << "_________________________________" << endl;
        cout << "[ ITER ] _" << parameters.GetOutputPrefix() << ": " << loopIteration << endl;
        pmf->SetPRNG (prng);
        cout << "[ SEGM ] :  pre-iteration.begin()" << endl;

        std::string _str;
        _str += std::string(parameters.GetOutputDirectory());
        _str += std::string(parameters.GetOutputPrefix());
        _str += std::string("last-iteration-save.txt");
        ofstream fout2( _str.c_str() );
        //out.rdbuf(fout2.rdbuf());
        pmf->SetOutStream( fout2 );

        char filename[256];
        //int iterNum = 27270;
        int iterNum = numeric_limits<int>::max();
        //iterNum = 20299;

        //if (loopIteration >= 17744) Geometry::qq = true;
        //if (loopIteration >= iterNum) pmf->EraseSmallPolygons(0.0001);

        //if (loopIteration == 16429)  SegmentationParameters::_trigger = 1;
        //if (loopIteration == 20318)  SegmentationParameters::_trigger = 20318;

        if (loopIteration < iterNum)
            sprintf(filename, std::string(std::string(parameters.GetOutputDirectory()) + std::string(parameters.GetOutputPrefix()) + std::string("pre.txt")).c_str() );
        else
            sprintf(filename, "output/%spre%li.txt", parameters.GetOutputPrefix() ? parameters.GetOutputPrefix() : "", loopIteration);
        pmf->SavePMF(filename);

        if (loopIteration < iterNum)
            sprintf(filename, std::string(std::string(parameters.GetOutputDirectory()) + std::string(parameters.GetOutputPrefix()) + std::string("pre.ggb")).c_str() );
        else
            sprintf(filename, "output/%spre%li.ggb", parameters.GetOutputPrefix() ? parameters.GetOutputPrefix() : "", loopIteration);
        pmf->SavePMF(filename, GeoGebraFile);

        clone = pmf->Clone();
        apply = true;

        pmf->SetOutStream( cout );
        cout << "[ SEGM ] :  pre-iteration.end()" << endl;
    }

    void
    BinarySegmentation::PostIteration()
    {
        cout << "[ SEGM ] _" << parameters.GetOutputPrefix() << ":  post-iteration.begin()" << endl;

        //pmf->SavePMF("output/post-before-check-0.ggb", GeoGebraFile);
        cout << " post check 0" << endl;
        assert( pmf->TestPointsCoincidence() );

        cout << " post check 1" << endl;
        delete clone;

        cout << " post check 2" << endl;
        ++loopIteration;

        cout << "[ SEGM ] _" << parameters.GetOutputPrefix() << ":  post-iteration.end()" << endl;
    }


    void
    BinarySegmentation::Prepare()
    {
        cout << "[ SEGM ] : prepare.begin()" << endl;

        areaOfPMF = M_PI * pmf->GetHeight() * pmf->GetWidth();
        storedArea = 1.;

        double pmfeps = numerics->GetDistEpsilon() * 20;
        _sigma6 = (pmfeps * pmfeps) * (pmfeps * pmfeps) * (pmfeps * pmfeps);
        _sigma12 = _sigma6 * _sigma6;
        _rcutoff = 4 * pmfeps;

        cout << "[ PARAM ] :   sigma6 = " << cout.precision(21) << _sigma6 << endl;
        cout << "[ PARAM ] :  sigma12 = " << cout.precision(21) << _sigma12 << endl;
        cout << "[ PARAM ] : r_cutoff = " << cout.precision(21) << _rcutoff << endl;

        //prng = new DoublePRNG(parameters.GetSeed());
        //pmf->SetPRNG (prng);

        cout << "[ SEGM ] : prepare.end()" << endl;
    }

    void
    BinarySegmentation::Finish()
    {
        cout << "[ SEGM ] : finish.begin()" << endl;

        if (outputfile)
        {
            std::string resultfile( std::string(parameters.GetOutputDirectory()) + std::string(parameters.GetOutputPrefix()) + std::string(parameters.GetOutputFile()) );
            pmf->SavePMF (resultfile.c_str());

            std::string resultggb( std::string(parameters.GetOutputDirectory()) + std::string(parameters.GetOutputPrefix()) + std::string("sim-result.ggb") );
            pmf->SavePMF (resultggb.c_str(), GeoGebraFile);

            //pmf->EraseSmallPolygons();
        }
        delete pmf;

        cout << "[ SEGM ] : finish.end()" << endl;
    }


    double
    BinarySegmentation::CalculateImageEnergy() { return pmf->CalculateGrayscaleImageEnergyTerm(img); }

    double
    BinarySegmentation::GetUniform01() { return prng->GetUniform(); }
/*
    void
    BinarySegmentation::SetPRNG(pmf::DoublePRNG * _prng)
    {
        prng = _prng;
        pmf->SetPRNG(prng);
    }
// */
}
