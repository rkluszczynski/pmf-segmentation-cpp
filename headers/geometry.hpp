#ifndef __GEOMETRY_HPP__
#define __GEOMETRY_HPP__


#ifndef EPSILON
#define EPSILON 0.0000005
#endif


#define ABS(x)   ( ((x)>0) ? (x) : (-(x)) )
template <class T_REAL>
int sgnDet(T_REAL xp, T_REAL yp, T_REAL xq, T_REAL yq, T_REAL xr, T_REAL yr)
{
  T_REAL tmp;
  tmp = xp*yq + xr*yp + xq*yr - xr*yq - xq*yp - xp*yr;
  if(ABS(tmp) < EPSILON) return(0);
  else if(tmp > 0.0) return(1);
  else return(-1);
}
#undef ABS


#define MIN(x,y) ( ((x)<(y)) ? (x) : (y) )
#define MAX(x,y) ( ((x)>(y)) ? (x) : (y) )
template <class T_REAL>
int isOnSegment(T_REAL xp, T_REAL yp, T_REAL xq, T_REAL yq, T_REAL xr, T_REAL yr)
/* Return values:
 *   1 - point (xr,yr) belongs to a segment (xp,yp)-(xq,yq)
 *   0 - point is outside the segment
 */
{
     if( sgnDet(xp, yp, xq, yq, xr, yr) == 0
         &&  MIN(xp,xq) <= xr  &&  xr <= MAX(xp,xq)
         &&  MIN(yp,yq) <= yr  &&  yr <= MAX(yp,yq) )
         return(1);
     return(0);
}
#undef MAX
#undef MIN


/* Function 'cross3' added 4 Nov 2005 */
template <class T_REAL>
int cross3( T_REAL xp, T_REAL yp, T_REAL xq, T_REAL yq /* Line 1 ( p-q ) */,
		  T_REAL xr, T_REAL yr, T_REAL xs, T_REAL ys /* Line 2 ( r-s ) */ )
/* Returning values:
 *    0  - lines do not cross
 *    1  - lines crosses each others
 *  2..5 - end of one line belongs to other line
 */
{
  int sgnDetPQR, sgnDetPQS, sgnDetRSP, sgnDetRSQ;

  sgnDetPQR = sgnDet(xp, yp, xq, yq, xr, yr);
  sgnDetPQS = sgnDet(xp, yp, xq, yq, xs, ys);
  sgnDetRSP = sgnDet(xr, yr, xs, ys, xp, yp);
  sgnDetRSQ = sgnDet(xr, yr, xs, ys, xq, yq);

  if( (sgnDetPQR != 0  ||  sgnDetPQS != 0)  &&
      (sgnDetRSP != 0  ||  sgnDetRSQ != 0)  &&
      sgnDetPQR == - sgnDetPQS  &&  sgnDetRSP == - sgnDetRSQ
    )
    return(1);
  if(isOnSegment(xp, yp, xq, yq, xr, yr) == 1) return(2);
  if(isOnSegment(xp, yp, xq, yq, xs, ys) == 1) return(3);
  if(isOnSegment(xr, yr, xs, ys, xp, yp) == 1) return(4);
  if(isOnSegment(xr, yr, xs, ys, xq, yq) == 1) return(5);
  return(0);
}


#define ABS(x)   ( ((x)>0) ? (x) : (-(x)) )
template <class T_REAL>
void crosspoint2( T_REAL x3, T_REAL y3, T_REAL x4, T_REAL y4,
		T_REAL x1, T_REAL y1, T_REAL x2, T_REAL y2, T_REAL & x, T_REAL & y )
{
	T_REAL a, b, c, d;
	if( x1 != x2 )
	{
		a = (y1 - y2)/(x1 - x2);
		b = y1 - a * x1;
		if( x3 != x4 )
		{
			c = (y3 - y4)/(x3 - x4);
			d = y3 - c * x3;

			x = (d - b)/(a - c);
			y = a * (x) + b;
		}
		else {
			x = x3;
			y = a * (x) + b;
		}
	}
	else {
		c = (y3 - y4)/(x3 - x4);
		d = y3 - c * x3;

		x = x1;
		y = c * (x) + d;
	}
}
#undef ABS

#define radians2degree(X) (X * 180.0 / M_PI)

#define degree2radians(X) (X * M_PI / 180.0)


#endif /* __GEOMETRY_HPP__ */
