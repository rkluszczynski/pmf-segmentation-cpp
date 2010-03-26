#include "MosaicGraph.hpp"

#include "IntersectionDetector.hpp"
#include <fstream>

#define REP(X,N)        for(int X = 0; X < (N); ++X)


MosaicGraph::MosaicGraph()
{
    //ctor
}


MosaicGraph::~MosaicGraph()
{
    //dtor
}


bool
MosaicGraph::IsThereAnIntersection()
{
    IntersectionDetector detect;
    FOREACH(it, nodes)
    {
        MosaicGraphNode * node = *it;
        assert(node->Size() == 2);

        unsigned n1 = node->Front()->GetId();
        unsigned n2 = node->Back()->GetId();

        if (node->GetId() < n1)
        {
            detect.AddSegment(node->x(), node->y(), nodes[n1]->x(), nodes[n1]->y());
        }
        if (node->GetId() < n2)
        {
            detect.AddSegment(node->x(), node->y(), nodes[n2]->x(), nodes[n2]->y());
        }
    }
    bool answer = detect.CheckIntersectionExistance();
    return answer;
}


void
MosaicGraph::MakeGaussianShakeToDisorder (double u)
{
    double   width = MosaicConstants::GetPmfWidth();
    double  height = MosaicConstants::GetPmfHeight();
    double epsilon = 2. * MosaicConstants::GetEpsilon();

    FOREACH(it, nodes)
    {
        MosaicGraphNode * node = *it;

        if (node->GetIgnoreDisorder()) continue;

        double x = node->x();
        double y = node->y();

        if (not (x == 0.  or  x ==  width))
        {
            x += GaussianRandomClass::GetGaussianWithVariance(u);
            x = std::min( std::max(x, epsilon), width-epsilon );
        }
        if (not (y == 0.  or  y == height))
        {
            y += GaussianRandomClass::GetGaussianWithVariance(u);
            y = std::min( std::max(y, epsilon), height-epsilon );
        }
        node->SetXY(x, y);
    }


}


#define DIST(N1, N2) sqrt( ((N2)->x()-(N1)->x())*((N2)->x()-(N1)->x()) + ((N2)->y()-(N1)->y())*((N2)->y()-(N1)->y()) )
void
MosaicGraph::SaveAsTextFile (const char * filename)
{
    double  width = MosaicConstants::GetPmfWidth();
    double height = MosaicConstants::GetPmfHeight();

    std::vector<int> nids(nodes.size(), -1);
    ///fill_n(nids.begin(), nodes.size(), -1);

    int counter = 0;
    FOREACH(it, nodes)
    {
        MosaicGraphNode * node = *it;
        assert(node->Size() <= 3);
        if (node->x() == 0.  or  node->x() == width  or  node->y() == 0.  or  node->y() == height)
        {
            if (node->Size() < 3)  continue;
            assert(node->Size() == 3);
        }
        else
            assert(node->Size() == 2);

        nids[node->GetId()] = (++counter);
    }
    ///FOREACH(it, nids) std::cout << " " << *it;  std::cout << std::endl;

    std::ofstream fout(filename);
    fout << width << " " << height << std::endl;
    fout << counter << std::endl;

    int precision = 17;
    fout.precision(precision);

    FOREACH(it, nodes)
    {
        MosaicGraphNode * node = *it;
        int n1 = 0;
        int n2 = 0;
        double l1 = 0.;
        double l2 = 0.;

        if (nids[node->GetId()] < 0)  continue;
        if (node->x() == 0.  or  node->x() == width  or  node->y() == 0.  or  node->y() == height)
        {
            assert(node->Size() == 3);
            FOREACH(nit, *node)
                if (not (nodes[(*nit)->GetId()]->x() == 0.  or  nodes[(*nit)->GetId()]->x() == width  or  nodes[(*nit)->GetId()]->y() == 0.  or  nodes[(*nit)->GetId()]->y() == height))
                {
                    assert(n1 == 0);
                    n1 = nids[(*nit)->GetId()];
                    l1 = DIST(node, nodes[(*nit)->GetId()]);
                }
        }
        else
        {
            assert(node->Size() == 2);
            n1 = nids[node->Front()->GetId()];
            l1 = DIST(node, nodes[node->Front()->GetId()]) + 1.;
            n2 = nids[node->Back()->GetId()];
            l2 = DIST(node, nodes[node->Back()->GetId()]) + 1.;
        }

        fout.width(4);  fout << nids[node->GetId()] << " ";
        fout.width(precision+5);  fout << node->x() << " ";
        fout.width(precision+5);  fout << node->y() << " ";
        fout.width(4);                   fout << n1 << " ";
        fout.width(4);                   fout << n2 << " ";
        fout.width(precision+5);         fout << l1 << " ";
        fout.width(precision+5);         fout << l2 << " ";
        fout.width(2);                    fout << 0 << " ";
        fout.width(3);                 fout << "-1" << " ";
        fout.width(3);                    fout << 0 << " ";
        fout << std::endl;
    }
    fout.close();
}
#undef DIST



#include <wx/wx.h>
#include <wx/log.h>
#include <wx/dcmemory.h>
#include <wx/wfstream.h>
#include <wx/zipstrm.h>
#include <wx/txtstrm.h>


void
MosaicGraph::SaveAsGeoGebraFile (const char * filename)
{
        wxCSConv conv_ascii(_T("ISO-8859-1"));
        wxString outputfile( filename, conv_ascii );
        //wxFFileOutputStream fout(wxString::Format(_T("%s"), filename));
        wxFFileOutputStream fout(outputfile);
        wxZipOutputStream zip(fout);
        wxTextOutputStream txt(zip);
        wxString sep(wxFileName::GetPathSeparator());
        /*
        zip.PutNextEntry(_T("entry1.txt"));
        txt << _T("Some text for entry1.txt\n");

        zip.PutNextEntry(_T("subdir") + sep + _T("entry2.txt"));
        txt << _T("Some text for subdir/entry2.txt\n");
        //*/
        zip.PutNextEntry(_T("geogebra.xml"));
        //*
        txt << _("<?xml version='1.0' encoding='utf-8'?>") << endl;
        txt << _("<geogebra format='3.2'>") << endl;
        txt << _("    <gui>") << endl;
		txt << _("        <show algebraView='true' spreadsheetView='false' auxiliaryObjects='true' algebraInput='true' cmdList='true'/>") << endl;
		txt << _("        <splitDivider loc='822' locVertical='400' loc2='221' locVertical2='300' horizontal='true'/>") << endl;
		txt << _("        <labelingStyle  val='3'/>") << endl;
		txt << _("        <font  size='12'/>") << endl;
        txt << _("    </gui>") << endl;
        txt << _("    <euclidianView>") << endl;
		txt << _("        <size  width='691' height='707'/>") << endl;
		txt << _("        <coordSystem xZero='49.20558651917091' yZero='641.537966311083' scale='597.2884022128751' yscale='597.2884022128751'/>") << endl;
		txt << _("        <evSettings axes='true' grid='true' gridIsBold='false' pointCapturing='0' pointStyle='0' rightAngleStyle='1' checkboxSize='13' gridType='0'/>") << endl;
		txt << _("        <bgColor r='255' g='255' b='255'/>") << endl;
		txt << _("        <axesColor r='0' g='0' b='0'/>") << endl;
		txt << _("        <gridColor r='192' g='192' b='192'/>") << endl;
		txt << _("        <lineStyle axes='1' grid='10'/>") << endl;
		txt << _("        <axis id='0' show='false' label='x' unitLabel='' tickStyle='0' showNumbers='true'/>") << endl;
		txt << _("        <axis id='1' show='false' label='y' unitLabel='' tickStyle='0' showNumbers='true'/>") << endl;
        txt << _("    </euclidianView>") << endl;
        txt << _("    <kernel>") << endl;
        txt << _("        <continuous val='true'/>") << endl;
		txt << _("        <decimals val='10'/>") << endl;
		txt << _("        <angleUnit val='degree'/>") << endl;
		txt << _("        <coordStyle val='0'/>") << endl;
        txt << _("    </kernel>") << endl;
        txt << _("    <construction title='' author='Rafal Kluszczynski' date=''>") << endl;

        FOREACH(it, nodes)
        {
            MosaicGraphNode * node = *it;

            txt << _("<element type='point' label='P") << wxString::Format(_("%li"), node->GetId()) << _("'>") << endl;
			txt << _("    <show object='true' label='true'/>") << endl;
			txt << _("    <objColor r='0' g='0' b='255' alpha='0.0'/>") << endl;
			txt << _("    <layer val='0'/>") << endl;
			txt << _("    <labelMode val='0'/>") << endl;
			txt << _("    <animation step='0.1' speed='1' type='0' playing='false'/>") << endl;
			txt << _("    <coords x='") << wxString::Format(_("%.20lf"), node->x()) << _("' y='") << wxString::Format(_("%.20lf"), node->y()) << _("' z='1.0'/>") << endl;
			txt << _("    <pointSize val='3'/>") << endl;
            txt << _("</element>") << endl;
        }
        FOREACH(it, nodes)
        {
            MosaicGraphNode * node = *it;

            FOREACH(iit, *node)
                if (nodes[(*iit)->GetId()]->GetId() < node->GetId())
                {
                    int i = (*iit)->GetId();
                    txt << _("<command name='Segment'>") << endl;
                    txt << _("    <input a0='P") << wxString::Format(_("%li"), nodes[i]->GetId()) << _("' a1='P") << wxString::Format(_("%li"), node->GetId()) << _("'/>") << endl;
                    txt << _("    <output a0='p") << wxString::Format(_("%li"), nodes[i]->GetId()) << _("p") << wxString::Format(_("%li"), node->GetId()) << _("'/>") << endl;
                    txt << _("</command>") << endl;
                } // */
        }

        txt << _("    </construction>") << endl;
        txt << _("</geogebra>") << endl;

        zip.CloseEntry();
        zip.SetComment(_("PMF Configuration"));
        // */
}





/** @brief (one liner)
  *
  * (documentation goes here)
  */
void
MosaicGraph::SortNeighborsInCounterClockwiseOrder ()
{
    FOREACH(it, nodes)
        (*it)->SortNodesNeighborsInCounterClockwiseOrder (nodes);
}


/** @brief (one liner)
  *
  * (documentation goes here)
  */
unsigned int
MosaicGraph::CreateNewNode (double x, double y)
{
    unsigned int id = nodes.size();
    MosaicGraphNode * node = new MosaicGraphNode(x, y, id);
    nodes.push_back(node);
    return id;
}


/** @brief (one liner)
  *
  * (documentation goes here)
  */
void
MosaicGraph::AddEdge (unsigned int n1, unsigned int n2, int deg)
{
    assert(0 <= n1  and  n1 < nodes.size());
    assert(0 <= n2  and  n2 < nodes.size());
    assert(n1 != n2);

    MosaicGraphEdge * edge1 = new MosaicGraphEdge(n1, deg);
    MosaicGraphEdge * edge2 = new MosaicGraphEdge(n2, deg);

    MosaicGraphNode::Iterator it1 = nodes[n1]->AddNeighbor(edge2);
    if (n1 != n2)
    {
        MosaicGraphNode::Iterator it2 = nodes[n2]->AddNeighbor(edge1);
        edge1->SetOther(it1);
        edge2->SetOther(it2);
    }
    else assert(false);
}


void
MosaicGraph::RemoveEdge (unsigned int n1, MosaicGraphNode::Iterator & it, unsigned int n2)
{
    MosaicGraphNode::Iterator jt = (*it)->GetOther();

    nodes[n1]->EraseNeighbor(it);
    nodes[n2]->EraseNeighbor(jt);
}


void
MosaicGraph::RemoveNeighborsOf (unsigned int id, std::vector<int> & neighbours)
{
    if (!neighbours.size()) return;

    std::vector<std::pair<MosaicGraphNode::Iterator, unsigned int> > toerase;
    for (MosaicGraphNode::Iterator it = nodes[id]->Begin(); it != nodes[id]->End(); ++it)
    {
        int anum = (*it)->GetId();
        if (binary_search(neighbours.begin(), neighbours.end(), anum))
        {
            toerase.push_back(std::make_pair(it, anum));
        }
    }
    FOREACH(it, toerase)  this->RemoveEdge(id, it->first, it->second);
}


#define L2(_PX, _PY, _QX, _QY) sqrt((_PX-_QX)*(_PX-_QX)+(_PY-_QY)*(_PY-_QY))
bool AreCollinear(double xp, double yp, double xq, double yq, double xr, double yr)
{
    static double Epsilon = MosaicConstants::GetEpsilon();

    double D = (xq - xp)*(yr - yp) - (yq - yp)*(xr - xp);
    //double M = abs(xq - xp)*abs(yr - yp) + abs(yq - yp)*abs(xr - xp);
    double b = std::max( std::max( L2(xp, yp, xq, yq), L2(xq, yq, xr, yr) ), L2(xr, yr, xp, yp) );

    double h = fabs(0.5 * D) / b;
    //double dh = 7. * epsilon * (M / b);
    /*
    std::cout.precision(25);
    std::cout << " D = " << h << std::endl;
    std::cout << " b = " << h << std::endl;
    std::cout << " h = " << h << std::endl;
    // */
    return h < Epsilon;
}
#undef L2

void
MosaicGraph::RemoveUnnecessaryCollinearNodes (bool printDebug)
{
    double Epsilon = MosaicConstants::GetEpsilon();
    std::vector<int> newids(nodes.size());
    fill_n(newids.begin(), newids.size(), -1);

    if (printDebug)  std::cout << " Epsilon = " << Epsilon << std::endl;
    FOREACH(it, nodes)
    {
        MosaicGraphNode * node = *it;

        if (node->Size() == 2)
        {
            if (printDebug)  std::cout << node->GetId() << std::endl;

            unsigned int n1 = node->Front()->GetId();
            unsigned int n2 = node->Back()->GetId();
            MosaicGraphNode::Iterator it1 = node->Begin();
            MosaicGraphNode::Iterator it2 = --node->End();
            int d1 = node->Front()->GetDegree();
            int d2 = node->Back()->GetDegree();

            double x0 = node->x();
            double y0 = node->y();
            double x1 = this->Get( n1 )->x();
            double y1 = this->Get( n1 )->y();
            double x2 = this->Get( n2 )->x();
            double y2 = this->Get( n2 )->y();

            bool coolinear = AreCollinear(x0, y0, x1, y1, x2, y2);

            double dx01 = x1 - x0;
            double dy01 = y1 - y0;
            double dx02 = x2 - x0;
            double dy02 = y2 - y0;

            double val = (dx01 * dy02 - dx02 * dy01);
#define SIGN(X) (((X) < 0) ? -1 : (((X) > 0) ? 1 : 0))
            //if (SIGN(dx01) != SIGN(dx02)  and  SIGN(dy01) != SIGN(dy02))  coolinear = false;
            ;
#undef SIGN
            //if (-Epsilon < val  and  val < Epsilon)
            if (coolinear)
            {
            if (printDebug)
            {
                std::cout << "( " << node->GetId() << " - " << n1 << " ) :  " << dx01 << "  " << dy01 << std::endl;
                std::cout << "( " << node->GetId() << " - " << n2 << " ) :  " << dx02 << "  " << dy02 << std::endl;
            }
                if (printDebug)  std::cout << node->GetId() << " " << n1 << " " << n2 << "   : " << val << std::endl;

                assert(d1 == d2);
                this->RemoveEdge(node->GetId(), it1, n1);
                this->RemoveEdge(node->GetId(), it2, n2);
                this->AddEdge(n1, n2, d1);
            }
        }
    }
    //std::cout << *this << std::endl;

    unsigned int freeplace = 0;
    unsigned int i = 0;
    FOREACH(it, nodes)
    {
        if ((* it)->Size() != 0)
        {
            if (printDebug)  std::cout << " moving " << i << " to " << freeplace << std::endl;

            nodes[freeplace] = nodes[i];
            newids[i] = freeplace;
            ++freeplace;
        }
        else {
            if (printDebug)  std::cout << " erasing " << i << std::endl;
        }
        ++i;
    }
    while (nodes.size() > freeplace) nodes.pop_back();

    //for(int j = 0; j < newids.size(); ++j)  std::cout << " " << j << "=" << newids[j];  std::cout << std::endl;

    FOREACH(it, nodes)
    {
        (*it)->SetId( newids[(*it)->GetId()] );
        FOREACH(jt, **it)
            (*jt)->SetId( newids[(*jt)->GetId()] );
    }
}


void
MosaicGraph::MutateIntersectionElements ()
{
    double epsilon = MosaicConstants::GetEpsilon();
    double  offset = 2. * epsilon;
    std::vector<std::pair<int, int> > edgestoadd;
    FOREACH(it, nodes)
    {
        MosaicGraphNode * node = *it;
        if (node->Size() > 3)
        {
            std::cout << node->GetId() << std::endl;

            double x0 = node->x();
            double y0 = node->y();

            std::vector<MosaicGraphEdge *> nlist;
            std::vector<double> nxx;
            std::vector<double> nyy;
            std::vector<int> nids;
            FOREACH(nit, *node)
            {
                nlist.push_back(*nit);

                double xx = nodes[ (* nit)->GetId() ]->x();
                double yy = nodes[ (* nit)->GetId() ]->y();

                std::cout << "P" << node->GetId() << "x" << (* nit)->GetId();// << "   :  ";

                double dx = x0 - xx;
                double dy = y0 - yy;
                double dd = sqrt(dx*dx + dy*dy);
                double d = dd - offset;
                assert(d > 0.);
                double scale = d / dd;

                xx += scale * dx;
                yy += scale * dy;

                nxx.push_back(xx);
                nyy.push_back(yy);
                nids.push_back((*nit)->GetId());

                std::cout.precision(15);
                std::cout << "=(" << xx << "," << yy << ")" << std::endl;
            }
            //*
            //for (MosaicGraphNode::Iterator nit = node->Begin(); nit != node->End(); ++nit)
            while (node->Size() > 0)
            {
                MosaicGraphNode::Iterator nit = node->Begin();
                RemoveEdge(node->GetId(), nit, (* node->Begin())->GetId());
            }
            // */
            /*
            double dist01 = (nodes[nlist[0]->GetId()]->x() - nodes[nlist[1]->GetId()]->x()) * (nodes[nlist[0]->GetId()]->x() - nodes[nlist[1]->GetId()]->x())
                            + (nodes[nlist[0]->GetId()]->y() - nodes[nlist[1]->GetId()]->y()) * (nodes[nlist[0]->GetId()]->y() - nodes[nlist[1]->GetId()]->y());
            double dist21 = (nodes[nlist[2]->GetId()]->x() - nodes[nlist[1]->GetId()]->x()) * (nodes[nlist[2]->GetId()]->x() - nodes[nlist[1]->GetId()]->x())
                            + (nodes[nlist[2]->GetId()]->y() - nodes[nlist[1]->GetId()]->y()) * (nodes[nlist[2]->GetId()]->y() - nodes[nlist[1]->GetId()]->y());
            // */
            double dist01 = (nxx[0] - nxx[1]) * (nxx[0] - nxx[1])  +  (nyy[0] - nyy[1]) * (nyy[0] - nyy[1]);
            double dist21 = (nxx[2] - nxx[1]) * (nxx[2] - nxx[1])  +  (nyy[2] - nyy[1]) * (nyy[2] - nyy[1]);

            std::cout << "dist01 = " << dist01 << std::endl;
            std::cout << "dist21 = " << dist21 << std::endl;

            if (dist01 < dist21)
            {
                std::cout << " link1 : " << nids[0] << " ~ " << nids[1] << std::endl;

                unsigned int nid01 = CreateNewNode(0.5 * (nxx[0] + nxx[1]), 0.5 * (nyy[0] + nyy[1]));
                Get(nid01)->SetIgnoreDisorder(true);
                AddEdge(nid01, nids[0], 2);
                AddEdge(nid01, nids[1], 2);
                //edgestoadd.push_back(std::make_pair(nid01, nids[0]));
                //edgestoadd.push_back(std::make_pair(nid01, nids[1]));

                unsigned int nid23 = CreateNewNode(0.5 * (nxx[2] + nxx[3]), 0.5 * (nyy[2] + nyy[3]));
                Get(nid23)->SetIgnoreDisorder(true);
                AddEdge(nid23, nids[2], 2);
                AddEdge(nid23, nids[3], 2);
                //edgestoadd.push_back(std::make_pair(nid23, nids[2]));
                //edgestoadd.push_back(std::make_pair(nid23, nids[3]));
            }
            else
            {
                std::cout << " link2 : " << nids[1] << " ~ " << nids[2] << std::endl;

                unsigned int nid21 = CreateNewNode(0.5 * (nxx[2] + nxx[1]), 0.5 * (nyy[2] + nyy[1]));
                Get(nid21)->SetIgnoreDisorder(true);
                AddEdge(nid21, nids[2], 2);
                AddEdge(nid21, nids[1], 2);
                //edgestoadd.push_back(std::make_pair(nid21, nids[2]));
                //edgestoadd.push_back(std::make_pair(nid21, nids[1]));

                unsigned int nid03 = CreateNewNode(0.5 * (nxx[0] + nxx[3]), 0.5 * (nyy[0] + nyy[3]));
                Get(nid03)->SetIgnoreDisorder(true);
                AddEdge(nid03, nids[0], 2);
                AddEdge(nid03, nids[3], 2);
                //edgestoadd.push_back(std::make_pair(nid03, nids[0]));
                //edgestoadd.push_back(std::make_pair(nid03, nids[3]));
            }

            /* 4 points instead 1
            unsigned int nid0 = CreateNewNode(nxx[0], nyy[0]);
            unsigned int nid1 = CreateNewNode(nxx[1], nyy[1]);
            unsigned int nid2 = CreateNewNode(nxx[2], nyy[2]);
            unsigned int nid3 = CreateNewNode(nxx[3], nyy[3]);
            edgestoadd.push_back(std::make_pair(nid0, nids[0]));
            edgestoadd.push_back(std::make_pair(nid1, nids[1]));
            edgestoadd.push_back(std::make_pair(nid2, nids[2]));
            edgestoadd.push_back(std::make_pair(nid3, nids[3]));
            if (dist01 < dist21)
            {
                std::cout << " link1 : " << nids[0] << " ~ " << nids[1] << std::endl;
                edgestoadd.push_back(std::make_pair(nid0, nid1));
                edgestoadd.push_back(std::make_pair(nid2, nid3));
            }
            else
            {
                std::cout << " link2 : " << nids[1] << " ~ " << nids[2] << std::endl;
                edgestoadd.push_back(std::make_pair(nid2, nid1));
                edgestoadd.push_back(std::make_pair(nid0, nid3));
            }
            // */
        }
    }
    //FOREACH(it, edgestoadd)  AddEdge(it->first, it->second, 2);
}


std::ostream & operator << (std::ostream & out, const MosaicGraph & graph)
{
    out << "[ MosaicGraph ]  :  nodes = " << graph.nodes.size() << std::endl;
    int i = 0;
    FOREACH(it, graph.nodes) out << i++ << " : " << *it;
    out << std::endl;
    return out;
}


MosaicGraph::MosaicGraph(const MosaicGraph & other)
{
    //copy ctor
    FOREACH(it, other.nodes)
        nodes.push_back(new MosaicGraphNode((*it)->x(), (*it)->y(), (*it)->GetId()));
    FOREACH(it, other.nodes)
        FOREACH(jt, **it)
        {
            unsigned int nodeid = (*it)->GetId();
            unsigned int edgeid = (*jt)->GetId();

            if (nodeid < edgeid)
                this->AddEdge(nodeid, edgeid, (*jt)->GetDegree());
        }
    this->SortNeighborsInCounterClockwiseOrder();
}

