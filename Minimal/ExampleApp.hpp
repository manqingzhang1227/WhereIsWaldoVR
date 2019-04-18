#ifndef WHEREISWALDOVR_EXAMPLEAPP_H
#define WHEREISWALDOVR_EXAMPLEAPP_H


#include "Core.h"
#include "RiftApp.hpp"
#include "Scene.hpp"
#include <GL/glew.h>
#include <ft2build.h>
#include FT_FREETYPE_H


// An example application that renders a simple cube
class ExampleApp : public RiftApp {
  std::shared_ptr <ColorCubeScene> cubeScene;


  bool gameStarted;


  double startTime;


  int points;


public:
  ExampleApp() {
    points = 0;
    startTime = -1;
    gameStarted = false;
  }

protected:
  void initGl() override {
    RiftApp::initGl();
    glClearColor( 1.0f, 1.0f, 1.0f, 0.0f );
    glEnable( GL_DEPTH_TEST );
    ovr_RecenterTrackingOrigin( _session );
    //cubeScene = std::shared_ptr <ColorCubeScene>( new ColorCubeScene() );
    cubeScene = std::shared_ptr <ColorCubeScene>(
      new ColorCubeScene() );
  }


  void shutdownGl() override {
    cubeScene.reset();
  }


  void renderScene( const glm::mat4 &projection,
                    const glm::mat4 &headPose ) override {
    // Get Current Input State
    ovrInputState inputState;
    ovr_GetInputState( _session, ovrControllerType_Touch, &inputState );
    if( inputState.IndexTrigger[ovrHand_Right] > 0.5f && !gameStarted ) {
      startTime = ovr_GetTimeInSeconds();
      gameStarted = true;
      std::cout << " starting at time  " << ovr_GetTimeInSeconds() << std::endl;
    }
    //std::cout << ovr_GetTimeInSeconds() << " vs " << startTime << std::endl;
    if( ovr_GetTimeInSeconds() - startTime > 60.0 && gameStarted &&
        startTime != -1 ) {
      std::cout << "Game Over! You got " << points << " points." << std::endl;
      gameStarted = false;
      points = 0;
    }


    // get the general state hmdState
    double ftiming = ovr_GetPredictedDisplayTime( _session, 0 );
    ovrTrackingState hmdState = ovr_GetTrackingState( _session,
                                                      ftiming, ovrTrue );
    // Get the state of hand poses
    ovrPoseStatef handPoseState = hmdState.HandPoses[ovrHand_Right];
    //Get the hand pose position.
    glm::vec3 controllerPosition = ovr::toGlm(
      handPoseState.ThePose.Position );

    ovr_GetInputState( _session, ovrControllerType_Touch, &inputState );
    if( inputState.IndexTrigger[ovrHand_Right] > 0.5f ) {
      //std::cout << "Touched Trigger Button" << std::endl;
      if( cubeScene->checkOverlap( controllerPosition ) && gameStarted ) {
        std::cout << "1 Point Added" << std::endl;
        std::cout << "You have " << 60 - ( ovr_GetTimeInSeconds() - startTime )
                  << " seconds left." << std::endl;
        std::cout << "Current score: " << points << "." << std::endl;
        std::cout << std::endl;

        points++;
      }
    }

    cubeScene->render( projection, glm::inverse( headPose ),
                       controllerPosition );

  }
};


#endif //WHEREISWALDOVR_EXAMPLEAPP_H
