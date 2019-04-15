//
// Created by Manqing (Stefanie) Zhang on 2019-04-15.
//



#ifndef WHEREISWALDOVR_EXAMPLEAPP_H
#define WHEREISWALDOVR_EXAMPLEAPP_H


#include "Core.h"
#include "RiftApp.h"

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
    cubeScene->render(projection, glm::inverse(headPose));
  }
};

#endif //WHEREISWALDOVR_EXAMPLEAPP_H
