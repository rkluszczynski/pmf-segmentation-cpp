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
            MosaicGraphNode * pt = *it;

            txt << _("<element type='point' label='P") << wxString::Format(_("%li"), pt->GetId()) << _("'>") << endl;
			txt << _("    <show object='true' label='true'/>") << endl;
			txt << _("    <objColor r='0' g='0' b='255' alpha='0.0'/>") << endl;
			txt << _("    <layer val='0'/>") << endl;
			txt << _("    <labelMode val='0'/>") << endl;
			txt << _("    <animation step='0.1' speed='1' type='0' playing='false'/>") << endl;
			txt << _("    <coords x='") << wxString::Format(_("%.20lf"), pt->x()) << _("' y='") << wxString::Format(_("%.20lf"), pt->y()) << _("' z='1.0'/>") << endl;
			txt << _("    <pointSize val='3'/>") << endl;
            txt << _("</element>") << endl;
        }

        FOREACH(it, nodes)
        {
            MosaicGraphNode * pt = *it;

            FOREACH(iit, pt->n)
            {
                int i = (*iit).first;
                txt << _("<command name='Segment'>") << endl;
                txt << _("    <input a0='P") << wxString::Format(_("%li"), nodes[i]->GetId()) << _("' a1='P") << wxString::Format(_("%li"), pt->GetId()) << _("'/>") << endl;
                txt << _("    <output a0='p") << wxString::Format(_("%li"), nodes[i]->GetId()) << _("p") << wxString::Format(_("%li"), pt->GetId()) << _("'/>") << endl;
                txt << _("</command>") << endl;
            }
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
        (*it)->SortNeighboursInCounterClockwiseOrder (nodes);
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
    nodes[n1]->n.push_back(std::make_pair(n2, deg));
    if (n1 != n2)
        nodes[n2]->n.push_back(std::make_pair(n1, deg));
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
        nodes.push_back (new MosaicGraphNode(*(*it)));
}
