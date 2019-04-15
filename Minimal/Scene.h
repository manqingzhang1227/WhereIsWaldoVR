//
// Created by Manqing (Stefanie) Zhang on 2019-04-15.
//



#ifndef WHEREISWALDOVR_SCENE_H
#define WHEREISWALDOVR_SCENE_H

#include <vector>
#include "shader.h"
#include "Cube.h"
// a class for building and rendering cubes
class ColorCubeScene {

  // Program
  std::vector<glm::mat4> instance_positions;
  GLuint instanceCount;
  GLuint shaderID;
  std::unique_ptr<Cube> cube;

  const unsigned int GRID_SIZE{5};

public:
  ColorCubeScene() {
    // Create a cube of cubes
    {
      for (unsigned int z = 0; z < GRID_SIZE; ++z) {
        for (unsigned int y = 0; y < GRID_SIZE; ++y) {
          for (unsigned int x = 0; x < GRID_SIZE; ++x) {
            int xpos = (x - (GRID_SIZE / 2)) * 2;
            int ypos = (y - (GRID_SIZE / 2)) * 2;
            int zpos = (z - (GRID_SIZE / 2)) * 2;
            vec3 relativePosition = vec3(xpos, ypos, zpos);
            if (relativePosition == vec3(0)) {
              continue;
            }
            instance_positions.push_back(glm::translate(glm::mat4(1.0f), relativePosition));
          }
        }
      }
    }

    instanceCount = instance_positions.size();

    // Shader Program
    shaderID = LoadShaders("shader.vert", "shader.frag");

    // Cube
    cube = std::make_unique<Cube>();
  }

  void render(const glm::mat4& projection, const glm::mat4& view) {
    for (int i = 0; i < instanceCount; i++) {
      cube->toWorld = instance_positions[i] * glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
      cube->draw(shaderID, projection, view);
    }
  }
};


#endif //WHEREISWALDOVR_SCENE_H
