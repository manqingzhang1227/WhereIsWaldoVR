//
// Created by Manqing (Stefanie) Zhang on 2019-04-15.
//

#ifndef WHEREISWALDOVR_CORE_H
#define WHEREISWALDOVR_CORE_H


#include <iostream>
#include <memory>
#include <exception>
#include <algorithm>

#include <Windows.h>

#define __STDC_FORMAT_MACROS 1

#define FAIL(X) throw std::runtime_error(X)

///////////////////////////////////////////////////////////////////////////////
//
// GLM is a C++ math library meant to mirror the syntax of GLSL
//

#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>



///////////////////////////////////////////////////////////////////////////////
//
// GLEW gives cross platform access to OpenGL 3.x+ functionality.
//

#include <GL/glew.h>

#include <GLFW/glfw3.h>


#include <OVR_CAPI.h>
#include <OVR_CAPI_GL.h>

#include <vector>
#include "shader.h"
#include "Cube.h"


#include "ExampleApp.h
#include "GlfwApp.h"
#include "ovr.h"
#include "RiftApp.h"
#include "RiftManagerApp.h"
#include "Scene.h"

#endif //WHEREISWALDOVR_CORE_H
