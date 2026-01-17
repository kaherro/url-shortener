# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/kahero/Desktop/url-shortener/build/_deps/sqlite-src"
  "/home/kahero/Desktop/url-shortener/build/_deps/sqlite-build"
  "/home/kahero/Desktop/url-shortener/build/_deps/sqlite-subbuild/sqlite-populate-prefix"
  "/home/kahero/Desktop/url-shortener/build/_deps/sqlite-subbuild/sqlite-populate-prefix/tmp"
  "/home/kahero/Desktop/url-shortener/build/_deps/sqlite-subbuild/sqlite-populate-prefix/src/sqlite-populate-stamp"
  "/home/kahero/Desktop/url-shortener/build/_deps/sqlite-subbuild/sqlite-populate-prefix/src"
  "/home/kahero/Desktop/url-shortener/build/_deps/sqlite-subbuild/sqlite-populate-prefix/src/sqlite-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/kahero/Desktop/url-shortener/build/_deps/sqlite-subbuild/sqlite-populate-prefix/src/sqlite-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/kahero/Desktop/url-shortener/build/_deps/sqlite-subbuild/sqlite-populate-prefix/src/sqlite-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
