# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.13

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/herrygabriel/ALEDAPP/Analysis/WebcamAnalysis/CardiacFrequency

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/herrygabriel/ALEDAPP/Analysis/WebcamAnalysis/CardiacFrequency/build

# Include any dependencies generated for this target.
include CMakeFiles/CardiacFrequency.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/CardiacFrequency.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/CardiacFrequency.dir/flags.make

CMakeFiles/CardiacFrequency.dir/CardiacFrequency.cpp.o: CMakeFiles/CardiacFrequency.dir/flags.make
CMakeFiles/CardiacFrequency.dir/CardiacFrequency.cpp.o: ../CardiacFrequency.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/herrygabriel/ALEDAPP/Analysis/WebcamAnalysis/CardiacFrequency/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/CardiacFrequency.dir/CardiacFrequency.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/CardiacFrequency.dir/CardiacFrequency.cpp.o -c /home/herrygabriel/ALEDAPP/Analysis/WebcamAnalysis/CardiacFrequency/CardiacFrequency.cpp

CMakeFiles/CardiacFrequency.dir/CardiacFrequency.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/CardiacFrequency.dir/CardiacFrequency.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/herrygabriel/ALEDAPP/Analysis/WebcamAnalysis/CardiacFrequency/CardiacFrequency.cpp > CMakeFiles/CardiacFrequency.dir/CardiacFrequency.cpp.i

CMakeFiles/CardiacFrequency.dir/CardiacFrequency.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/CardiacFrequency.dir/CardiacFrequency.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/herrygabriel/ALEDAPP/Analysis/WebcamAnalysis/CardiacFrequency/CardiacFrequency.cpp -o CMakeFiles/CardiacFrequency.dir/CardiacFrequency.cpp.s

# Object files for target CardiacFrequency
CardiacFrequency_OBJECTS = \
"CMakeFiles/CardiacFrequency.dir/CardiacFrequency.cpp.o"

# External object files for target CardiacFrequency
CardiacFrequency_EXTERNAL_OBJECTS =

CardiacFrequency: CMakeFiles/CardiacFrequency.dir/CardiacFrequency.cpp.o
CardiacFrequency: CMakeFiles/CardiacFrequency.dir/build.make
CardiacFrequency: dlib_build/libdlib.a
CardiacFrequency: /usr/local/lib/libopencv_gapi.so.4.1.2
CardiacFrequency: /usr/local/lib/libopencv_stitching.so.4.1.2
CardiacFrequency: /usr/local/lib/libopencv_aruco.so.4.1.2
CardiacFrequency: /usr/local/lib/libopencv_bgsegm.so.4.1.2
CardiacFrequency: /usr/local/lib/libopencv_bioinspired.so.4.1.2
CardiacFrequency: /usr/local/lib/libopencv_ccalib.so.4.1.2
CardiacFrequency: /usr/local/lib/libopencv_dnn_objdetect.so.4.1.2
CardiacFrequency: /usr/local/lib/libopencv_dnn_superres.so.4.1.2
CardiacFrequency: /usr/local/lib/libopencv_dpm.so.4.1.2
CardiacFrequency: /usr/local/lib/libopencv_face.so.4.1.2
CardiacFrequency: /usr/local/lib/libopencv_freetype.so.4.1.2
CardiacFrequency: /usr/local/lib/libopencv_fuzzy.so.4.1.2
CardiacFrequency: /usr/local/lib/libopencv_hfs.so.4.1.2
CardiacFrequency: /usr/local/lib/libopencv_img_hash.so.4.1.2
CardiacFrequency: /usr/local/lib/libopencv_line_descriptor.so.4.1.2
CardiacFrequency: /usr/local/lib/libopencv_quality.so.4.1.2
CardiacFrequency: /usr/local/lib/libopencv_reg.so.4.1.2
CardiacFrequency: /usr/local/lib/libopencv_rgbd.so.4.1.2
CardiacFrequency: /usr/local/lib/libopencv_saliency.so.4.1.2
CardiacFrequency: /usr/local/lib/libopencv_stereo.so.4.1.2
CardiacFrequency: /usr/local/lib/libopencv_structured_light.so.4.1.2
CardiacFrequency: /usr/local/lib/libopencv_superres.so.4.1.2
CardiacFrequency: /usr/local/lib/libopencv_surface_matching.so.4.1.2
CardiacFrequency: /usr/local/lib/libopencv_tracking.so.4.1.2
CardiacFrequency: /usr/local/lib/libopencv_videostab.so.4.1.2
CardiacFrequency: /usr/local/lib/libopencv_xfeatures2d.so.4.1.2
CardiacFrequency: /usr/local/lib/libopencv_xobjdetect.so.4.1.2
CardiacFrequency: /usr/local/lib/libopencv_xphoto.so.4.1.2
CardiacFrequency: /usr/lib/x86_64-linux-gnu/libSM.so
CardiacFrequency: /usr/lib/x86_64-linux-gnu/libICE.so
CardiacFrequency: /usr/lib/x86_64-linux-gnu/libX11.so
CardiacFrequency: /usr/lib/x86_64-linux-gnu/libXext.so
CardiacFrequency: /usr/lib/x86_64-linux-gnu/libpng.so
CardiacFrequency: /usr/lib/x86_64-linux-gnu/libz.so
CardiacFrequency: /usr/lib/x86_64-linux-gnu/libjpeg.so
CardiacFrequency: /usr/lib/x86_64-linux-gnu/libcblas.so
CardiacFrequency: /usr/lib/x86_64-linux-gnu/liblapack.so
CardiacFrequency: /usr/local/lib/libopencv_shape.so.4.1.2
CardiacFrequency: /usr/local/lib/libopencv_highgui.so.4.1.2
CardiacFrequency: /usr/local/lib/libopencv_datasets.so.4.1.2
CardiacFrequency: /usr/local/lib/libopencv_plot.so.4.1.2
CardiacFrequency: /usr/local/lib/libopencv_text.so.4.1.2
CardiacFrequency: /usr/local/lib/libopencv_dnn.so.4.1.2
CardiacFrequency: /usr/local/lib/libopencv_ml.so.4.1.2
CardiacFrequency: /usr/local/lib/libopencv_phase_unwrapping.so.4.1.2
CardiacFrequency: /usr/local/lib/libopencv_optflow.so.4.1.2
CardiacFrequency: /usr/local/lib/libopencv_ximgproc.so.4.1.2
CardiacFrequency: /usr/local/lib/libopencv_video.so.4.1.2
CardiacFrequency: /usr/local/lib/libopencv_videoio.so.4.1.2
CardiacFrequency: /usr/local/lib/libopencv_imgcodecs.so.4.1.2
CardiacFrequency: /usr/local/lib/libopencv_objdetect.so.4.1.2
CardiacFrequency: /usr/local/lib/libopencv_calib3d.so.4.1.2
CardiacFrequency: /usr/local/lib/libopencv_features2d.so.4.1.2
CardiacFrequency: /usr/local/lib/libopencv_flann.so.4.1.2
CardiacFrequency: /usr/local/lib/libopencv_photo.so.4.1.2
CardiacFrequency: /usr/local/lib/libopencv_imgproc.so.4.1.2
CardiacFrequency: /usr/local/lib/libopencv_core.so.4.1.2
CardiacFrequency: CMakeFiles/CardiacFrequency.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/herrygabriel/ALEDAPP/Analysis/WebcamAnalysis/CardiacFrequency/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable CardiacFrequency"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/CardiacFrequency.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/CardiacFrequency.dir/build: CardiacFrequency

.PHONY : CMakeFiles/CardiacFrequency.dir/build

CMakeFiles/CardiacFrequency.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/CardiacFrequency.dir/cmake_clean.cmake
.PHONY : CMakeFiles/CardiacFrequency.dir/clean

CMakeFiles/CardiacFrequency.dir/depend:
	cd /home/herrygabriel/ALEDAPP/Analysis/WebcamAnalysis/CardiacFrequency/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/herrygabriel/ALEDAPP/Analysis/WebcamAnalysis/CardiacFrequency /home/herrygabriel/ALEDAPP/Analysis/WebcamAnalysis/CardiacFrequency /home/herrygabriel/ALEDAPP/Analysis/WebcamAnalysis/CardiacFrequency/build /home/herrygabriel/ALEDAPP/Analysis/WebcamAnalysis/CardiacFrequency/build /home/herrygabriel/ALEDAPP/Analysis/WebcamAnalysis/CardiacFrequency/build/CMakeFiles/CardiacFrequency.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/CardiacFrequency.dir/depend
