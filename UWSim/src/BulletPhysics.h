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

#ifndef BULLETPHYSICS_H_
#define BULLETPHYSICS_H_

#include "SimulatorConfig.h"
#include "UWSimUtils.h"


#include <osgbDynamics/MotionState.h>
#include <osgbCollision/CollisionShapes.h>
#include <osgbCollision/Utils.h>

#include <btBulletDynamicsCommon.h>
#include <iostream>

#include <osgOcean/OceanScene>

#include "BulletHfFluid/btHfFluidRigidDynamicsWorld.h"
#include "BulletHfFluid/btHfFluidRigidCollisionConfiguration.h"
#include "BulletHfFluid/btHfFluid.h"
#include "BulletHfFluid/btHfFluidBuoyantConvexShape.h"


//#include <osgbCollision/GLDebugDrawer.h>



#define UWSIM_DEFAULT_GRAVITY	btVector3(0,0,-1)

// Define filter groups
enum CollisionTypes {
    COL_NOTHING = 0x00000000,
    COL_OBJECTS = 0x00000001,
    COL_VEHICLE = 0x00000010,
    COL_EVERYTHING = 0x11111111,
};



/*class NodeDataType : public osg::Referenced{
    public:
       NodeDataType(btRigidBody * rigidBody,int catcha){ catchable=catcha; rb=rigidBody;}; 
       int catchable;
       btRigidBody * rb;
       
};*/

class CollisionDataType : public osg::Referenced{
    public:
       CollisionDataType(std::string nam,std::string vehName,int isVehi){vehicleName=vehName;name=nam;isVehicle=isVehi;};
       std::string getObjectName(){if(isVehicle) return vehicleName; else return name;};
       std::string name, vehicleName;
       int isVehicle;
       
};

class BulletPhysics: public osg::Referenced {

public:
	typedef enum {SHAPE_BOX, SHAPE_SPHERE, SHAPE_TRIMESH,SHAPE_COMPOUND_TRIMESH,SHAPE_COMPOUND_BOX} collisionShapeType_t;

	btHfFluidRigidDynamicsWorld * dynamicsWorld;
	//osgbCollision::GLDebugDrawer debugDrawer;

	BulletPhysics(double configGravity[3],osgOcean::OceanTechnique* oceanSurf,PhysicsWater physicsWater);

	void setGravity(btVector3 g) {dynamicsWorld->setGravity( g );}

	btRigidBody* addDynamicObject(osg::MatrixTransform *root, osg::Node *node, btScalar mass, btVector3 inertia, collisionShapeType_t ctype, CollisionDataType * data,osg::Node * colShape = NULL );
	btRigidBody* addKinematicObject(osg::MatrixTransform *root, osg::Node *node, btScalar mass, btVector3 inertia, collisionShapeType_t ctype, CollisionDataType * data,osg::Node * colShape= NULL);

	btRigidBody* addFloatingObject(osg::MatrixTransform *root, osg::Node *node, btScalar mass, btVector3 inertia, collisionShapeType_t ctype, CollisionDataType * data ,osg::Node * colShape= NULL);

	void stepSimulation(btScalar timeStep, int maxSubSteps, btScalar fixedTimeStep );
	void printManifolds();

	int getNumCollisions();

	btPersistentManifold * getCollision(int i);

	~BulletPhysics() {};

private:
	btHfFluidRigidCollisionConfiguration * collisionConfiguration;
	btCollisionDispatcher * dispatcher;
	btConstraintSolver * solver;
	btBroadphaseInterface * inter;
 	btHfFluid* fluid;

	osgOcean::OceanTechnique* oceanSurface;

	void cleanManifolds();
	btCollisionShape* GetCSFromOSG(osg::Node * node, collisionShapeType_t ctype);
	btConvexShape* GetConvexCSFromOSG(osg::Node * node, collisionShapeType_t ctype);
	btRigidBody* addObject(osg::MatrixTransform *root, osg::Node *node, btScalar mass, btVector3 inertia, collisionShapeType_t ctype, CollisionDataType * data,osg::Node * colShape= NULL );

	void updateOceanSurface();
	
};


#endif

