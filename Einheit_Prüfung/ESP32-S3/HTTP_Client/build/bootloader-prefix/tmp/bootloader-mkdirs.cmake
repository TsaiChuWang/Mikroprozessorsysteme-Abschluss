# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/wtsaichu/esp/esp-idf/components/bootloader/subproject"
  "/home/wtsaichu/Dokumente/Arbeitsplatz/Mikroprozessorsysteme-Abschluss/Einheit_Prüfung/ESP32-S3/HTTP_Get/build/bootloader"
  "/home/wtsaichu/Dokumente/Arbeitsplatz/Mikroprozessorsysteme-Abschluss/Einheit_Prüfung/ESP32-S3/HTTP_Get/build/bootloader-prefix"
  "/home/wtsaichu/Dokumente/Arbeitsplatz/Mikroprozessorsysteme-Abschluss/Einheit_Prüfung/ESP32-S3/HTTP_Get/build/bootloader-prefix/tmp"
  "/home/wtsaichu/Dokumente/Arbeitsplatz/Mikroprozessorsysteme-Abschluss/Einheit_Prüfung/ESP32-S3/HTTP_Get/build/bootloader-prefix/src/bootloader-stamp"
  "/home/wtsaichu/Dokumente/Arbeitsplatz/Mikroprozessorsysteme-Abschluss/Einheit_Prüfung/ESP32-S3/HTTP_Get/build/bootloader-prefix/src"
  "/home/wtsaichu/Dokumente/Arbeitsplatz/Mikroprozessorsysteme-Abschluss/Einheit_Prüfung/ESP32-S3/HTTP_Get/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/wtsaichu/Dokumente/Arbeitsplatz/Mikroprozessorsysteme-Abschluss/Einheit_Prüfung/ESP32-S3/HTTP_Get/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/wtsaichu/Dokumente/Arbeitsplatz/Mikroprozessorsysteme-Abschluss/Einheit_Prüfung/ESP32-S3/HTTP_Get/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
