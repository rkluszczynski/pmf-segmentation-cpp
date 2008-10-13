#ifndef GRAYSCALEIMAGE_HPP_INCLUDED
#define GRAYSCALEIMAGE_HPP_INCLUDED

#include <iostream>
#include <queue>
#include "CImg.h"
#include "PMF.hpp"

using namespace cimg_library;


class GrayScaleImage
{
    private :
        int ** data;
        int x, y, v, z;

        void ScanVerticalLine(PMF<double> *, double);


    public :
        GrayScaleImage(const char *);
        virtual ~GrayScaleImage();

        inline int GetWidth()   { return x; }
        inline int GetHeight()  { return y; }

        inline int GetPixel(int xx, int yy) { return data[xx][yy]; }

        double CalculateEnergy(PMF<double> *);
};


GrayScaleImage :: GrayScaleImage (const char * filename)
{
    CImg<int> img(filename);
    x = img.dimx();
    y = img.dimy();
    v = img.dimv();
    z = img.dimz();

    printf("[ INFO ] : '%s' - [%i,%i,%i,%i]\n", filename, x,y,v,z);
    if (v > 1 || z > 1) {
        printf("[ WARN ] : color picture - reading only RED value\n");
    }
    //*
    data = (int **) calloc(x, sizeof(int *));
    int * p = img.data;
    for (int i = 0; i < x; i++)
    {
        data[i] = (int *) calloc(y, sizeof(int));
        memcpy(data[i], p, y*sizeof(int));
        p += y;
    }
    //*/
}


GrayScaleImage :: ~GrayScaleImage ()
{
    for (int i = 0; i < x; i++) free(data[i]);
    free(data);
}


double GrayScaleImage :: CalculateEnergy (PMF<double> * conf)
{
/*
float monte2(qTab *Conf, int orientation)
{
  float iter;
  float amount, Ocolor[2] = { 0, 0 };
  float Oblack, Owhite;
  float x, y;

  int qq = 0;
  iter= 0;
  for (x = 0.0; x < WIDTH; x += 0.05)
  {
    y = 1.0;
    iter += Conf->isMonteColor(x, y, orientation, &Oblack, &Owhite);
    Ocolor[0] += Oblack;
    Ocolor[1] += Owhite;
    qq++;
  }
  // fprintf(stderr,"<%li,%li>", Ocolor[0], Ocolor[1]);

  //* Wybieramy wieksza ilosc aby bylo minimum zle sklasyfikowanych. *
  if(Ocolor[0] < Ocolor[1]) amount =  Ocolor[1];
  else amount = Ocolor[0];

  // fprintf(stderr, "(%i) %li >>> ", qq, iter);  fflush(stderr);
  //* Returning the minimum. *
  return((float)(iter-amount)/(float)(iter));
}
//*/
    return 0.0;
}


#define ABS(x)   ( ((x)>0) ? (x) : (-(x)) )
#define MAX(x,y) ( ((x)>(y)) ? (x) : (y) )
void GrayScaleImage :: ScanVerticalLine(PMF<double> * pmf, double xx)
{
    Element<pmf_point<double> > * head = pmf->getFirstElement();

    priority_queue<pair<double, double> > pq;


    //* Wyszukujemy krawedzie przecinajace sie *
    double ymax    = pmf->GetPMFHeight();
    int amountXeq0 = 0;
    for (Element<pmf_point<double> > * temp = head; temp; temp = temp->next)
    {
        if (temp->data->n1)
            if (temp->data->x < temp->data->n1->x) {
                if ( cross3(temp->data->x, temp->data->y, temp->data->n1->x, temp->data->n1->y,
                        xx, 0.0, xx, ymax) > 0
                    )
                {
                    pair<double, double> cpt;
                    crosspoint2(temp->data->x, temp->data->y, temp->data->n1->x, temp->data->n1->y,
                        xx, 0.0, xx, ymax, cpt.first, cpt.second);
                    pq.push(cpt);
                }
            }
        if (temp->data->n2)
            if (temp->data->x < temp->data->n2->x) {
                if ( cross3(temp->data->x, temp->data->y, temp->data->n2->x, temp->data->n2->y,
                        xx, 0.0, xx, ymax) > 0
                    )
                {
                    pair<double, double> cpt;
                    crosspoint2(temp->data->x, temp->data->y, temp->data->n2->x, temp->data->n2->y,
                        xx, 0.0, xx, ymax, cpt.first, cpt.second);
                    pq.push(cpt);
                }
            }
        if (temp->data->x == 0.0)  amountXeq0++;
    }


    double    amount[2] = { 0.0, 0.0 };
    int bottomLeftColor = amountXeq0 % 2;

    while (! pq.empty())
    {
        pair<double, double> pt = pq.top();
        pq.pop();

/*

    while( // xp < pkt.x  &&    yp < pkt.y )
    {
      pic_x = (int)floor(xp*scaleX);
      if (orientation == -1)  pic_x = PIC_WIDTH - pic_x;
      pic_y = (int)floor(yp*scaleY);
      in_pic = PICTURE[pic_y][pic_x];
      dist = ABS(in_pic-(float)kolor);
      if (dist > 0.5) ilosc[0]+=dist;
      else ilosc[1]+=(1.0-dist);
//      if(in_pic == kolor) ilosc[0]++;
//      else ilosc[1]++;
      xp += 0.0; yp += 0.05;
    }
//    cout << "      :   0 -> " << ilosc[0] << "   1 -> " << ilosc[1] << endl;
//    xp = pkt.x;  yp = pkt.y;
    kolor = 1 - kolor;
  }
*/

    }

}
#undef MAX
#undef ABS

/*
bool operator<(const Tpoint& a, const Tpoint& b)
{
  return a.x+a.y>b.x+b.y;
}


float qTab::isMonteColor(float x, float y, int orientation,
    float *Oblack, float *Owhite)
//    long *Oblack, long *Owhite)
{
  float alfa, A, B, xp, yp, tmpy,dist;
  float dl[2] = { 0, 0 };
  int tura, orient;
  struct qTabElem *temp;
  struct Tpoint pkt;
  priority_queue<struct Tpoint> pq;
  long amountXeq0, amountYeq0;

  // cout << " Punkt O = ( " << x << " , " << y << " )" << endl;

  // Wylosowanie prostej losowej
  // alfa = (rand()/((float)RAND_MAX)) * (M_PI * 0.5);
  //  alfa = 0.785398163;  // 45 stopni
  //  alfa = 0.523598776;  // 30 stopni
  //  cout << "Kat alfa = " << alfa << "  (" << (alfa * 180 / M_PI) << ")" << endl;
  if
    //(alfa == (M_PI *0.5))
  (1) { xp = x;  yp = 0.0; y = HEIGHT; }
  else if
    //(alfa == 0.0)
  (1)   { yp = y;  xp = 0.0; x = WIDTH; }
  else {
    A = tan(alfa);
    B = y - A * x;
    if(B <= 0.0) { xp = - B / A;  yp = 0; }
    else { yp = B;  xp = 0; };
    if (B+A*WIDTH >= HEIGHT) { x = (HEIGHT-B)/A; y = HEIGHT; }
    else { y = B+A*WIDTH; x = WIDTH; };
    //   cout << "  Prosta :  y = " << A << " * x + " << B << endl;
  }
  // cout << " Punkt P = ( " << xp << " , " << yp << " )" << endl;

  // Wyszukujemy krawedzie przecinajace sie
  temp = head;
  amountXeq0 = 0;
//  while( temp != NULL  &&  temp->point->x < x )  { temp = temp->next; }
  while( temp != NULL )
  {
    if( xp == 0.0  &&  temp->point->x == 0.0  &&  temp->point->y < yp )
      amountXeq0++;
    if( temp->point->r != NULL  &&
          cross( temp->point->x, temp->point->y,
		 temp->point->r->x, temp->point->r->y, xp, yp, x, y) == 1
	// &&  temp->point->r->x != x  &&  temp->point->r->y != y
	)
    {
      crosspoint( temp->point->x, temp->point->y, temp->point->r->x,
	  temp->point->r->y, xp, yp, x, y, &pkt.x, &pkt.y);

      pq.push(pkt);
//      if(pkt.x <= x  &&  pkt.y <= y) { pq.push(pkt); }
    }
    temp = temp->next;
  }


  if( yp == 0.0 ) {
    amountYeq0 = countYeq0toX(xp);
 // cout << "Ilosc punktow o y==0 : " << amountYeq0 << endl;
   }
  else fprintf(stderr,"ZLE!");
 // else cout << "Ilosc punktow o x==0 : " << amountXeq0 << endl;

  // Wyznaczenie dluzszej roznicy
  // float wymX, wymY;
  // float stepX, stepY;
  // wymX = ABS(x-xp);
  // wymY = ABS(y-yp);
  // if(wymX > wymY)
  // {
  //  orient = 1;
  //   stepX = 1.0 / scaleX;
  //   stepY = (stepX * wymY) / wymX;
  //  }
  // else {
  //   orient = 0;
  //   stepY = 1.0 / scaleY;
  //   stepX = (stepY * wymX) / wymY;
  //  }
  //   cout << "     Steps :  X -> " << stepX << "   Y -> " << stepY << endl;

  float ilosc[2] = {0.0 , 0.0};
  ilosc[0] = 0.0;
  ilosc[1] = 0.0;
  int kolor, pic_x, pic_y;
  float in_pic;

  if(xp == 0.0) kolor = amountXeq0 % 2;
  else kolor = amountYeq0 % 2;

  while(! pq.empty())
  {
    pkt = pq.top();
    pq.pop();

    // cout << pq.size() << " :  ( " << pkt.x << " , " << pkt.y <<
    //  " ) ( " << xp << " , " << yp
    //     << " ) " << endl;

    while( // xp < pkt.x  &&
    yp < pkt.y )
    {
      pic_x = (int)floor(xp*scaleX);
      if (orientation == -1)  pic_x = PIC_WIDTH - pic_x;
      pic_y = (int)floor(yp*scaleY);
      in_pic = PICTURE[pic_y][pic_x];
      dist = ABS(in_pic-(float)kolor);
      if (dist > 0.5) ilosc[0]+=dist;
      else ilosc[1]+=(1.0-dist);
//      if(in_pic == kolor) ilosc[0]++;
//      else ilosc[1]++;
      xp += 0.0; yp += 0.05;
    }
//    cout << "      :   0 -> " << ilosc[0] << "   1 -> " << ilosc[1] << endl;
//    xp = pkt.x;  yp = pkt.y;
    kolor = 1 - kolor;
  }

  while( // xp <= x  &&
  yp < y )
  {
    pic_x = (int)floor(xp*scaleX);
    if (orientation == -1)  pic_x = PIC_WIDTH - pic_x;
    pic_y = (int)floor(yp*scaleY);
    in_pic = PICTURE[pic_y][pic_x] ;
     dist = ABS(in_pic-(float)kolor);
      if (dist > 0.5) ilosc[0]+=dist;
      else ilosc[1]+=(1.0-dist);

    xp += 0.0; yp += 0.05;
  }

//  cout << " ZBADANYCH PIKSELI = " << ilosc[0]+ilosc[1] << endl;
//  cout << " - zgodne z pokolorowaniem OBLACK = " << ilosc[0] << endl;
//  cout << " - zgodne z pokolorowaniem OWHITE = " << ilosc[1] << endl;
  (*Oblack) = ilosc[0];
  (*Owhite) = ilosc[1];
//  fprintf(stderr, " <<<!!! %li !!!>>> ", ilosc[0]+ilosc[1]);  fflush(stderr);
  return(ilosc[0]+ilosc[1]);
}
//*/
#endif // GRAYSCALEIMAGE_HPP_INCLUDED
