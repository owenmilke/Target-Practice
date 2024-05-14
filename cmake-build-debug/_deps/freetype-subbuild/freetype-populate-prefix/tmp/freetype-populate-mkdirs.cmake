# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/Users/owen/CLionProjects/M4OEP-omilke/cmake-build-debug/_deps/freetype-src"
  "/Users/owen/CLionProjects/M4OEP-omilke/cmake-build-debug/_deps/freetype-build"
  "/Users/owen/CLionProjects/M4OEP-omilke/cmake-build-debug/_deps/freetype-subbuild/freetype-populate-prefix"
  "/Users/owen/CLionProjects/M4OEP-omilke/cmake-build-debug/_deps/freetype-subbuild/freetype-populate-prefix/tmp"
  "/Users/owen/CLionProjects/M4OEP-omilke/cmake-build-debug/_deps/freetype-subbuild/freetype-populate-prefix/src/freetype-populate-stamp"
  "/Users/owen/CLionProjects/M4OEP-omilke/cmake-build-debug/_deps/freetype-subbuild/freetype-populate-prefix/src"
  "/Users/owen/CLionProjects/M4OEP-omilke/cmake-build-debug/_deps/freetype-subbuild/freetype-populate-prefix/src/freetype-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/owen/CLionProjects/M4OEP-omilke/cmake-build-debug/_deps/freetype-subbuild/freetype-populate-prefix/src/freetype-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/Users/owen/CLionProjects/M4OEP-omilke/cmake-build-debug/_deps/freetype-subbuild/freetype-populate-prefix/src/freetype-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
