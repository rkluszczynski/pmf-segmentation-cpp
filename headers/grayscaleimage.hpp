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


    public :
        GrayScaleImage(const char *);
        virtual ~GrayScaleImage();

        inline int GetWidth()   { return x; }
        inline int GetHeight()  { return y; }

        inline int GetPixel(int xx, int yy) { return data[xx][yy]; }

        void ScanVerticalLine(PMF<double> *, double, int *);

        double CalculateScanLinesEnergy(PMF<double> *);
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
            int pixel = GetPixel(int(xx * scaleX), int(yy * scaleY));
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
