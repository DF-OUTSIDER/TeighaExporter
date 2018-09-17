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


#include "OdaCommon.h"
#include "RevisionControlGraph.h"

namespace revision_control_graph {
  typedef std::vector<GraphNode>::iterator   GraphNodeListIterator;
  typedef std::vector<std::string>::iterator RevisionIdIterator;

  GraphColor::GraphColor()
    : red(0), green(0), blue(0) {}

  GraphColor::GraphColor(int r, int g, int b)
    : red(r), green(g), blue(b) {}

  GraphNode::GraphNode()
    : x(0.0f), y(0.0f) {}

  GraphNode::GraphNode(std::string id, std::vector<std::string> parents)
    : x(0.0f), y(0.0f), id(id), parents(parents) {}

  ViewParameters::ViewParameters() : spacing_x(20.0f), spacing_y(39.0f), commit_size(8),
                                     branch_width(2), top_margin(58), left_margin(20) {
  }

  ViewParameters::ViewParameters(float spacing_x,    float spacing_y,  float commit_size,
                                 float branch_width, float top_margin, float left_margin)
    : spacing_x(spacing_x), spacing_y(spacing_y), commit_size(commit_size),
      branch_width(branch_width), top_margin(top_margin), left_margin(left_margin) { }


  void RevisionView::setParameters(ViewParameters params) {
    parameters_ = params;
  }

  ViewParameters RevisionView::getParameters() const {
    return parameters_;
  }

  void RevisionView::parsing(std::vector<GraphNode> &list) {
    GraphNode node_new;
    clearModel();

    for (GraphNodeListIterator itr = list.begin(); itr != list.end(); itr++, index_++) {
      node_new = *itr;
      calculateVerticalPosition(node_new);

      if (! calculateHorizontalPosition(node_new)) {
        calculateNewShift(node_new);
      }

      calculateNodeColor(node_new);
      addToModel(node_new);
    }
  }
  // TODO Optimize algorithm
  bool RevisionView::calculateHorizontalPosition(GraphNode &node) {
    for (GraphNodeListIterator model_itr = model_.begin(); model_itr != model_.end(); ++model_itr) {
      RevisionIdIterator prnt = model_itr->parents.begin();

      if (prnt == model_itr->parents.end())
        continue;

      // there is a node that has this as a first parent - this node is at the same x
      if (*prnt == node.id) {
        node.x = model_itr->x;
        return true;
      }

      // node is a second parent - shift off
      for (; prnt != model_itr->parents.end(); ++prnt) {
        if (*prnt == node.id) {
          calculateNewShift(node);
          return true;
        }
      }
    }
    // node is new top
    return false;
  }

  void RevisionView::calculateNewShift (GraphNode &node) {
    node.x = parameters_.left_margin + parameters_.spacing_x * columns_++;
  }

  void RevisionView::calculateVerticalPosition(GraphNode &node) {
    node.y = index_ * parameters_.spacing_y + parameters_.top_margin;
  }

  void RevisionView::calculateNodeColor(GraphNode &node) {
    node.color = getBranchColor((node.x - parameters_.left_margin) / parameters_.spacing_x);
  }

  void RevisionView::draw(IRevisionPainter *painter) {
    GraphNode parent;

    for (GraphNodeListIterator  itr = model_.begin(); itr != model_.end(); itr++) {
      for (RevisionIdIterator prnt = (*itr).parents.begin(); prnt != (*itr).parents.end(); prnt++) {
        if (getNodeById(*prnt, parent)) {
          painter->drawConnectionOfNodes(*itr, parent, parameters_);
        } 
      }

      painter->drawNode(*itr, parameters_);
    }
  }

  void RevisionView::setBranchColors(std::vector<GraphColor> branch_colors) {
    branch_colors_ = branch_colors;
  }

  bool RevisionView::getNodeById(std::string &id, GraphNode& node) {
    if (model_id_maping_.find(id) != model_id_maping_.end()) {
      node = model_id_maping_[id];
      return true;
    }

    return false;
  }

  void RevisionView::clearModel() {
    index_   = 0;
    columns_ = 0;
    model_.clear();
    model_id_maping_.clear();
  }

  void RevisionView::addToModel(GraphNode &node) {
    model_.push_back(node);
    model_id_maping_[node.id] = node;
  }

  GraphColor RevisionView::getBranchColor(unsigned int index) {
    if (index < branch_colors_.size()) {
      return branch_colors_[index];
    }
    return GraphColor();
  }

  float RevisionView::getWidth() {
    return parameters_.left_margin + parameters_.spacing_x * columns_;
  }

  float RevisionView::getHeight() {
    return model_.size() * parameters_.spacing_y + parameters_.top_margin;
  }

}
