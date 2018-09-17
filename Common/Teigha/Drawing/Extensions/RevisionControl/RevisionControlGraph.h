/////////////////////////////////////////////////////////////////////////////// 
// Copyright (C) 2002-2018, Open Design Alliance (the "Alliance"). 
// All rights reserved. 
// 
// This software and its documentation and related materials are owned by 
// the Alliance. The software may only be incorporated into application 
// programs owned by members of the Alliance, subject to a signed 
// Membership Agreement and Supplemental Software License Agreement with the
// Alliance. The structure and organization of this software are the valuable  
// trade secrets of the Alliance and its suppliers. The software is also 
// protected by copyright law and international treaty provisions. Application  
// programs incorporating this software must include the following statement 
// with their copyright notices:
//   
//   This application incorporates Teigha(R) software pursuant to a license 
//   agreement with Open Design Alliance.
//   Teigha(R) Copyright (C) 2002-2018 by Open Design Alliance. 
//   All rights reserved.
//
// By use of this software, its documentation or related materials, you 
// acknowledge and accept the above terms.
///////////////////////////////////////////////////////////////////////////////

#ifndef REVISION_CONTROL_GRAPH_
#define REVISION_CONTROL_GRAPH_

#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <vector>

/** \details
  <group TD_Namespaces>
*/
namespace revision_control_graph {
  struct GraphColor {
    int red;
    int green;
    int blue;

    GraphColor();

    GraphColor(int r, int g, int b);
  };
  // (sha is ansi string) so using std::string is ok
  struct GraphNode {
    float x;
    float y;
    GraphColor color;
    std::string              id;
    std::vector<std::string> parents;

    GraphNode();

    GraphNode(std::string id, std::vector<std::string> parents);
  };

  struct ViewParameters {
    float spacing_x;
    float spacing_y;
    float commit_size;
    float branch_width;
    float top_margin;
    float left_margin;

    ViewParameters();

    ViewParameters(
      float spacing_x,
      float spacing_y,
      float commit_size, // size of circle in node
      float branch_width,
      float top_margin,
      float left_margin);
  };

  /** \details
   Interface to Draw Revision Tree
   Library: Source provided.
   <group RevisionControl_Classes>
 */
  struct IRevisionPainter {
    /** \details
    This interface member will be called when RevisionPointer finds Node,
    it supposed to draw revision node
    \param node [in] node drawing data
    \param view_params [in] canvas parameters
    \note
    */
    virtual void drawNode(GraphNode &node, ViewParameters &view_params) = 0;

    /** \details
    This interface member will be called when edge bettwen two nodes will be found,
    it supposed to draw edge between two nodes
    \param start [in] start node of edge
    \param end [in] end node of edge
    \param view_params [in] canvas parameters
    \note
    */
    virtual void drawConnectionOfNodes(GraphNode &start, GraphNode &end, ViewParameters& view_params) = 0;
  };

  class RevisionView {
  public:
    // set parameters for future position calculation node

    /** \details
    Set parameters for node position calculation
    \param view_params [in] canvas parameters
    \note
    */
    void setParameters(ViewParameters params);

    /** \details
    Get parameters for node position calculation
    \note
    */
    ViewParameters getParameters() const;

    /** \details
    Build graph of revisions
    \param list [in] list of all nodes in Graph
    \note
    */
    void parsing(std::vector<GraphNode> &list);

    /** \details
    Trace graph to draw graph on canvas
    \param painter [in] interface with implementaed functions to draw
    \note
    */
    void draw(IRevisionPainter *painter);

    /** \details
    This API set a set of colors for branches
    \param branch_colors [in] array of colors for branches in RGB
    \note
    */
    void setBranchColors(std::vector<GraphColor> branch_colors);

    /** \details
    This API get a certain color for certain branch
    \param index [in] index of branch
    \return Certain color in RGB for some branch
    \note
    */
    GraphColor getBranchColor(unsigned int index);

    /** \details
     Get width of graph based on ViewParameters
    \return width of graph
    \note
    */
    float getWidth();

    /** \details
     Get width of graph based on ViewParameters
    \return width of graph
    \note
    */
    float getHeight();

  protected:
    bool getNodeById(std::string &id, GraphNode& node);

    void clearModel();

    void addToModel(GraphNode &node);

    inline void calculateNewShift(GraphNode &node);

    inline void calculateVerticalPosition(GraphNode &node);

    inline void calculateNodeColor(GraphNode &node);

    bool calculateHorizontalPosition(GraphNode &node);

  private:
    long                             index_;
    long                             columns_;
    ViewParameters                   parameters_;
    std::vector<GraphNode>           model_;
    std::map<std::string, GraphNode> model_id_maping_;
    std::vector<GraphColor>          branch_colors_;
  };
}

#endif //
