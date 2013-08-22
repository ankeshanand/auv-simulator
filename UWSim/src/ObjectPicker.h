/* 
 * Copyright (c) 2013 University of Jaume-I.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0
 * which accompanies this distribution, and is available at
 * http://www.gnu.org/licenses/gpl.html
 * 
 * Contributors:
 *     Mario Prats
 *     Javier Perez
 */

#ifndef OBJECTPICKER_H_
#define OBJECTPICKER_H_

#include "VirtualRangeSensor.h"
#include "UWSimUtils.h"

//Node tracker that updates the ray coordinates from the tracked node position, computes intersections and 'picks' nodes
class ObjectPickerUpdateCallback : public IntersectorUpdateCallback {
  virtual void operator() (osg::Node *node, osg::NodeVisitor *nv)  {
	osg::Matrixd mStart, mEnd;
	mStart = osg::computeLocalToWorld(nv->getNodePath() );
	traverse(node,nv);

	//update ray and compute intersections. Checks intersections along X axis of the local frame
	mEnd=mStart;
	mEnd.preMultTranslate(osg::Vec3d(range,0,0));

	intersector->reset();
	intersector->setStart(mStart.getTrans());
	intersector->setEnd(mEnd.getTrans());

	root->accept(intersectVisitor);

	if ( intersector->containsIntersections() && !picked )
	{
	    osgUtil::LineSegmentIntersector::Intersection intersection=intersector->getFirstIntersection();
	    osg::Vec3d worldIntPoint=intersection.getWorldIntersectPoint();
	    distance_to_obstacle=(worldIntPoint-mStart.getTrans()).length();
	    impact=intersection.nodePath;

	    //search for catchable objects in nodepath
	    for(osg::NodePath::iterator i=impact.begin();i!=impact.end();++i){
	      osg::ref_ptr<NodeDataType> data = dynamic_cast<NodeDataType*> (i[0]->getUserData());
	      if(data!=NULL && data->catchable){

	        std::cerr << "Picking object up." << std::endl;
	  
		//Get coordinates to change them when changing position in graph
		osg::Matrixd *originalpos=getWorldCoords(i[0]);
		osg::Matrixd *hand = getWorldCoords(trackNode);
	    	hand->invert(*hand);

	    	//ADD node in hand, remove object from original position.
	    	trackNode->asTransform()->addChild(i[0]);
	    	i[0]->getParent(0)->asGroup()->removeChild(i[0]);

	    	osg::Matrixd matrix=*originalpos * *hand;
	    	i[0]->asTransform()->asMatrixTransform()->setMatrix(matrix);

	    	picked=true;
	      }
	    }
	} else if (!picked) distance_to_obstacle=range;
  }
  public:
	osg::NodePath impact;
	osg::Node *trackNode;
	bool picked;

	ObjectPickerUpdateCallback(osg::Node *trackNode, double range, bool visible, osg::Node *root) : IntersectorUpdateCallback (range, visible, root) { this->trackNode=trackNode; picked=false;}
};

class ObjectPicker : public VirtualRangeSensor{
  public:
    ObjectPicker(std::string name, osg::Node *root, osg::Node *trackNode, double range, bool visible);
    ObjectPicker();

    void init(std::string name,  osg::Node *root, osg::Node *trackNode, double range, bool visible);
};

#endif
