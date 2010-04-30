#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>
#include <omp.h>
#include <unistd.h>
#include <pthread.h>

#include <windows.h>  /* for sleep() in dos/mingw */
#define sleep(seconds) Sleep((seconds)*100) /* from mingw.org list */

using namespace std;


void runInParallel(int n)
{
    omp_set_num_threads(n);
    vector<int> sims(n, 0);
    time_t czas = time(NULL);
#pragma omp parallel default(none) shared(sims, czas)
    {
        int id = omp_get_thread_num();
        printf("%i : sims[%i] = %i\n", id, id, sims[id]);
        sims[id] = id;
        srand(czas + id);
    }
    printf("::begin()\n");
    vector<int> run(n, 0);
    bool sync = false;
    bool done = false;
#pragma omp parallel default(none) shared(run, sims, sync, done)
    {
        int id = omp_get_thread_num();
        printf("%i : sims[%i] = %i\n", id, id, sims[id]);
        bool runIt = true;
        while (runIt)
        {
            unsigned czas = rand() % 70 + 10;
            printf("%i : [%i] sleeping %u sec. \n", id, run[id], czas);
            sleep(czas);
            if (czas == 58) done = true;
            printf("%i : [%i] awaken (%c)\n", id, run[id], done ? 'T' : 'F');

            if (sync  or  run[id] == 3  or  done)
            {
                #pragma omp critical
                    sync = true;
                #pragma omp barrier
                #pragma omp master
                    {
                        printf("sync::begin()\n");
                        for (int i = 0; i < omp_get_num_threads(); ++i)  sims[i] = 17;
                        //srand(id);
                        sync = false;
                        fill(run.begin(), run.end(), -1);
                        scanf("%*c");
                        printf("sync::end()\n");
                    }
                #pragma omp barrier
                    if (done) runIt = false;
            }
            ++run[id];
        }
        printf("%i : finito\n", id);
    }
    printf("::end()\n");
#pragma omp parallel default(none) shared(sims)
    printf("%i : sims[%i] = %i\n", omp_get_thread_num(), omp_get_thread_num(), sims[omp_get_thread_num()]);
}


int main()
{
#pragma omp parallel
    printf("Hello, world!\n");

    runInParallel(3);
    return 0;
}
