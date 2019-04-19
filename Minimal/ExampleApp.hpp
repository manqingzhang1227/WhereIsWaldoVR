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


  int gameStatus;
  const unsigned int MAX_TIME_PER_GAME{ 5 };

public:
  ExampleApp() {
    points = 0;
    startTime = -1;
    gameStarted = false;
	gameStatus = 0;

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
  void setStartGame() {
	  gameStatus = 1;
  }

  void setGameOver() {
	  gameStatus = 2;
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
	  setStartGame();
	  points = 0;

	
	}
    //std::cout << ovr_GetTimeInSeconds() << " vs " << startTime << std::endl;
    if( ovr_GetTimeInSeconds() - startTime > MAX_TIME_PER_GAME && gameStarted &&
        startTime != -1 ) {
      std::cout << "Game Over! You got " << points << " points." << std::endl;
      gameStarted = false;
	  setGameOver();

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
        std::cout << "You have " << MAX_TIME_PER_GAME - ( ovr_GetTimeInSeconds() - startTime )
                  << " seconds left." << std::endl;
        std::cout << "Current score: " << points << "." << std::endl;
        std::cout << std::endl;

        points++;
      }
    }

	string tenth;
	string one;



	//std::cout << tenth << "\t"<< one << std::endl;
	//std::cout << "Current score: " << points << "." << std::endl;

    cubeScene->render( projection, glm::inverse( headPose ),
                       controllerPosition, gameStatus, (int)(MAX_TIME_PER_GAME - (ovr_GetTimeInSeconds() - startTime), points);

  }
};


#endif //WHEREISWALDOVR_EXAMPLEAPP_H
