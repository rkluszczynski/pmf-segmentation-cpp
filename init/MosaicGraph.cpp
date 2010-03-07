#include "MosaicGraph.hpp"


MosaicGraph::MosaicGraph()
{
    //ctor
}


MosaicGraph::~MosaicGraph()
{
    //dtor
}


#include <wx/wx.h>
#include <wx/log.h>
#include <wx/dcmemory.h>
#include <wx/wfstream.h>
#include <wx/zipstrm.h>
#include <wx/txtstrm.h>


void
MosaicGraph::SaveAsGeoGebraFile (char * filename)
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
		txt << _("        <axis id='0' show='true' label='x' unitLabel='' tickStyle='0' showNumbers='true'/>") << endl;
		txt << _("        <axis id='1' show='true' label='y' unitLabel='' tickStyle='0' showNumbers='true'/>") << endl;
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


void
MosaicGraph::RemoveUnnecessaryCollinearNodes ()
{
    std::vector<int> newids(nodes.size());
    fill_n(newids.begin(), newids.size(), -1);

    FOREACH(it, nodes)
    {
        MosaicGraphNode * node = *it;

        if (node->Size() == 2)
        {
            std::cout << node->GetId() << std::endl;

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

            double dx01 = x1 - x0;
            double dy01 = y1 - y0;
            double dx02 = x2 - x0;
            double dy02 = y2 - y0;

            double val = dx01 * dy02 - dx02 * dy01;
            if (-1e-8 < val  and  val < 1e-8)
            {
                std::cout << node->GetId() << " " << n1 << " " << n2 << std::endl;
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
            std::cout << " moving " << i << " to " << freeplace << std::endl;
            nodes[freeplace] = nodes[i];
            newids[i] = freeplace;
            ++freeplace;
        }
        else {
            std::cout << " erasing " << i << std::endl;
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
                double d = dd - 1e-7;
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
                AddEdge(nid01, nids[0], 2);
                AddEdge(nid01, nids[1], 2);
                //edgestoadd.push_back(std::make_pair(nid01, nids[0]));
                //edgestoadd.push_back(std::make_pair(nid01, nids[1]));

                unsigned int nid23 = CreateNewNode(0.5 * (nxx[2] + nxx[3]), 0.5 * (nyy[2] + nyy[3]));
                AddEdge(nid23, nids[2], 2);
                AddEdge(nid23, nids[3], 2);
                //edgestoadd.push_back(std::make_pair(nid23, nids[2]));
                //edgestoadd.push_back(std::make_pair(nid23, nids[3]));
            }
            else
            {
                std::cout << " link2 : " << nids[1] << " ~ " << nids[2] << std::endl;

                unsigned int nid21 = CreateNewNode(0.5 * (nxx[2] + nxx[1]), 0.5 * (nyy[2] + nyy[1]));
                AddEdge(nid21, nids[2], 2);
                AddEdge(nid21, nids[1], 2);
                //edgestoadd.push_back(std::make_pair(nid21, nids[2]));
                //edgestoadd.push_back(std::make_pair(nid21, nids[1]));

                unsigned int nid03 = CreateNewNode(0.5 * (nxx[0] + nxx[3]), 0.5 * (nyy[0] + nyy[3]));
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

