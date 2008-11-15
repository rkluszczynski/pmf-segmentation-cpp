#ifndef GRAYSCALEIMAGE_HPP_INCLUDED
#define GRAYSCALEIMAGE_HPP_INCLUDED

#include <iostream>
#include <queue>
#include <cmath>
#include "CImg.h"
#include "PMF.hpp"

using namespace cimg_library;


class GrayScaleImage
{
    private :
        int ** data;
        int x, y, v, z;


    public :
        GrayScaleImage(const char *);
        virtual ~GrayScaleImage();

        inline int GetWidth()   { return x; }
        inline int GetHeight()  { return y; }

        inline int GetPixelValue(int xx, int yy) { return data[xx][yy]; }
        double GetGradientValue(int xx, int yy, pair<double, double> * Gxy = NULL);

        EdgePoints<double> * GetRandomEdgePixels(int, int, double, double);

        void ScanVerticalLine(PMF<double> *, double, int *);

        double CalculateScanLinesEnergy(PMF<double> *);
};


double asd2(double gx, double gy) {
    if (gx == 0.0  &&  gy == 0.0)  return 0.0;

    double aaa = atan2(gx, gy);
    if (aaa < 0.0) aaa += (2.0 * M_PI);
    if (aaa < M_PI_2)  aaa += M_PI;
    if (aaa >= 1.5 * M_PI)  aaa -= M_PI;
    return aaa;
}

EdgePoints<double> * GrayScaleImage :: GetRandomEdgePixels(int amount, int min_dist, double fWidth, double fHeight)
{
    EdgePoints<double> * ep = new EdgePoints<double>();
    vector<pair<int, int> > pixels;

    int min_dist2 = min_dist * min_dist;
    int i = 0;

    // Searching maximal gradient value
    double maks = 0.0;
    for (int ix = 1; ix < GetWidth()-1; ix++) {
        for (int iy = 1; iy < GetHeight()-1; iy++) {
            double grad = GetGradientValue(ix, iy);
            if (grad > maks) maks = grad;
        }
    }

    // Getting random points
    while (i < amount)
    {
        bool nextone = true;
        int xx = rand() % (GetWidth()-2) + 1;
        int yy = rand() % (GetHeight()-2) + 1;

        // Checking the distance with others
        if (min_dist > 0)
            for (unsigned int k = 0; k < pixels.size(); k++) {
                int dist2 = (pixels[k].first - xx)*(pixels[k].first - xx) +
                    (pixels[k].second - yy)*(pixels[k].second - yy);
                if (dist2 < min_dist2) {
                    nextone = false;
                }
            }

        if (nextone) {
            pair<double, double> G;
            double limit = GetGradientValue(xx, yy, &G) / maks;
            double fate = Uniform(0.0, 1.0);
            if (limit > 0.0  &&  fate <= limit) {
                double aaa = asd2(G.first, G.second);

                if (rand() & 0x1)  aaa += M_PI_2;
                else  aaa -= M_PI_2;

                pair<int, int> pixel(xx, yy);
                pixels.push_back(pixel);
                //cerr << " point  " << xx << " x " << yy << endl;
                //cerr << " image  " << GetWidth() << " x " << GetHeight() << endl;
                //cerr << "  pmf   " << fWidth << " x " << fHeight << endl;
                ep->AddEdgePoint( (double(xx) / double(GetWidth())) * fWidth,
                                 (double(yy) / double(GetHeight())) * fHeight, aaa);

                cerr << " point accepted " << endl;
                //ep->PrintData(cerr);
                i++;
            }
        }
    }
    return ep;
}


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


double GrayScaleImage :: GetGradientValue (int xx, int yy, pair<double, double> * Gxy)
{
    double pix1 = GetPixelValue(xx-1, yy-1);
    double pix2 = GetPixelValue(xx, yy-1);
    double pix3 = GetPixelValue(xx+1, yy-1);
    double pix4 = GetPixelValue(xx-1, yy);

    double pix6 = GetPixelValue(xx+1, yy);
    double pix7 = GetPixelValue(xx-1, yy+1);
    double pix8 = GetPixelValue(xx, yy+1);
    double pix9 = GetPixelValue(xx+1, yy+1);

    double Gx = pix3 + 2.0 * pix6 + pix9 - pix1 - 2.0 * pix4 - pix7;
    double Gy = pix1 + 2.0 * pix2 + pix3 - pix7 - 2.0 * pix8 - pix9;

    if (Gxy) {
        Gxy->first = Gx;
        Gxy->second = Gy;
    }
    return sqrt(Gx*Gx+Gy*Gy);
}


double GrayScaleImage :: CalculateScanLinesEnergy (PMF<double> * pmf)
{
    int oColor[2] = { 0, 0 };
    for (double xx = 0.0; xx < pmf->GetPMFWidth(); xx += 0.05)
    {
        int amount[2];
        ScanVerticalLine(pmf, xx, amount);

        oColor[0] += amount[0];
        oColor[1] += amount[1];
    }
    double maxAmount = double( (oColor[0] > oColor[1]) ? oColor[0] : oColor[1] );
    double     total = double( oColor[0] + oColor[1] );

    //cout << " oBLACK = " << (total - oColor[0]) / total << endl;
    //cout << " oWHITE = " << (total - oColor[1]) / total << endl;
    return (total - maxAmount)/total;
}


#define ABS(x)   ( ((x)>0) ? (x) : (-(x)) )
void GrayScaleImage :: ScanVerticalLine(PMF<double> * pmf, double xx, int * originColor = NULL)
{
    Element<pmf_point<double> > * head = pmf->getFirstElement();
    priority_queue<pair<double, double>, vector<pair<double, double> >, greater<pair<double, double> > > pq;

    //* Searching for segments crossing with vertical line *
    double ymax    = pmf->GetPMFHeight();
    int amountYeq0 = 0;
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
        if (temp->data->y == 0.0  &&  temp->data->x <= xx)  amountYeq0++;
    }

    double scaleX = GetWidth()  / pmf->GetPMFWidth();
    double scaleY = GetHeight() / pmf->GetPMFHeight();

    int     amount[2] = { 0, 0 };
    int startTopColor = (amountYeq0 % 2 == 0) ? 0 : 255;
    double         yy = 0.0;
    //cout << "[ StartTopColor ] = " << startTopColor << "   [ " << amountYeq0 << " ]" << endl;

    // * Counting matching pixels *
    while (! pq.empty())
    {
        pair<double, double> pt = pq.top();
        pq.pop();
        //cout << " [ POINT ] : " << pt.first << "  x  " << pt.second << "   COLOR = " << startTopColor << endl;

        while (yy < pt.second)
        {
            int pixel = GetPixelValue(int(xx * scaleX), int(yy * scaleY));
            int  dist = ABS(pixel - startTopColor);
            //cout << "   PIXEL   : " << int(xx * scaleX) << "  x  " << int(yy * scaleY) << " = " << pixel << "  {" << dist << "}" << endl;

            if (dist < 128) amount[0] += (dist + 1);
            else    amount[1] += (255 - dist + 1);
            //cout << "           : " << amount[0] << " , " << amount[1] << endl;

            yy += 0.05;
        }
        startTopColor = 255 - startTopColor;
    }

    // * Saving result if necessary *
    if (originColor) {
        originColor[0] = amount[0];
        originColor[1] = amount[1];
    }
    //cout << " ZBADANYCH PIKSELI = " << amount[0]+amount[1] << endl;
    //cout << " - zgodne z pokolorowaniem OBLACK = " << amount[0] << endl;
    //cout << " - zgodne z pokolorowaniem OWHITE = " << amount[1] << endl;
    return;
}
#undef ABS


#endif // GRAYSCALEIMAGE_HPP_INCLUDED
