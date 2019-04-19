#ifndef WHEREISWALDOVR_EXAMPLEAPP_H
#define WHEREISWALDOVR_EXAMPLEAPP_H


#include "Core.h"
#include "RiftApp.hpp"
#include "Scene.hpp"
#include <GL/glew.h>


// An example application that renders a simple cube
class ExampleApp : public RiftApp {
  std::shared_ptr <GameOverScene> cubeScene;


  bool gameStarted;


  double startTime;


  int points;

  GLuint textShaderID;

  Text* text;

  glm::mat4 proj;

  glm::mat4 toWorld;

  bool justStarted;

  double timeReference;


  int gameStatus;
  const unsigned int MAX_TIME_PER_GAME{ 5 };


public:
  ExampleApp() {
    points = 0;
    startTime = -1;
	timeReference = -1;
	gameStarted = false;
	justStarted = true;
	gameStatus = 0;
  }

protected:
  void initGl() override {
    RiftApp::initGl();
    glClearColor( 1.0f, 1.0f, 1.0f, 0.0f );
    glEnable( GL_DEPTH_TEST );
    ovr_RecenterTrackingOrigin( _session );
    //cubeScene = std::shared_ptr <ColorCubeScene>( new ColorCubeScene() );
    cubeScene = std::shared_ptr <GameOverScene>(
      new GameOverScene);
	textShaderID = LoadShaders("textShader.vert", "textShader.frag");
	text = new Text();
	proj = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);
	 toWorld = glm::translate(glm::mat4(1.0f), glm::vec3(10.0f, 0.0f, 10.0f));

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
		timeReference = ovr_GetTimeInSeconds();
		gameStarted = true;
	  justStarted = false;
	  points = 0;
	  setStartGame();
    }

    //std::cout << ovr_GetTimeInSeconds() << " vs " << startTime << std::endl;
    if( ovr_GetTimeInSeconds() - startTime > MAX_TIME_PER_GAME && gameStarted &&
        startTime != -1 ) {
      std::cout << "Game Over! You got " << points << " points." << std::endl;
      gameStarted = false;

	  setGameOver();
		  
	  
    }

	if (!gameStarted && !justStarted) {
		string gameOver = "Game Over! You got " + std::to_string(points) + " points.";
//		text->RenderText(textShaderID, gameOver, 350.0f, 300.0f, 0.05f,
//			proj, glm::inverse(headPose), toWorld);
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

	if (ovr_GetTimeInSeconds() - timeReference > 5.0  && inputState.IndexTrigger[ovrHand_Right] > 0.5f) {
		string onePointAdded = "1 Point Added";
//		text->RenderText(textShaderID, onePointAdded, 350.0f, 200.0, 0.05f,
//			proj, glm::inverse(headPose), toWorld);
		timeReference = ovr_GetTimeInSeconds();

	}

	if (gameStarted) {


		string score = "Current score: " + std::to_string(points) + ".";
//		text->RenderText(textShaderID, score, 350.0f, 400.0, 0.05f,
//			proj, glm::inverse(headPose), toWorld);

		string timeLeft = "You have " + std::to_string(int(MAX_TIME_PER_GAME - (ovr_GetTimeInSeconds() - startTime))) + " seconds left.";
//		text->RenderText(textShaderID, timeLeft, 350.0f, 300.0f, 0.05f,
//			proj, glm::inverse(headPose), toWorld);
	}

    cubeScene->render( projection, glm::inverse( headPose ),
                       controllerPosition, gameStatus );


	//render the text



	//text->RenderText(textShaderID, string("Game Start!"), 400.0f, 300.0f, 0.1f,
		//proj, glm::inverse(headPose), toWorld);
  }
};


#endif //WHEREISWALDOVR_EXAMPLEAPP_H
