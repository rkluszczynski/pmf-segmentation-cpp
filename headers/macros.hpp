#ifndef MACROS_HPP_INCLUDED
#define MACROS_HPP_INCLUDED

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cfloat>
#include <climits>
#include <cmath>
#include <ctime>
#include <limits>
#include <unistd.h>
#include <sys/timeb.h>

#include <iostream>
#include <ostream>
#include <fstream>
#include <string>
#include <map>
#include <set>
#include <vector>
#include <algorithm>
#include <utility>

#include <wx/wx.h>
#include <wx/log.h>
#include <wx/dcmemory.h>
#include <wx/wfstream.h>
#include <wx/zipstrm.h>
#include <wx/txtstrm.h>

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


namespace pmf
{
    void GetTheFieldAndDoTheModification (double, double, time_t, const char *, const char *, double, double, double, struct timeb *, struct timeb *);
    void GetTheFieldAndDoTheModification (double, double, time_t, const char *, const char *, long, double, struct timeb *, struct timeb *);
}


#endif // MACROS_HPP_INCLUDED
