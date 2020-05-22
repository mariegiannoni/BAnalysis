




cmake_minimum_required(VERSION 2.8.12)

project(EyesBlinking_project)
find_package( OpenCV REQUIRED )


# Tell cmake we will need dlib.  This command will pull in dlib and compile it
# into your project.  Note that you don't need to compile or install dlib.  All
# cmake needs is the dlib source code folder and it will take care of everything.
add_subdirectory(/home/herrygabriel/ALEDAPP/DlibLibrary/dlib dlib_build)

 
add_executable(EyesBlinking EyesBlinking.cpp)


target_link_libraries(EyesBlinking dlib::dlib ${OpenCV_LIBS} )



# To compile this program all you need to do is ask cmake.  You would type
# these commands from within the directory containing this CMakeLists.txt
# file:
#   mkdir build
#   cd build
#   cmake ..
#   cmake --build . --config Release

