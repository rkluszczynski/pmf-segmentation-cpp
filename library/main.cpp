#include <cstdio>

#include "IntersectionDetector.hpp"

#define VAR(V, N)       __typeof(N) V = (N)
#define FOREACH(I, C)   for(VAR(I, (C).begin()); I != (C).end(); ++I)

int main()
{
    IntersectionDetector id;

    id.AddSegment(.5, .5, 1, 0);
    id.AddSegment(0, 1, .1, .1);
    id.AddSegment(0, .5, .4, .4);
    id.AddSegment(0, 1, 1, .5);
    id.AddSegment(1, .5, 2, .5);

    bool ans = id.CheckIntersectionExistance();
    printf("do %sintersect\n", ans ? "" : "NOT ");

    printf("test\n");
    id.ClearSegments();

    return 0;
}
