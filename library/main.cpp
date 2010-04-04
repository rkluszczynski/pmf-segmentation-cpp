#include <cstdio>

#include "IntersectionDetector.hpp"

#define VAR(V, N)       __typeof(N) V = (N)
#define FOREACH(I, C)   for(VAR(I, (C).begin()); I != (C).end(); ++I)

int main()
{
    printf("%i\n", sizeof(void *));
    //return 0;
    printf("test 1\n");
    //if (false)
    {
        IntersectionDetector id;

        id.AddSegment(.5, .5, 1, 0);
        id.AddSegment(0, 1, .1, .1);
        id.AddSegment(0, .5, .4, .4);
        id.AddSegment(0, 1, 1, .5);
        id.AddSegment(1, .5, 2, .5);

        bool ans = id.CheckIntersectionExistance();
        printf("do %sintersect\n", ans ? "" : "NOT ");

        id.ClearSegments();
    }
    printf("test 2\n");
    //if (false)
    {
        IntersectionDetector id;

        id.AddSegment(0, 0, 0, 3);
        id.AddSegment(0, 0, 3, 0);
        id.AddSegment(3, 0, 3, 3);
        id.AddSegment(0, 3, 3, 3);

        bool ans = id.CheckIntersectionExistance();
        printf("do %sintersect\n", ans ? "" : "NOT ");

        id.ClearSegments();
    }
    return 0;
}
