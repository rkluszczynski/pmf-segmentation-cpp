#ifndef MACROS_HPP_INCLUDED
#define MACROS_HPP_INCLUDED

#include <cstdio>
#include <cmath>

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <set>
#include <vector>
#include <algorithm>

#define REP(X, N)       for(int X = 0; X < (N); ++X)
#define FOR(X, B, E)    for(int X = B; X <= (E); ++X)
#define FORD(X, B, E)   for(int X = B; X >= (E); --X)

#define VAR(V, N)       __typeof(N) V = (N)
#define FOREACH(I, C)   for(VAR(I, (C).begin()); I != (C).end(); ++I)

using namespace std;

#endif // MACROS_HPP_INCLUDED
