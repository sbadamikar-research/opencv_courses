cmake_minimum_required(VERSION 3.10)

PROJECT(CVIP_Assignments)
SET(CMAKE_CXX_STANDARD 11)

######################## EDIT IF REQUIRED ####################
# ###Uncomment the line below and specify the path to OpenCV directory i.e. the path to the OpenCVConfig.cmake file. Check the examples given below.
SET(OpenCV_DIR $CONDA_PREFIX/envs/learn_opencv/lib/cmake/opencv4/OpenCVConfig.cmake)
SET(CMAKE_PREFIX_PATH $ENV{CONDA_PREFIX} ${CMAKE_PREFIX_PATH})

# SET(OpenCV_DIR /usr/local/share/OpenCV)

################### OpenCV_DIR Examples  #####################

### MACOS : /usr/local/Cellar/opencv/3.3.1_1/share/OpenCV/

### UBUNTU : /usr/local/share/OpenCV/

### WINDOWS : C:\Users\yourname\Documents\opencv-3.3.1\build\install

##############################################################

###################   ***DO NOT EDIT***  #####################

#############  Common Instructions for all Users  ############
if(MSVC)
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
endif()

find_package( OpenCV REQUIRED )

include_directories( ${OpenCV_INCLUDE_DIRS})

MACRO(add_example name)
  ADD_EXECUTABLE(${name}_submission ${name}.cpp)
  TARGET_LINK_LIBRARIES(${name}_submission ${OpenCV_LIBS} )
ENDMACRO()

add_example(assn2)