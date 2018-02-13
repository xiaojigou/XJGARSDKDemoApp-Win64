
project(libXJGARSDKTest)

cmake_minimum_required(VERSION 2.8)


find_package(OpenCV REQUIRED) 
 

include_directories( thirdPartLibs/freeglut-3.0.0/include  )
include_directories( thirdPartLibs/glew-2.1.0/include  )

ADD_EXECUTABLE(libXJGARSDKTest  
				test_XJGARSDK_freeglut.cpp
				libVidCapInterface.cpp 
				libVidCapInterface.h 
				XJGArSdk.h)	
TARGET_LINK_LIBRARIES(libXJGARSDKTest ${OpenCV_LIBS} 
				libXJGARSDK
				)




ADD_EXECUTABLE(libXJGARSDKTest_simple  
				test_XJGARSDK_simple.cpp 
				libVidCapInterface.cpp 
				libVidCapInterface.h )	
TARGET_LINK_LIBRARIES(libXJGARSDKTest_simple ${OpenCV_LIBS} 
				libXJGARSDK
				#glew32.lib
				)


