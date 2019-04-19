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

#include "Model.h"

#include <soil.h>

#include "stb_image.h"

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


  GLuint shaderID, textureShaderID, textureId;


  GLint uniform_texture, attribute_texture;


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

  std::vector <Model*> nums;


  glm::mat4 numPos;


  glm::mat4 numStartPos;
  //use the gameovershaderid


  std::vector <glm::mat4> axis_positions;


  Model* scoreText;
  glm::mat4 scoreTextPos;
  //use the gameovershaderid

  Model* timeLeftText;
  glm::mat4 timeLeftTextPos;
  //use the gameovershaderid

public:
  ColorCubeScene() {

    loadAxis();

    loadGameOver();

    loadGameStart();
    loadNumbers();
    loadScoreText();
    //loadTimeLeftText();

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

    textureShaderID = LoadShaders( "textureShader.vert", "textureShader.frag" );

    glActiveTexture( GL_TEXTURE0 );

    //Setting up textures
    unsigned char* data;
    int width, height, numChannels;

    data = stbi_load( "waldo.jpg", &width, &height, &numChannels, STBI_rgb_alpha );
    if( !data ) {
      throw std::runtime_error( "Cannot load file waldo.jpg" );
    }

    glGenTextures( 1, &textureId );
    glBindTexture( GL_TEXTURE_2D, textureId );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data );

    // textureId = SOIL_load_OGL_texture("waldo.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_TEXTURE_REPEATS);
    uniform_texture = glGetUniformLocation( textureShaderID, "tex" );
    glUniform1i( uniform_texture, 0 );
    glBindTexture( GL_TEXTURE_2D, textureId );

  }



  void render( const glm::mat4 &projection, const glm::mat4 &view,
               const glm::vec3 &controllerPosition, int status, int timeLeft, int score ) {

    //renderAxis( projection, view, controllerPosition );


    if( status == 0 ) {
      renderStartText( projection, view, controllerPosition );
    }
    if( status == 1 ) {
      for( int i = 0; i < instanceCount; i++ ) {
        glm::mat4 toWorld = instance_positions[i] *
                            glm::scale( glm::mat4( 1.0f ), glm::vec3( 0.5f ) );
        if( lightedInstanceIndex == i ) {
          sphere->Draw( textureShaderID, projection, view, toWorld, ( lightedInstanceIndex == i ) );
        }
        else {
          sphere->Draw( shaderID, projection, view, toWorld,
                        ( lightedInstanceIndex == i ) );
        }
      }
      glm::mat4 controller_transform =
        glm::translate( glm::mat4( 1.0f ), controllerPosition ) *
        glm::scale( glm::mat4( 1.0f ), vec3( 0.0175, 0.0175, 0.0175 ) );
      sphere->Draw( shaderID, projection, view, controller_transform, 2 );
      //renderAxis( projection, view, controllerPosition );

      renderNumber( projection, view, controllerPosition, timeLeft, status );


    }
    else if( status == 2 ) {
      renderGameOverText( projection, view, controllerPosition );
      renderNumber( projection, view, controllerPosition, score,  status );
    }


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






  void loadNumbers() {

    for( unsigned int i = 0; i < 10; i++ ) {
      string fileName = "Number" + std::to_string( i ) + ".obj";
	  std::cout << fileName << std::endl;
      nums.push_back( new Model( fileName ) );

    }
    GameOverShaderID = LoadShaders( "gameoverShader.vert", "gameoverShader.frag" );
    float xpos = ( GRID_SIZE - 1 - ( GRID_SIZE / 2.0f ) ) * SPACING;
    float ypos = ( GRID_SIZE - 1 - ( GRID_SIZE / 2.0f ) ) * SPACING;
    float zpos = ( GRID_SIZE - 1 - ( GRID_SIZE / 2.0f ) ) * SPACING;
    vec3 relativePosition = vec3( 1, 1, 1 );
    glm::mat4 numPos =
      glm::translate( glm::mat4( 1.0f ), relativePosition )
    //*glm::scale(glm::mat4(1.0f), vec3(0.05f))
    ;

  }


  void loadGameOver() {
    //for gameover text
    gameOver = new Model( "GameOver.obj" );
    GameOverShaderID = LoadShaders( "gameoverShader.vert", "gameoverShader.frag" );
    float xpos = ( GRID_SIZE - 1 - ( GRID_SIZE / 2.0f ) ) * SPACING;
    float ypos = ( GRID_SIZE - 1 - ( GRID_SIZE / 2.0f ) ) * SPACING;
    float zpos = ( GRID_SIZE - 1 - ( GRID_SIZE / 2.0f ) ) * SPACING;
    vec3 relativePosition = vec3( 1, 1, 1 );
    glm::mat4 gameOverPos =
      glm::translate( glm::mat4( 1.0f ), relativePosition )
    //*glm::scale(glm::mat4(1.0f), vec3(0.05f))
    ;
  }


  void loadScoreText() {
    //for gameover text
    scoreText = new Model( "Score.obj" );
    GameOverShaderID = LoadShaders( "gameoverShader.vert", "gameoverShader.frag" );
    float xpos = ( GRID_SIZE - 1 - ( GRID_SIZE / 2.0f ) ) * SPACING;
    float ypos = ( GRID_SIZE - 1 - ( GRID_SIZE / 2.0f ) ) * SPACING;
    float zpos = ( GRID_SIZE - 1 - ( GRID_SIZE / 2.0f ) ) * SPACING;
    vec3 relativePosition = vec3( 1, 1, 1 );
    glm::mat4 scoreTextPos =
      glm::translate( glm::mat4( 1.0f ), relativePosition )
    //*glm::scale(glm::mat4(1.0f), vec3(0.05f))
    ;
  }

  void loadTimeLeftText() {
    //for gameover text
    scoreText = new Model( "TimeLeft.obj" );
    GameOverShaderID = LoadShaders( "gameoverShader.vert", "gameoverShader.frag" );
    float xpos = ( GRID_SIZE - 1 - ( GRID_SIZE / 2.0f ) ) * SPACING;
    float ypos = ( GRID_SIZE - 1 - ( GRID_SIZE / 2.0f ) ) * SPACING;
    float zpos = ( GRID_SIZE - 1 - ( GRID_SIZE / 2.0f ) ) * SPACING;
    vec3 relativePosition = vec3( 1, 1, 1 );
    glm::mat4 timeLeftPos =
      glm::translate( glm::mat4( 1.0f ), relativePosition )
    //*glm::scale(glm::mat4(1.0f), vec3(0.05f))
    ;
  }

  void loadGameStart() {
    gameStart = new Model( "PullTheTriggerToStartTheGame.obj" );
    GameOverShaderID = LoadShaders( "gameoverShader.vert", "gameoverShader.frag" );
    float xpos = ( GRID_SIZE - 1 - ( GRID_SIZE / 2.0f ) ) * SPACING;
    float ypos = ( GRID_SIZE - 1 - ( GRID_SIZE / 2.0f ) ) * SPACING;
    float zpos = ( GRID_SIZE - 1 - ( GRID_SIZE / 2.0f ) ) * SPACING;
    vec3 relativePosition = vec3( 1, 1, 1 );
    glm::mat4 gameStartPos =
      glm::translate( glm::mat4( 1.0f ), relativePosition )
    //*glm::scale(glm::mat4(1.0f), vec3(0.05f))
    ;
  }


  void loadAxis() {


    sphere = new Model( "sphere.obj" );

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
      glm::translate( glm::mat4( 1.0f ), vec3( 0, 0, 0 ) ) *
      glm::scale( glm::mat4( 1.0f ), vec3( 0.07, 0.07, 0.07 ) );
    axis_positions.push_back( transfMat );


    //}
    //}
// }

    // Shader Program
    shaderID = LoadShaders( "shader.vert", "shader.frag" );
  }


  void loadSphere() {
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
    lightedInstanceIndex = rand() % instanceCount;         // in the range 0 to instanceCount


    // Shader Program
    shaderID = LoadShaders( "shader.vert", "shader.frag" );
  }


  void renderGameOverText( const glm::mat4 &projection, const glm::mat4 &view,
                           const glm::vec3 &controllerPosition ) {
    glm::mat4 gameOverTransform =
      gameOverPos
      * glm::translate( glm::mat4( 1.0f ), vec3( 3.5, -15, -2 ) )
      * glm::scale( glm::mat4( 1.0f ), vec3( 20, 20, 1 ) )
      * glm::translate( glm::mat4( 1.0f ), vec3( -0.5, 1, -10 ) )
      * glm::scale( glm::mat4( 1.0f ), vec3( 0.01 ) )
      * glm::rotate( glm::mat4( 1.0f ), glm::radians( -90.0f ), vec3( 0, 0, 1 ) );

    gameOver->Draw( GameOverShaderID, projection, view, gameOverTransform, -1 );
  }


  void renderStartText( const glm::mat4 &projection, const glm::mat4 &view,
                        const glm::vec3 &controllerPosition ) {
    glm::mat4 gameStartTransform =
      gameStartPos
      * glm::scale( glm::mat4( 1.0f ), vec3( 0.05, 0.05, 0.005 ) )
      * glm::translate( glm::mat4( 1.0f ), vec3( -30, 30, -500 ) )
      //* glm::scale(glm::mat4(1.0f), vec3(10, 10, 1))
      //* glm::translate(glm::mat4(1.0f), vec3(-0.5, 1, -10))
      //* glm::scale(glm::mat4(1.0f), vec3(0.01))
      * glm::rotate( glm::mat4( 1.0f ), glm::radians( -90.0f ), vec3( 0, 0, 1 ) );

    gameStart->Draw( GameOverShaderID, projection, view, gameStartTransform, -1 );
  }


  void renderGrid( const glm::mat4 &projection, const glm::mat4 &view,
                   const glm::vec3 &controllerPosition ) {
    for( unsigned int i = 0; i < instanceCount; i++ ) {
      glm::mat4 toWorld = instance_positions[i] *
                          glm::scale( glm::mat4( 1.0f ), glm::vec3( 0.5f ) );
      sphere->Draw( shaderID, projection, view, toWorld,
                    ( lightedInstanceIndex == i ) );
    }
  }


  void renderAxis( const glm::mat4 &projection, const glm::mat4 &view,
                   const glm::vec3 &controllerPosition ) {
    for( unsigned int i = 0; i < axis_positions.size(); i++ ) {
      glm::mat4 toWorld = axis_positions[i] *
                          glm::scale( glm::mat4( 1.0f ), glm::vec3( 0.5f ) );
      sphere->Draw( shaderID, projection, view, toWorld,
                    -1 );
    }
  }


  void renderController( const glm::mat4 &projection, const glm::mat4 &view,
                         const glm::vec3 &controllerPosition ) {
    glm::mat4 controller_transform =
      glm::translate( glm::mat4( 1.0f ), controllerPosition ) *
      glm::scale( glm::mat4( 1.0f ), vec3( 0.0175, 0.0175, 0.0175 ) );
    sphere->Draw( shaderID, projection, view, controller_transform, 2 );
  }


  void renderNumber(const glm::mat4 &projection, const glm::mat4 &view,
	  const glm::vec3 &controllerPosition, int numToRender, int status) {
	  //	  string tenth, one;
	  //	  if (status == 1) {
	  //		  tenth = std::to_string(timeLeft / 10);
	  //		  one = std::to_string(timeLeft % 10);
	  //	  }
	  //	  else if (status == 2) {
	  //		  tenth = std::to_string(score / 10);
	  //		  one = std::to_string(score % 10);
	  //	  }


	  glm::mat4 NumberTransform = numPos
		  * glm::translate(glm::mat4(1.0f), vec3(2, -3, -10))
		  * glm::scale(glm::mat4(1.0f), vec3(10, 10, 1))
		  * glm::translate(glm::mat4(1.0f), vec3(-0.5, 1, -10))
		  * glm::scale(glm::mat4(1.0f), vec3(0.01))
		  * glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f),
			  vec3(0, 0, 1));

	  int tenthIndex = numToRender / 10;
	  int oneIndex = numToRender % 10;

	  nums[tenthIndex]->Draw(GameOverShaderID, projection, view, NumberTransform, -1);

	  NumberTransform = NumberTransform
		  * glm::translate(glm::mat4(1.0f), vec3(0, 25, 0));

	  nums[oneIndex]->Draw(GameOverShaderID, projection, view, NumberTransform, -1);

    NumberTransform = NumberTransform
                      * glm::translate(glm::mat4(1.0f), vec3(0, -70, 0));

    if( status == 1 ) {
      //timeLeftText->Draw(GameOverShaderID, projection, view, NumberTransform, -1);
    }
    else {
      scoreText->Draw(GameOverShaderID, projection, view, NumberTransform, -1);
    }
  }


};


#endif //WHEREISWALDOVR_SCENE_H
