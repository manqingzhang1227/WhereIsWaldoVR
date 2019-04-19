//
// Created by Manqing (Stefanie) Zhang on 2019-04-15.
//



#ifndef WHEREISWALDOVR_SCENE_H
#define WHEREISWALDOVR_SCENE_H


#include <vector>
#include "shader.h"
#include "Cube.h"

#include <random>
#include <stdlib.h>     /* srand, rand */

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "Model.h"

#include "text.h"


class GameOverScene {

  // Program
  std::vector <glm::mat4> instance_positions;


  GLuint instanceCount;


  GLuint shaderID;


  Model* sphere;


  GLuint lightedInstanceIndex;


  const unsigned int GRID_SIZE{ 5 };


  const float SPACING{ 0.14f };


  Model* gameOver;
  glm::mat4 gameOverPos;
  GLuint GameOverShaderID;


  Model* gameStart;
  glm::mat4 gameStartPos;
  //use the gameovershaderid

  std::vector <glm::mat4> axis_positions;


public:
  GameOverScene() {

	  loadSphere();

	  loadAxis();

	  loadGameOver();

	  loadGameStart();
    
  }


  void highlightRandomSphere() {
    lightedInstanceIndex =
      rand() % instanceCount;         // in the range 0 to instanceCount
  }


  void loadGameOver() {
	  //for gameover text
	  gameOver = new Model("GameOver.obj");
	  GameOverShaderID = LoadShaders("gameoverShader.vert", "gameoverShader.frag");
	  float xpos = (GRID_SIZE - 1 - (GRID_SIZE / 2.0f)) * SPACING;
	  float ypos = (GRID_SIZE - 1 - (GRID_SIZE / 2.0f)) * SPACING;
	  float zpos = (GRID_SIZE - 1 - (GRID_SIZE / 2.0f)) * SPACING;
	  vec3 relativePosition = vec3(1, 1, 1);
	  glm::mat4 gameOverPos =
		  glm::translate(glm::mat4(1.0f), relativePosition)
		  //*glm::scale(glm::mat4(1.0f), vec3(0.05f))
		  ;
  }

  void loadGameStart() {
    gameStart = new Model("PullTheTriggerToStartTheGame.obj");
    GameOverShaderID = LoadShaders("gameoverShader.vert", "gameoverShader.frag");
    float xpos = (GRID_SIZE - 1 - (GRID_SIZE / 2.0f)) * SPACING;
    float ypos = (GRID_SIZE - 1 - (GRID_SIZE / 2.0f)) * SPACING;
    float zpos = (GRID_SIZE - 1 - (GRID_SIZE / 2.0f)) * SPACING;
    vec3 relativePosition = vec3(1, 1, 1);
    glm::mat4 gameStartPos =
      glm::translate(glm::mat4(1.0f), relativePosition)
    //*glm::scale(glm::mat4(1.0f), vec3(0.05f))
    ;
  }

  void loadAxis() {
	  sphere = new Model("sphere.obj");

	  //for (float z = 0; z < GRID_SIZE; z++) {
		  //for (float y = -5; y < 5; y++) {
			  //for (float x = -5; x < 5; x++) {
				  //float xpos = (x - (GRID_SIZE / 2.0f)) * SPACING;
				  //float ypos = (y - (GRID_SIZE / 2.0f)) * SPACING;
				  //float zpos = (z - (GRID_SIZE / 2.0f)) * SPACING;
				  //vec3 relativePosition = vec3(xpos, ypos, zpos);
				  //if (relativePosition == vec3(0)) {
					//  continue;
				  //}
				  glm::mat4 transfMat =
					  glm::translate(glm::mat4(1.0f), vec3(0, 0, 0)) *
					  glm::scale(glm::mat4(1.0f), vec3(0.07, 0.07, 0.07));
				  axis_positions.push_back(transfMat);

				  
			  //}
		  //}
	 // }

	  // Shader Program
	  shaderID = LoadShaders("shader.vert", "shader.frag");
  }

  void loadSphere() {
	  sphere = new Model("sphere.obj");

	  for (float z = 0; z < GRID_SIZE; z++) {
		  for (float y = 0; y < GRID_SIZE; y++) {
			  for (float x = 0; x < GRID_SIZE; x++) {
				  float xpos = (x - (GRID_SIZE / 2.0f)) * SPACING;
				  float ypos = (y - (GRID_SIZE / 2.0f)) * SPACING;
				  float zpos = (z - (GRID_SIZE / 2.0f)) * SPACING;
				  vec3 relativePosition = vec3(xpos, ypos, zpos);
				  if (relativePosition == vec3(0)) {
					  continue;
				  }
				  glm::mat4 transfMat =
					  glm::translate(glm::mat4(1.0f), relativePosition) *
					  glm::scale(glm::mat4(1.0f), vec3(0.035, 0.035, 0.035));
				  instance_positions.push_back(transfMat);
			  }
		  }
	  }


	  instanceCount = (GLuint)instance_positions.size();
	  lightedInstanceIndex = rand() % instanceCount;         // in the range 0 to instanceCount


	  // Shader Program
	  shaderID = LoadShaders("shader.vert", "shader.frag");
  }


  bool checkOverlap( const glm::vec3 &controllerPosition ) {
    glm::vec4 highlightedCubePosition_h =
      instance_positions.at( lightedInstanceIndex ) *
      vec4( 0.0f, 0.0f, 0.0f, 1.0f );
    glm::vec3 highlightedCubePosition =
      vec3( highlightedCubePosition_h.x, highlightedCubePosition_h.y,
            highlightedCubePosition_h.z ) / highlightedCubePosition_h.w;
    float distance = glm::length(
      controllerPosition - highlightedCubePosition );

    if( distance < 0.035f ) {
      highlightRandomSphere();
      return true;
    }
    else {
      return false;
    }
  }

  void renderGameOverText(const glm::mat4 &projection, const glm::mat4 &view,
	  const glm::vec3 &controllerPosition) {
	  glm::mat4 gameOverTransform =
		  gameOverPos
		  * glm::translate(glm::mat4(1.0f), vec3(3.5, -13, 1))
		  * glm::scale(glm::mat4(1.0f), vec3(20, 20, 1))
		  * glm::translate(glm::mat4(1.0f), vec3(-0.5, 1, -10))
		  * glm::scale(glm::mat4(1.0f), vec3(0.01))
		  * glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), vec3(0, 0, 1));

	 	  gameOver->Draw(GameOverShaderID, projection, view, gameOverTransform, -1);
  }

  void renderStartText(const glm::mat4 &projection, const glm::mat4 &view,
                          const glm::vec3 &controllerPosition) {
    glm::mat4 gameStartTransform =
      gameStartPos
		* glm::scale(glm::mat4(1.0f), vec3(0.05, 0.05, 0.005))
		* glm::translate(glm::mat4(1.0f), vec3(-30, 30, -500))
      //* glm::scale(glm::mat4(1.0f), vec3(10, 10, 1))
      //* glm::translate(glm::mat4(1.0f), vec3(-0.5, 1, -10))
      //* glm::scale(glm::mat4(1.0f), vec3(0.01))
      * glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), vec3(0, 0, 1));

    gameStart->Draw(GameOverShaderID, projection, view, gameStartTransform, -1);
  }

  void renderGrid(const glm::mat4 &projection, const glm::mat4 &view,
	  const glm::vec3 &controllerPosition) {
	  for (unsigned int i = 0; i < instanceCount; i++) {
		  glm::mat4 toWorld = instance_positions[i] *
			  glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
		  sphere->Draw(shaderID, projection, view, toWorld,
			  (lightedInstanceIndex == i));
	  }
  }

  void renderAxis(const glm::mat4 &projection, const glm::mat4 &view,
	  const glm::vec3 &controllerPosition) {
	  for (unsigned int i = 0; i < axis_positions.size(); i++) {
		  glm::mat4 toWorld = axis_positions[i] *
			  glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
		  sphere->Draw(shaderID, projection, view, toWorld,
			  3);
	  }
  }

  void renderController(const glm::mat4 &projection, const glm::mat4 &view,
	  const glm::vec3 &controllerPosition) {
	  glm::mat4 controller_transform =
		  glm::translate(glm::mat4(1.0f), controllerPosition) *
		  glm::scale(glm::mat4(1.0f), vec3(0.0175, 0.0175, 0.0175));
	  sphere->Draw(shaderID, projection, view, controller_transform, 2);
  }

  void render(const glm::mat4 &projection, const glm::mat4 &view,
	  const glm::vec3 &controllerPosition, int status) {


	  renderAxis(projection, view, controllerPosition);

    if( status <= 1 ) {
		if (status == 0) {
			renderStartText(projection, view, controllerPosition);
		}
	  renderGrid(projection, view, controllerPosition);
	  renderController(projection, view, controllerPosition);
    }
    
	  else if (status == 2){
		  renderGameOverText(projection, view, controllerPosition);
	  }
  }
};


// a class for building and rendering cubes
class ColorSphereSceneWithText {

  // Program
  std::vector <glm::mat4> instance_positions;


  GLuint instanceCount;


  GLuint shaderID;


  Model* sphere;


  GLuint lightedInstanceIndex;


  const unsigned int GRID_SIZE{ 5 };


  const float SPACING{ 0.14f };


public:
  ColorSphereSceneWithText() {
    // Create a cube of cubes
    sphere = new Model( "sphere.obj" );

    for( float z = 0; z < GRID_SIZE; z++ ) {
      for( float y = 0; y < GRID_SIZE; y++ ) {
        for( float x = 0; x < GRID_SIZE; x++ ) {
          float xpos = ( x - ( GRID_SIZE / 2.0f ) ) * SPACING;
          float ypos = ( y - ( GRID_SIZE / 2.0f ) ) * SPACING;
          float zpos = ( z - ( GRID_SIZE / 2.0f ) ) * SPACING;
          vec3 relativePosition = vec3( xpos, ypos, zpos );
          if( relativePosition == vec3( 0 ) ) {
            continue;
          }
          glm::mat4 transfMat =
            glm::translate( glm::mat4( 1.0f ), relativePosition ) *
            glm::scale( glm::mat4( 1.0f ), vec3( 0.035, 0.035, 0.035 ) );
          instance_positions.push_back( transfMat );
        }
      }
    }


    instanceCount = ( GLuint ) instance_positions.size();
    lightedInstanceIndex = rand() % instanceCount;
    // in the range 0 to instanceCount

    // Shader Program
    shaderID = LoadShaders( "shader.vert", "shader.frag" );


  }


  void highlightRandomSphere() {
    lightedInstanceIndex = rand() % instanceCount;
    // in the range 0 to instanceCount
  }


  bool checkOverlap( const glm::vec3 &controllerPosition ) {
    glm::vec4 highlightedCubePosition_h =
      instance_positions.at( lightedInstanceIndex ) *
      vec4( 0.0f, 0.0f, 0.0f, 1.0f );
    glm::vec3 highlightedCubePosition =
      vec3( highlightedCubePosition_h.x, highlightedCubePosition_h.y,
            highlightedCubePosition_h.z ) / highlightedCubePosition_h.w;
    float distance = glm::length(
      controllerPosition - highlightedCubePosition );

    if( distance < 0.035f ) {
      highlightRandomSphere();
      return true;
    }
    else {
      return false;
    }
  }


  void render( const glm::mat4 &projection, const glm::mat4 &view,
               const glm::vec3 &controllerPosition ) {

    //render the spheres chunk

    for( unsigned int i = 0; i < instanceCount; i++ ) {
      glm::mat4 toWorld = instance_positions[i] *
                          glm::scale( glm::mat4( 1.0f ), glm::vec3( 0.5f ) );
      sphere->Draw( shaderID, projection, view, toWorld,
                    ( lightedInstanceIndex == i ) );
    }


    //render the controller sphere
    glm::mat4 controller_transform =
      glm::translate( glm::mat4( 1.0f ), controllerPosition ) *
      glm::scale( glm::mat4( 1.0f ), vec3( 0.0175, 0.0175, 0.0175 ) );
    sphere->Draw( shaderID, projection, view, controller_transform, 2 );


  }


};


// a class for building and rendering cubes
class ColorCubeScene {

  // Program
  std::vector <glm::mat4> instance_positions;


  GLuint instanceCount;


  GLuint shaderID;


  Model* sphere;


  GLuint lightedInstanceIndex;


  const unsigned int GRID_SIZE{ 5 };


  const float SPACING{ 0.14f };


public:
  ColorCubeScene() {
    // Create a cube of cubes
    {

      sphere = new Model( "sphere.obj" );

      for( float z = 0; z < GRID_SIZE; z++ ) {
        for( float y = 0; y < GRID_SIZE; y++ ) {
          for( float x = 0; x < GRID_SIZE; x++ ) {
            float xpos = ( x - ( GRID_SIZE / 2.0f ) ) * SPACING;
            float ypos = ( y - ( GRID_SIZE / 2.0f ) ) * SPACING;
            float zpos = ( z - ( GRID_SIZE / 2.0f ) ) * SPACING;
            vec3 relativePosition = vec3( xpos, ypos, zpos );
            if( relativePosition == vec3( 0 ) ) {
              continue;
            }
            glm::mat4 transfMat =
              glm::translate( glm::mat4( 1.0f ), relativePosition ) *
              glm::scale( glm::mat4( 1.0f ), vec3( 0.035, 0.035, 0.035 ) );
            instance_positions.push_back( transfMat );
          }
        }
      }
    }

    instanceCount = ( GLuint ) instance_positions.size();
    lightedInstanceIndex =
      rand() % instanceCount;         // in the range 0 to instanceCount


    // Shader Program
    shaderID = LoadShaders( "shader.vert", "shader.frag" );

  }


  void highlightRandomSphere() {
    lightedInstanceIndex =
      rand() % instanceCount;         // in the range 0 to instanceCount
  }


  bool checkOverlap( const glm::vec3 &controllerPosition ) {
    glm::vec4 highlightedCubePosition_h =
      instance_positions.at( lightedInstanceIndex ) *
      vec4( 0.0f, 0.0f, 0.0f, 1.0f );
    glm::vec3 highlightedCubePosition =
      vec3( highlightedCubePosition_h.x, highlightedCubePosition_h.y,
            highlightedCubePosition_h.z ) / highlightedCubePosition_h.w;
    float distance = glm::length(
      controllerPosition - highlightedCubePosition );

    if( distance < 0.035f ) {
      highlightRandomSphere();
      return true;
    }
    else {
      return false;
    }
  }


  void render( const glm::mat4 &projection, const glm::mat4 &view,
               const glm::vec3 &controllerPosition ) {

    for( unsigned int i = 0; i < instanceCount; i++ ) {
      glm::mat4 toWorld = instance_positions[i] *
                          glm::scale( glm::mat4( 1.0f ), glm::vec3( 0.5f ) );
      sphere->Draw( shaderID, projection, view, toWorld,
                    ( lightedInstanceIndex == i ) );
    }
    glm::mat4 controller_transform =
      glm::translate( glm::mat4( 1.0f ), controllerPosition ) *
      glm::scale( glm::mat4( 1.0f ), vec3( 0.0175, 0.0175, 0.0175 ) );
    sphere->Draw( shaderID, projection, view, controller_transform, 2 );

  }


};


#endif //WHEREISWALDOVR_SCENE_H
