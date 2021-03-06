# Copyright (C) 2018, Gepard Graphics
# Copyright (C) 2018, Dániel Bátyai <dbatyai@inf.u-szeged.hu>
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice, this
#    list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
# ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

cmake_minimum_required(VERSION 2.8.10)
project(gepard-thirdparty)
set(PROJECT_VERSION 0.0.1)

# The outer build system will look in these folders for required files
set(BINARY_OUTPUT_DIR ${PROJECT_SOURCE_DIR}/bin)
set(LIB_OUTPUT_DIR ${PROJECT_SOURCE_DIR}/lib)
set(INCLUDE_OUTPUT_DIR ${PROJECT_SOURCE_DIR}/include)

# Dependencies are handled by the custom targets listed below this comment
# block. The build script will call these respective targets when building
# dependencies for a specific backend.
# The process of adding a new dependency should consist of creating a build
# step that is responsible for building and installing the library, and then
# adding this step as a dependency to the the custom target of the backend
# that requires this dependency.
#
# When creating the build step, the following general rules apply:
# - Source code should be fetched into a subdirectory of the 'src' directory.
# - Created output files should be installed into their respective directories
#   defined above this comment block.
# - A subdirectory in PROJECT_BINARY_DIR can be used as a build directory.

add_custom_target(common ALL
                  DEPENDS gtest)

add_custom_target(gles2 ALL
                  DEPENDS common)

add_custom_target(vulkan ALL
                  DEPENDS common
                          glslang)

add_custom_target(software ALL
                  DEPENDS common)

include(ExternalProject)

ExternalProject_Add(gtest
                    PREFIX ${PROJECT_SOURCE_DIR}/src/googletest
                    GIT_REPOSITORY https://github.com/google/googletest.git
                    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${PROJECT_SOURCE_DIR} -DCMAKE_INSTALL_LIBDIR=lib -DCMAKE_INSTALL_INCLUDEDIR=include -DBUILD_GMOCK=OFF
                    GIT_TAG release-1.8.1
                    BINARY_DIR ${PROJECT_BINARY_DIR}/googletest
                    BUILD_COMMAND make gtest
                    INSTALL_COMMAND make install > /dev/null)

ExternalProject_Add(glslang
                    PREFIX ${PROJECT_SOURCE_DIR}/src/glslang
                    GIT_REPOSITORY https://github.com/KhronosGroup/glslang.git
                    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${PROJECT_SOURCE_DIR} -DCMAKE_INSTALL_LIBDIR=lib -DCMAKE_INSTALL_INCLUDEDIR=include
                    GIT_TAG master-tot
                    BINARY_DIR ${PROJECT_BINARY_DIR}/glslang
                    BUILD_COMMAND make glslangValidator
                    INSTALL_COMMAND make install > /dev/null)

file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/stamps)
add_custom_command(TARGET gles2
                  POST_BUILD
                  COMMAND touch ${PROJECT_BINARY_DIR}/stamps/gles2.stamp)
add_custom_command(TARGET vulkan
                  POST_BUILD
                  COMMAND touch ${PROJECT_BINARY_DIR}/stamps/vulkan.stamp)
add_custom_command(TARGET software
                  POST_BUILD
                  COMMAND touch ${PROJECT_BINARY_DIR}/stamps/software.stamp)
