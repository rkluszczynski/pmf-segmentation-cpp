#ifndef ENERGY_HPP_INCLUDED
#define ENERGY_HPP_INCLUDED


template <class REAL>
REAL
PMF<REAL> :: CalculateEnergy (GrayscaleImage * img)
{
    int oColor[2] = { 0, 0 };
    REAL delta = 0.04999999;

    for (REAL xx = 0.0;  xx < GetWidth();  xx += delta)
    {
        pair<REAL, REAL> res = ScanVerticalLine (img, xx, delta);
        oColor[0] += res.ST;
        oColor[1] += res.ND;
    }
    REAL maxAmount = REAL( max(oColor[0], oColor[1]) );
    REAL     total = REAL( oColor[0] + oColor[1] );
    //*
    cout << " oBLACK = " << (total - oColor[0]) / total << endl;
    cout << " oWHITE = " << (total - oColor[1]) / total << endl;
    //*/
    return (total - maxAmount) / total;
}


template <class REAL>
pair<REAL, REAL>
PMF<REAL> :: ScanVerticalLine(GrayscaleImage * img, REAL xx, REAL delta)
{
    using namespace Geometry;
    priority_queue<pair<REAL, REAL>, vector<pair<REAL, REAL> >, greater<pair<REAL, REAL> > > pq;

    REAL    ymax = GetHeight();
    int amountYeq0 = 0;

    FOREACH(it, *cf)
    {
        Point<REAL> * pt = *it;
        Point<REAL> * n1 = pt->n1;
        Point<REAL> * n2 = pt->n2;

        switch (pt->type)
        {
            case PT_BirthOnBorder :
                                    if (IsZero(pt->y)  &&  pt->x < xx)  ++amountYeq0;
                                    assert(n2 == NULL);
                                    break;
            case PT_DeathOnBorder :
                                    if (IsZero(pt->y)  &&  pt->x < xx)  ++amountYeq0;
                                    assert(n2 == NULL);
            case PT_Update        :
                                {
                                    int q1 = CheckIntersection2(pt->x, pt->y, n1->x, n1->y, xx, 0.0, xx, ymax);
                                    if (q1 > 0)
                                    {
                                        pair<REAL, REAL> cpt = CalculateIntersection(pt->x, pt->y, n1->x, n1->y, xx, 0.0, xx, ymax);
                                        pq.push(cpt);
                                    }
                                    break;
                                }
            case PT_Collision     :
                                {
                                    int q1 = CheckIntersection2(pt->x, pt->y, n1->x, n1->y, xx, 0.0, xx, ymax);
                                    if (q1 > 0)
                                    {
                                        pair<REAL, REAL> cpt = CalculateIntersection(pt->x, pt->y, n1->x, n1->y, xx, 0.0, xx, ymax);
                                        pq.push(cpt);
                                    }
                                    int q2 = CheckIntersection2(pt->x, pt->y, n2->x, n2->y, xx, 0.0, xx, ymax);
                                    if (q2 > 0)
                                    {
                                        pair<REAL, REAL> cpt = CalculateIntersection(pt->x, pt->y, n2->x, n2->y, xx, 0.0, xx, ymax);
                                        pq.push(cpt);
                                    }
                                    break;
                                }
            case PT_BirthInField  :
                                    break;
            default :
                        assert("WRONG POINT TYPE DURING CALCULATING ENERGY" && false);
        }
    }
    pair<REAL, REAL> lastPt(xx, GetHeight());
    pq.push(lastPt);

    REAL scaleX = img->GetWidth() / GetWidth();
    REAL scaleY = img->GetHeight() / GetHeight();

    int     amount[2] = { 0, 0 };
    int startTopColor = (amountYeq0 % 2 == 0) ? 0 : 255;
    REAL         yy = 0;
    //cout << "[ StartTopColor ] = " << startTopColor << "   [ " << amountYeq0 << " ]" << endl;

    // * Counting matching pixels *
    while (! pq.empty())
    {
        pair<REAL, REAL> pt = pq.top();
        pq.pop();
        //cout << " [ POINT ] : " << pt.first << "  x  " << pt.second << "   COLOR = " << startTopColor << endl;

        while (yy < pt.second)
        {
            int pixel = int( (*img)[ img->GetHeight() - int(yy*scaleY) ][ int(xx*scaleX) ][0] );
            //int pixel = int( (*img)[ int(xx*scaleX) ][ int(yy*scaleY) ][0] );
            int dist  = abs(pixel - startTopColor);
            //cout << "   PIXEL   : " << int(xx * scaleX) << "  x  " << int(yy * scaleY) << " = " << pixel << "  {" << dist << "}" << endl;

            if (dist < 128)  amount[0] += (dist + 1);
            else  amount[1] += (255 - dist + 1);
            //cout << "           : " << amount[0] << " , " << amount[1] << endl;

            yy += (1. / scaleY);
            //yy += delta;
        }
        startTopColor = 255 - startTopColor;
    }
    /*
    cout << " ZBADANYCH PIKSELI = " << amount[0]+amount[1] << endl;
    cout << " - zgodne z pokolorowaniem OBLACK = " << amount[0] << endl;
    cout << " - zgodne z pokolorowaniem OWHITE = " << amount[1] << endl;
    //*/
    return pair<REAL, REAL> (amount[0], amount[1]);
}


#endif // ENERGY_HPP_INCLUDED
