# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/wtsaichu/esp/esp-idf/components/bootloader/subproject"
  "/home/wtsaichu/esp/esp-idf/examples/protocols/http_request/build/bootloader"
  "/home/wtsaichu/esp/esp-idf/examples/protocols/http_request/build/bootloader-prefix"
  "/home/wtsaichu/esp/esp-idf/examples/protocols/http_request/build/bootloader-prefix/tmp"
  "/home/wtsaichu/esp/esp-idf/examples/protocols/http_request/build/bootloader-prefix/src/bootloader-stamp"
  "/home/wtsaichu/esp/esp-idf/examples/protocols/http_request/build/bootloader-prefix/src"
  "/home/wtsaichu/esp/esp-idf/examples/protocols/http_request/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/wtsaichu/esp/esp-idf/examples/protocols/http_request/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/wtsaichu/esp/esp-idf/examples/protocols/http_request/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
