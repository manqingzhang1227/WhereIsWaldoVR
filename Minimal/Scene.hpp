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
#include <soil.h>

#include "Mesh.h"
#include "Model.h"

//#include "text.h"


// a class for building and rendering cubes
class ColorSphereSceneWithText {

  // Program
  std::vector <glm::mat4> instance_positions;


  GLuint instanceCount;


  GLuint shaderID;


  Model* sphere;


  GLuint textShaderID;


  //Text* text;


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


    instanceCount = instance_positions.size();
    lightedInstanceIndex = rand() % instanceCount;
    // in the range 0 to instanceCount


    // Shader Program
    shaderID = LoadShaders( "shader.vert", "shader.frag" );

    //textShaderID = LoadShaders( "textShader.vert", "textShader.frag" );

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

    for( int i = 0; i < instanceCount; i++ ) {
      glm::mat4 toWorld = instance_positions[i] *
                          glm::scale( glm::mat4( 1.0f ), glm::vec3( 0.5f ) );
      sphere->Draw( shaderID, projection, view, toWorld,
                    ( lightedInstanceIndex == i ) );
    }
    glm::mat4 controller_transform =
      glm::translate( glm::mat4( 1.0f ), controllerPosition ) *
      glm::scale( glm::mat4( 1.0f ), vec3( 0.0175, 0.0175, 0.0175 ) );
    sphere->Draw( shaderID, projection, view, controller_transform, 2 );

    //text->RenderText( textShaderID, new string("TEXT") );
    //render the text

  }


};


// a class for building and rendering cubes
class ColorCubeScene {

  // Program
  std::vector <glm::mat4> instance_positions;


  GLuint instanceCount;


  GLuint shaderID, textureShaderID;

  GLint uniform_texture;

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

    instanceCount = instance_positions.size();
    lightedInstanceIndex =
      rand() % instanceCount;         // in the range 0 to instanceCount


    // Shader Program
    shaderID = LoadShaders( "shader.vert", "shader.frag" );

	textureShaderID = LoadShaders("textureShader.vert", "textureShader.frag");

	glActiveTexture(GL_TEXTURE0);
	GLuint textureId = SOIL_load_OGL_texture("Waldo.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y | SOIL_FLAG_TEXTURE_REPEATS);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glUniform1i()


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

    for( int i = 0; i < instanceCount; i++ ) {
      glm::mat4 toWorld = instance_positions[i] *
                          glm::scale( glm::mat4( 1.0f ), glm::vec3( 0.5f ) );
      sphere->Draw( shaderID, projection, view, toWorld,
                    ( lightedInstanceIndex == i ) );
    }
    glm::mat4 controller_transform =
      glm::translate( glm::mat4( 1.0f ), controllerPosition ) *
      glm::scale( glm::mat4( 1.0f ), vec3( 0.0175, 0.0175, 0.0175 ) );
    sphere->Draw( shaderID, projection, view, controller_transform, 2 );

    //render the text

  }


};


#endif //WHEREISWALDOVR_SCENE_H
