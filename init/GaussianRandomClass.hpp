#ifndef GAUSSIANRANDOMCLASS_HPP
#define GAUSSIANRANDOMCLASS_HPP

#include <memory>


class GaussianRandomClass
{
    public :
        typedef std::auto_ptr<GaussianRandomClass> InstancePointer;

        inline static InstancePointer & Instance()
        {
            static InstancePointer singleton(new GaussianRandomClass());
            return singleton;
        }
        virtual ~GaussianRandomClass();

        inline static double GetGaussian()                     { return Instance()->nextGaussian(); }
        inline static double GetGaussian(double m, double u)   { return (Instance()->nextGaussian() + m) * u; }
        inline static double GetGaussianWithMean(double m)     { return Instance()->nextGaussian() + m; }
        inline static double GetGaussianWithVariance(double u) { return Instance()->nextGaussian() * u; }

    protected:
        GaussianRandomClass();

        double nextDouble();
        double nextGaussian();

    private:
        bool haveNextNextGaussian;
        double nextNextGaussian;
        double randMax_1;
};

#endif // GAUSSIANRANDOMCLASS_HPP
