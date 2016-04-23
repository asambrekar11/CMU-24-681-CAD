cmake_minimum_required(VERSION 3.0)


#Cmake is supposed to set these two variables.  However, it doesn't.
#Looks to be something to do with variable cache.  Variable cache is one of the design failures of cmake.
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/exe)


# If ysgl source files may not be placed under ${CMAKE_CURRENT_SOURCE_DIR}/src/ysgl/src, include_directories must be added here.
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/imported/include)


# Capture global-YS_LIBRARY_LIST into local YS_LIBRARY_LIST
set(YS_LIBRARY_LIST ${YS_LIBRARY_LIST})

add_subdirectory("ysclass/src")
add_subdirectory("ysclass11/src")
add_subdirectory("ysport/src")

add_subdirectory("ysgl/src")
add_subdirectory("ysgl/test")
add_subdirectory("ysglcpp/src")
add_subdirectory("ysglcpp/src/gl1")
add_subdirectory("ysglcpp/src/gl2")
add_subdirectory("ysglcpp/src/nownd")

add_subdirectory("ysbitmap/src")
add_subdirectory("ysbitmap/cmdtools")
add_subdirectory("ysbitmaputil/src")
add_subdirectory("ysbitmapfont/src")
add_subdirectory("ysbitmapfont/cmdtools")
add_subdirectory("yssystemfont/src")

add_subdirectory("fssimplewindow/src")
add_subdirectory("fssimplewindow/template")
add_subdirectory("fssimplewindow/samples")
add_subdirectory("fslazywindow/src")
add_subdirectory("fslazywindow/template")
add_subdirectory("fslazywindow/samples/sample00-bouncingBall")

add_subdirectory("ysfontrenderer/src")

add_subdirectory("fsguilib/src")
add_subdirectory("fsguilib/src/gl1")
add_subdirectory("fsguilib/src/gl2")
add_subdirectory("fsguilib/src/nownd")
add_subdirectory("fsguilib/filedialog")
add_subdirectory("fsgui3d/src")
add_subdirectory("fsgui3d/src/gl1")
add_subdirectory("fsgui3d/src/gl2")
add_subdirectory("fsgui3d/template")
add_subdirectory("fsgui3d/template-easy")

add_subdirectory("ysgebl/src/kernel")
add_subdirectory("ysgebl/src/kernelutil")
add_subdirectory("ysgebl/src/shellrender")
add_subdirectory("ysgebl/src/shellrender/gl1")
add_subdirectory("ysgebl/src/shellrender/gl2")
add_subdirectory("ysgebl/src/shellrender/nownd")
add_subdirectory("ysgebl/src/gui_foundation")
add_subdirectory("ysgebl/src/gui")
add_subdirectory("ysgebl/src/main")
add_subdirectory("ysgebl/src/cmdlib")
add_subdirectory("ysgebl/src/cmdmain")
add_subdirectory("ysgebl/src/samples")

add_subdirectory("test")


# Export include paths
foreach(ONE_LIBRARY ${YS_LIBRARY_LIST})
set(${ONE_LIBRARY}_INCLUDE_DIR ${${ONE_LIBRARY}_INCLUDE_DIR} PARENT_SCOPE)
endforeach(ONE_LIBRARY)

# Copy local-YS_LIBRARY_LIST back to global-YS_LIBRARY_LIST
set(YS_LIBRARY_LIST ${YS_LIBRARY_LIST} PARENT_SCOPE)
