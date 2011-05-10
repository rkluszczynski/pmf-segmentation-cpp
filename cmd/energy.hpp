#ifndef ENERGY_HPP_INCLUDED
#define ENERGY_HPP_INCLUDED


template <class REAL>
inline
REAL
LJDistTerm(REAL r, REAL sig12, REAL sig6)
{
    REAL tmp = r*r*r*r*r*r;
    tmp = 1. / tmp;
    return tmp * (sig12 * tmp - sig6);

}

template <class REAL>
REAL
PMF<REAL> :: CalculateLennardJonesEnergyTerm (REAL epsilon_LJ, REAL sigma12_LJ, REAL sigma6_LJ, REAL rcut_LJ)
{
    REAL term = 4. * epsilon_LJ;
    REAL energy = 0.;
    REAL LJcutoff = LJDistTerm(rcut_LJ, sigma12_LJ, sigma6_LJ);

    REAL minD = numeric_limits<REAL>::max();
    FOREACH(it, *cf)
    {
        Point<REAL> * pt = *it;
        Point<REAL> * n1 = pt->n1;
        Point<REAL> * n2 = pt->n2;

        REAL dist;
        switch (pt->type)
        {
            case PT_BirthOnBorder :
                                    assert(n1 and not n2);
                                    break;
            case PT_DeathOnBorder :
                                    assert(n2 == NULL);
            case PT_Update        :
                                    dist = pt->CalculateDistance(n1);
                                    minD = min(minD, dist);
                                    if (dist < rcut_LJ)  energy += (LJDistTerm(dist, sigma12_LJ, sigma6_LJ) - LJcutoff);
                                    break;
            case PT_Collision     :
                                    dist = pt->CalculateDistance(n1);
                                    minD = min(minD, dist);
                                    if (dist < rcut_LJ)  energy += (LJDistTerm(dist, sigma12_LJ, sigma6_LJ) - LJcutoff);
                                    dist = pt->CalculateDistance(n2);
                                    minD = min(minD, dist);
                                    if (dist < rcut_LJ)  energy += (LJDistTerm(dist, sigma12_LJ, sigma6_LJ) - LJcutoff);
                                    break;
            case PT_BirthInField  :
                                    assert(n1 and n2);
                                    break;
            default :
                        assert("WRONG POINT TYPE DURING CALCULATING ENERGY" && false);
        }
    }
    energy *= term;
    printf("\n\n MINIMAL DISTANCE = %.21lf / %.21lf\n\n", minD, rcut_LJ);
    printf("            ENERGY  = %.21lf\n\n", energy);
    return energy;
}


template <class REAL>
REAL
PMF<REAL> :: CalculateGrayscaleImageEnergyTerm (GrayscaleImage * img)
{
    REAL oColor[2] = { 0, 0 };
    REAL delta = 0.03999999;

    for (REAL xx = 0.0;  xx < GetWidth();  xx += delta)
    {
        pair<REAL, REAL> res = ScanVerticalLine (img, xx, delta);
        oColor[0] += res.ST;
        oColor[1] += res.ND;
    }
    REAL maxAmount = REAL( max(oColor[0], oColor[1]) );
    REAL     total = REAL( oColor[0] + oColor[1] );
    /*
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
        REAL epsilon = nparams.GetAxisEpsilon();

        switch (pt->type)
        {
            case PT_BirthOnBorder :
                                    if (IsZero(pt->y, epsilon)  &&  pt->x < xx)  ++amountYeq0;
                                    assert(n2 == NULL);
                                    break;
            case PT_DeathOnBorder :
                                    if (IsZero(pt->y, epsilon)  &&  pt->x < xx)  ++amountYeq0;
                                    assert(n2 == NULL);
            case PT_Update        :
                                {
                                    int q1 = CheckIntersection2(pt->x, pt->y, n1->x, n1->y, xx, 0.0, xx, ymax, nparams);
                                    if (q1 > 0)
                                    {
                                        pair<REAL, REAL> cpt = CalculateIntersection(pt->x, pt->y, n1->x, n1->y, xx, 0.0, xx, ymax, nparams);
                                        pq.push(cpt);
                                    }
                                    break;
                                }
            case PT_Collision     :
                                {
                                    int q1 = CheckIntersection2(pt->x, pt->y, n1->x, n1->y, xx, 0.0, xx, ymax, nparams);
                                    if (q1 > 0)
                                    {
                                        pair<REAL, REAL> cpt = CalculateIntersection(pt->x, pt->y, n1->x, n1->y, xx, 0.0, xx, ymax, nparams);
                                        pq.push(cpt);
                                    }
                                    int q2 = CheckIntersection2(pt->x, pt->y, n2->x, n2->y, xx, 0.0, xx, ymax, nparams);
                                    if (q2 > 0)
                                    {
                                        pair<REAL, REAL> cpt = CalculateIntersection(pt->x, pt->y, n2->x, n2->y, xx, 0.0, xx, ymax, nparams);
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

            amount[0] += (dist);
            amount[1] += (255 - dist);

            /*
            if (dist < 128)
            {
                int addon = dist;
                addon = 0;
                amount[0] += (addon + 1);
            }
            else
            {
                int addon = 255 - dist;
                addon = 0;
                amount[1] += (addon + 1);
            }
            // */
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
