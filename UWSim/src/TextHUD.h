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

// ----------------------------------------------------
//                  Text HUD Class
// ----------------------------------------------------
#include <osg/PositionAttitudeTransform>

class TextHUD : public osg::Referenced
{
private:
    osg::ref_ptr< osg::Camera > _camera;
    osg::ref_ptr< osgText::Text > _modeText;    
    osg::ref_ptr< osgText::Text > _cameraModeText;    

public:
    TextHUD( void ){
        _camera = createCamera();
        _camera->addChild( createText() );
    }

    osg::Camera* createCamera( void )
    {
        osg::ref_ptr<osg::Camera> camera=new osg::Camera;

        camera->setViewport(0,0,1024,768);
        camera->setReferenceFrame( osg::Transform::ABSOLUTE_RF );
        camera->setProjectionMatrixAsOrtho2D(0,1024,0,768);
        camera->setRenderOrder(osg::Camera::POST_RENDER);
        camera->getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
        camera->setClearMask(GL_DEPTH_BUFFER_BIT);
//	camera->setProjectionResizePolicy(osg::Camera::VERTICAL);

        return camera.release();
    }

    osg::Node* createText( void )
    {
        osg::Geode* textGeode = new osg::Geode;

        osgText::Text* title = new osgText::Text;
        //title->setFont("fonts/arial.ttf");
        title->setCharacterSize(24);
        title->setLineSpacing(0.4f);

        std::string versionText = 
            std::string("\nPress 'h' for options");

        title->setText(versionText);
        textGeode->addDrawable( title );

        _modeText = new osgText::Text;
        //_modeText->setFont("fonts/arial.ttf");
        _modeText->setCharacterSize(24);
        _modeText->setPosition( osg::Vec3f(0.f, -40.f, 0.f ) );
        _modeText->setDataVariance(osg::Object::DYNAMIC);
        textGeode->addDrawable( _modeText.get() );

        _cameraModeText = new osgText::Text;
        //_cameraModeText->setFont("fonts/arial.ttf");
        _cameraModeText->setCharacterSize(24);
        _cameraModeText->setPosition( osg::Vec3f(0.f, -60.f, 0.f ) );
        _cameraModeText->setDataVariance(osg::Object::DYNAMIC);
        //textGeode->addDrawable( _cameraModeText.get() );

        osg::PositionAttitudeTransform* titlePAT = new osg::PositionAttitudeTransform;
        titlePAT->setPosition( osg::Vec3f( 10, 70, 0.f ) );
        //titlePAT->addChild(textGeode);

        return titlePAT;
    }

    void setSceneText( const std::string& preset )
    {
        _modeText->setText( "\nWeather: " + preset + "\n" );
    }

    void setCameraText(const std::string& mode )
    {
        _cameraModeText->setText( "Camera: " + mode );
    }

    osg::Camera* getHudCamera(void)
    {
        return _camera.get();
    }
};
