#ifndef MACROS_HPP_INCLUDED
#define MACROS_HPP_INCLUDED

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <climits>
#include <cmath>
#include <ctime>
#include <unistd.h>
#include <sys/timeb.h>

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <set>
#include <vector>
#include <algorithm>

#include <wx/wx.h>
#include <wx/log.h>

#include <cassert>


#define ST first
#define ND second

#define REP(X, N)       for(int X = 0; X < (N); ++X)
#define FOR(X, B, E)    for(int X = B; X <= (E); ++X)
#define FORD(X, B, E)   for(int X = B; X >= (E); --X)

#define VAR(V, N)       __typeof(N) V = (N)
#define FOREACH(I, C)   for(VAR(I, (C).begin()); I != (C).end(); ++I)

using namespace std;


#define EPSILON 1e-7

#define PMFLog(FRT, ARGS...)  wxLogMessage(wxString::Format(wxT(FRT), ##ARGS));
#define PMFLogV(FRT, ARGS...) wxLogVerbose(wxString::Format(wxT(FRT), ##ARGS));


#endif // MACROS_HPP_INCLUDED
