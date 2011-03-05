#ifndef DOUBLEPRNG_H
#define DOUBLEPRNG_H

namespace pmf
{

    class DoublePRNG
    {
        public:
            DoublePRNG();
            virtual ~DoublePRNG();
            DoublePRNG(const DoublePRNG& other);
            DoublePRNG& operator=(const DoublePRNG& other);
        protected:
        private:
    };

}

#endif // DOUBLEPRNG_H
