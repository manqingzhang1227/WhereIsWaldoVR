//
// Created by Manqing (Stefanie) Zhang on 2019-04-15.
//



#ifndef WHEREISWALDOVR_EXAMPLEAPP_H
#define WHEREISWALDOVR_EXAMPLEAPP_H


#include "Core.h"
#include "RiftApp.hpp"
#include "Scene.hpp"
#include <GL/glew.h>


// An example application that renders a simple cube
class ExampleApp : public RiftApp {
  std::shared_ptr<ColorCubeScene> cubeScene;

public:
  ExampleApp() {
  }

protected:
  void initGl() override {
    RiftApp::initGl();
    glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
    glEnable(GL_DEPTH_TEST);
    ovr_RecenterTrackingOrigin(_session);
    cubeScene = std::shared_ptr<ColorCubeScene>(new ColorCubeScene());
  }

  void shutdownGl() override {
    cubeScene.reset();
  }

  void renderScene(const glm::mat4& projection, const glm::mat4& headPose) override {
	// get the general state hmdState
	double ftiming = ovr_GetPredictedDisplayTime(_session, 0);
	ovrTrackingState hmdState = ovr_GetTrackingState(_session,
		ftiming, ovrTrue);
	// Get the state of hand poses
	ovrPoseStatef handPoseState = hmdState.HandPoses[ovrHand_Right];
	//Get the hand pose position.
	glm::vec3 controllerPosition = ovr::toGlm(
		handPoseState.ThePose.Position);

	// Get Current Input State
	ovrInputState inputState;
	ovr_GetInputState(_session, ovrControllerType_Touch, &inputState);
	if (inputState.IndexTrigger[ovrHand_Right] > 0.5f)
	{
		//std::cout << "Touched Trigger Button" << std::endl;
		cubeScene->checkOverlap(controllerPosition);
	}
    cubeScene->render(projection, glm::inverse(headPose), controllerPosition);
  }
};

#endif //WHEREISWALDOVR_EXAMPLEAPP_H
