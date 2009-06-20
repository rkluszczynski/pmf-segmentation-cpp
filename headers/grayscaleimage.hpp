#ifndef GRAYSCALEIMAGE_HPP_INCLUDED
#define GRAYSCALEIMAGE_HPP_INCLUDED

#include <iostream>
#include <queue>
#include <cmath>

#define cimg_display 0
#include "CImg.h"

#include "macros.hpp"
#include "PMF.hpp"

using namespace cimg_library;


class GrayScaleImage
{
    private :
        int ** data;
        int x, y, v, z;

        double GetAtan2Angle(double gx, double gy);


    public :
        GrayScaleImage(const char *);
        GrayScaleImage(int, int);
        virtual ~GrayScaleImage();

        inline int GetWidth()   { return x; }
        inline int GetHeight()  { return y; }

        inline void MakeContrast(int);

        inline void PutPixelValue(int xx, int yy, int value) { data[xx][yy] = value; }
        inline
        int GetPixelValue(int xx, int yy)
        {
            xx = (xx>=GetWidth()) ? GetWidth()-1 : ((xx<0) ? 0 : xx);
            yy = (yy>=GetHeight()) ? GetHeight()-1 : ((yy<0) ? 0 : yy);
            return data[xx][yy];
        }
        double GetGradientValue(int xx, int yy, pair<double, double> * Gxy = NULL);

        EdgePoints<double> * GetRandomEdgePixels(int, int, double, double);
        EdgePoints<double> * GetRandomEdgePixels2(unsigned int &, int, double, double);

        void ScanVerticalLine(PMF<double> *, double, double, int *);

        double CalculateScanLinesEnergy(PMF<double> *);
};


void GrayScaleImage :: MakeContrast(int exponent = 2)
{
    double maxval = 256.0;
    double inv256 = 1.0 / maxval;
    REP(ix,x) REP(iy,y) {
        double normval = GetPixelValue(ix,iy) * inv256;
        normval = (normval < 0.5) ? pow(normval, exponent) : 1.-pow(1.-normval, exponent);
        int pix = int(normval * maxval);
        PutPixelValue(ix,iy,pix);
    }
}


double GrayScaleImage :: GetAtan2Angle(double gx, double gy)
{
    if (gx == 0.0  &&  gy == 0.0)  return 0.0;

    double aaa = atan2(gx, gy);
    if (aaa < 0.0) aaa += (2.0 * M_PI);
    if (aaa < M_PI_2)  aaa += M_PI;
    if (aaa >= 1.5 * M_PI)  aaa -= M_PI;
    return aaa;
}
#define LOG2CERR 0
EdgePoints<double> * GrayScaleImage :: GetRandomEdgePixels2(unsigned int & amount, int min_dist, double fWidth, double fHeight)
{
    EdgePoints<double> * ep = new EdgePoints<double>();
    vector<pair<int, int> > grad_pixels;
    vector<pair<int, int> > selected_pixels;

    int min_dist2 = min_dist * min_dist;
    unsigned int i = 0;
    double cutoff = 0.3;

    // Searching maximal gradient value
    double max_grad = 0.0;
    for (int ix = 0; ix < GetWidth(); ix++) {
        for (int iy = 0; iy < GetHeight(); iy++) {
            double grad = GetGradientValue(ix, iy);
            if (grad > max_grad) max_grad = grad;
        }
    }
    double cutoffgrad = cutoff * max_grad;

    for (int ix = 0; ix < GetWidth(); ix++) {
        for (int iy = 0; iy < GetHeight(); iy++) {
            double grad = GetGradientValue(ix, iy);
            if(grad > cutoffgrad) {
                grad_pixels.push_back( pair<int,int>(ix,iy) );
            }
        }
    }
    if(grad_pixels.size() < amount)  amount = grad_pixels.size();

    // Getting random points
    while (i < amount)
    {
        bool nextone = true;
        int rrand = rand() % grad_pixels.size();
        int xx = grad_pixels[rrand].first;
        int yy = grad_pixels[rrand].second;
        swap(grad_pixels[rrand], grad_pixels[grad_pixels.size()-1]);
        grad_pixels.pop_back();
        cerr << ".";

        // Checking the distance with others
        if (min_dist > 0) {
            for (unsigned int k = 0; k < selected_pixels.size(); k++) {
                int dist2 = (selected_pixels[k].first - xx)*(selected_pixels[k].first - xx) +
                    (selected_pixels[k].second - yy)*(selected_pixels[k].second - yy);
                if (dist2 < min_dist2) {
                    nextone = false;
                }
            }
            if (nextone == false) cerr << "X";
        }

        if (nextone) {
            pair<double, double> G;
            double limit = GetGradientValue(xx, yy, &G) / max_grad;
            if (limit > 0.0) {
                double fate = Uniform(0.0, 1.0);
                cerr << "{" << limit << ">=" << fate << "}";
                if (fate <= limit) {
                    double aaa = GetAtan2Angle(G.first, G.second);
#if LOG2CERR
                    cerr << endl << " point  ( " << xx << " ; " << yy << " )" ;
                    cerr << "  angle : " << aaa << " ~ " << radians2degree(aaa) << endl;
                    cerr << " point accepted " << endl;
#endif
                    if (rand() & 0x1)  aaa += M_PI_2;  else  aaa -= M_PI_2;

                    pair<int, int> pixel(xx, yy);
                    selected_pixels.push_back(pixel);
                    ep->AddEdgePoint( (double(xx) / double(GetWidth())) * fWidth,
                                    (double(yy) / double(GetHeight())) * fHeight, aaa);

                    ep->PrintData(cerr);
                    i++;
                }
            }
            else cerr << "0";
        }
        else {
            if(amount > grad_pixels.size()) amount = grad_pixels.size();
        }
    }
    return ep;
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
        cerr << ".";

        // Checking the distance with others
        if (min_dist > 0) {
            for (unsigned int k = 0; k < pixels.size(); k++) {
                int dist2 = (pixels[k].first - xx)*(pixels[k].first - xx) +
                    (pixels[k].second - yy)*(pixels[k].second - yy);
                if (dist2 < min_dist2) {
                    nextone = false;
                }
            }
            if (nextone == false) cerr << "X";
        }

        if (nextone) {
            pair<double, double> G;
            double limit = GetGradientValue(xx, yy, &G) / maks;
            if (limit > 0.0) {
                double fate = Uniform(0.0, 1.0);
                cerr << "{" << limit << ">=" << fate << "}";
                if (fate <= limit) {
                    double aaa = GetAtan2Angle(G.first, G.second);
#if LOG2CERR
                    cerr << endl << " point  ( " << xx << " ; " << yy << " )" ;
                    cerr << "  angle : " << aaa << " ~ " << radians2degree(aaa) << endl;
                    cerr << " point accepted " << endl;
#endif
                    if (rand() & 0x1)  aaa += M_PI_2;  else  aaa -= M_PI_2;

                    pair<int, int> pixel(xx, yy);
                    pixels.push_back(pixel);
                    ep->AddEdgePoint( (double(xx) / double(GetWidth())) * fWidth,
                                    (double(yy) / double(GetHeight())) * fHeight, aaa);

                    ep->PrintData(cerr);
                    i++;
                }
            }
            else cerr << "0";
        }
    }
    return ep;
}
#undef LOG2CERR


GrayScaleImage :: GrayScaleImage (int xx, int yy)
{
    x = xx;
    y = yy;
    v = z = 0;

    data = (int **) calloc(x, sizeof(int *));
    for (int ix = 0; ix < x; ix++)
        data[ix] = (int *) calloc(y, sizeof(int));
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
    for (int ix = 0; ix < x; ix++)
    {
        data[ix] = (int *) calloc(y, sizeof(int));
        for (int iy = 0; iy < y; iy++) {
            data[ix][iy] = img(ix, iy, 0, 0);
        }
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

    //double Gx = pix3 + 2.0 * pix6 + pix9 - pix1 - 2.0 * pix4 - pix7;
    double Gx = pix9 + 2.0 * pix6 + pix3 - pix7 - 2.0 * pix4 - pix1;
    //double Gy = pix1 + 2.0 * pix2 + pix3 - pix7 - 2.0 * pix8 - pix9;
    double Gy = pix7 + 2.0 * pix8 + pix9 - pix1 - 2.0 * pix2 - pix3;

    if (Gxy) {
        Gxy->first = Gx;
        Gxy->second = Gy;
    }
    return sqrt(Gx*Gx+Gy*Gy);
}


double GrayScaleImage :: CalculateScanLinesEnergy (PMF<double> * pmf)
{
    int oColor[2] = { 0, 0 };
    double delta = 0.04999999;

    for (double xx = 0.0; xx < pmf->GetPMFWidth(); xx += delta)
    {
        int amount[2];
        ScanVerticalLine(pmf, xx, delta, amount);

        oColor[0] += amount[0];
        oColor[1] += amount[1];
    }
    double maxAmount = double( max(oColor[0], oColor[1]) );
    double     total = double( oColor[0] + oColor[1] );
    /*
    cout << " oBLACK = " << (total - oColor[0]) / total << endl;
    cout << " oWHITE = " << (total - oColor[1]) / total << endl;
    //*/
    return (total - maxAmount)/total;
}


#define ABS(x)   ( ((x)>0) ? (x) : (-(x)) )
void GrayScaleImage :: ScanVerticalLine(PMF<double> * pmf, double xx, double delta, int * originColor = NULL)
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
    pair<double, double> lastPt(xx, pmf->GetPMFHeight());
    pq.push(lastPt);

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

            //yy += (1.0 / scaleY);
            yy += delta;
        }
        startTopColor = 255 - startTopColor;
    }

    // * Saving result if necessary *
    if (originColor) {
        originColor[0] = amount[0];
        originColor[1] = amount[1];
    }
    /*
    cout << " ZBADANYCH PIKSELI = " << amount[0]+amount[1] << endl;
    cout << " - zgodne z pokolorowaniem OBLACK = " << amount[0] << endl;
    cout << " - zgodne z pokolorowaniem OWHITE = " << amount[1] << endl;
    //*/
    return;
}
#undef ABS


#endif // GRAYSCALEIMAGE_HPP_INCLUDED
