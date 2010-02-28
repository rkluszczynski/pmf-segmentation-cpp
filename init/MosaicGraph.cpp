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
        /*

        }

        FOREACH(it, (*pts))
        {
            Point<REAL> * pt = *it;
            /*
            txt << _("<command name='Segment'>") << endl;
            txt << _("    <input a0='P") << wxString::Format(_("%li"), pt->n2->id) << _("' a1='P") << wxString::Format(_("%li"), pt->id) << _("'/>") << endl;
            txt << _("    <output a0='p") << wxString::Format(_("%li"), pt->n2->id) << _("p") << wxString::Format(_("%li"), pt->id) << _("'/>") << endl;
            txt << _("</command>") << endl;
            txt << _("<command name='Segment'>") << endl;
            txt << _("    <input a0='P") << wxString::Format(_("%li"), pt->n1->id) << _("' a1='P") << wxString::Format(_("%li"), pt->id) << _("'/>") << endl;
            txt << _("    <output a0='p") << wxString::Format(_("%li"), pt->n1->id) << _("p") << wxString::Format(_("%li"), pt->id) << _("'/>") << endl;
            txt << _("</command>") << endl;
            //*/
            /*
            switch (pt->type)
            {
                case     PT_Collision :
                                txt << _("<command name='Segment'>") << endl;
                                txt << _("    <input a0='P") << wxString::Format(_("%li"), pt->n2->id) << _("' a1='P") << wxString::Format(_("%li"), pt->id) << _("'/>") << endl;
                                txt << _("    <output a0='p") << wxString::Format(_("%li"), pt->n2->id) << _("p") << wxString::Format(_("%li"), pt->id) << _("'/>") << endl;
                                txt << _("</command>") << endl;
                case PT_DeathOnBorder :
                case        PT_Update :
                                txt << _("<command name='Segment'>") << endl;
                                txt << _("    <input a0='P") << wxString::Format(_("%li"), pt->n1->id) << _("' a1='P") << wxString::Format(_("%li"), pt->id) << _("'/>") << endl;
                                txt << _("    <output a0='p") << wxString::Format(_("%li"), pt->n1->id) << _("p") << wxString::Format(_("%li"), pt->id) << _("'/>") << endl;
                                txt << _("</command>") << endl;
                                break;;
                default :
                                ;
            }
            //*/
        //}

        txt << _("    </construction>") << endl;
        txt << _("</geogebra>") << endl;

        zip.CloseEntry();
        zip.SetComment(_("PMF Configuration"));
        //*/
}





/** @brief (one liner)
  *
  * (documentation goes here)
  */
void
MosaicGraph::SortNeighboursInCounterClockwiseOrder ()
{
    FOREACH(it, nodes)
        //(*it)->SortNeighboursInCounterClockwiseOrder (nodes);
        (*it)->SortListNeighboursInCounterClockwiseOrder (nodes);
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
    /*
    nodes[n1]->n.push_back(std::make_pair(n2, deg));
    if (n1 != n2)
        nodes[n2]->n.push_back(std::make_pair(n1, deg));
    // */
    AddListEdge(n1, n2, deg);
}

void
MosaicGraph::AddListEdge (unsigned int n1, unsigned int n2, int deg)
{
    assert(0 <= n1  and  n1 < nodes.size());
    assert(0 <= n2  and  n2 < nodes.size());

    MosaicGraphEdge * edge1 = new MosaicGraphEdge(n1, deg);
    MosaicGraphEdge * edge2 = new MosaicGraphEdge(n2, deg);

    MosaicGraphNode::Iterator it1 = nodes[n1]->AddNeighbour(edge2);
    if (n1 != n2)
    {
        MosaicGraphNode::Iterator it2 = nodes[n2]->AddNeighbour(edge1);
        edge1->SetOther(it1);
        edge2->SetOther(it2);
    }
    else assert(false);
}


void
MosaicGraph::RemoveEdge (unsigned int n1, unsigned int n2)
{
    unsigned int j = 0;
    while (j < nodes[n1]->n.size()  and  nodes[n1]->n[j].first != n2) ++j;
    for (unsigned int i = j; i < nodes[n1]->n.size()-1; ++i) nodes[n1]->n[i] = nodes[n1]->n[i+1];
    nodes[n1]->n.pop_back();

    j = 0;
    while (j < nodes[n2]->n.size()  and  nodes[n2]->n[j].first != n1) ++j;
    for (unsigned int i = j; i < nodes[n2]->n.size()-1; ++i) nodes[n2]->n[i] = nodes[n2]->n[i+1];
    nodes[n2]->n.pop_back();
}

void
MosaicGraph::RemoveListEdge (unsigned int n1, MosaicGraphNode::Iterator & it, unsigned int n2)
{
    MosaicGraphNode::Iterator jt = (*it)->GetOther();

    nodes[n1]->EraseNeighbour(it);
    nodes[n2]->EraseNeighbour(jt);
}


void
MosaicGraph::RemoveNeighboursOf (unsigned int id, std::vector<int> & neighbours)
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
    FOREACH(it, toerase)  this->RemoveListEdge(id, it->first, it->second);
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
                this->RemoveListEdge(node->GetId(), it1, n1);
                this->RemoveListEdge(node->GetId(), it2, n2);
                this->AddListEdge(n1, n2, d1);
            }
        }
    }

    //std::cout << *this << std::endl;

    int freeplace = 0;
    int i = 0;
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
                this->AddListEdge(nodeid, edgeid, (*jt)->GetDegree());
        }
    this->SortNeighboursInCounterClockwiseOrder();
}

