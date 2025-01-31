#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "shapelib::shp" for configuration "Release"
set_property(TARGET shapelib::shp APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(shapelib::shp PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libshp.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS shapelib::shp )
list(APPEND _IMPORT_CHECK_FILES_FOR_shapelib::shp "${_IMPORT_PREFIX}/lib/libshp.a" )

# Import target "shapelib::shpcreate" for configuration "Release"
set_property(TARGET shapelib::shpcreate APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(shapelib::shpcreate PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/shpcreate"
  )

list(APPEND _IMPORT_CHECK_TARGETS shapelib::shpcreate )
list(APPEND _IMPORT_CHECK_FILES_FOR_shapelib::shpcreate "${_IMPORT_PREFIX}/bin/shpcreate" )

# Import target "shapelib::shpadd" for configuration "Release"
set_property(TARGET shapelib::shpadd APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(shapelib::shpadd PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/shpadd"
  )

list(APPEND _IMPORT_CHECK_TARGETS shapelib::shpadd )
list(APPEND _IMPORT_CHECK_FILES_FOR_shapelib::shpadd "${_IMPORT_PREFIX}/bin/shpadd" )

# Import target "shapelib::shpdump" for configuration "Release"
set_property(TARGET shapelib::shpdump APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(shapelib::shpdump PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/shpdump"
  )

list(APPEND _IMPORT_CHECK_TARGETS shapelib::shpdump )
list(APPEND _IMPORT_CHECK_FILES_FOR_shapelib::shpdump "${_IMPORT_PREFIX}/bin/shpdump" )

# Import target "shapelib::shprewind" for configuration "Release"
set_property(TARGET shapelib::shprewind APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(shapelib::shprewind PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/shprewind"
  )

list(APPEND _IMPORT_CHECK_TARGETS shapelib::shprewind )
list(APPEND _IMPORT_CHECK_FILES_FOR_shapelib::shprewind "${_IMPORT_PREFIX}/bin/shprewind" )

# Import target "shapelib::dbfcreate" for configuration "Release"
set_property(TARGET shapelib::dbfcreate APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(shapelib::dbfcreate PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/dbfcreate"
  )

list(APPEND _IMPORT_CHECK_TARGETS shapelib::dbfcreate )
list(APPEND _IMPORT_CHECK_FILES_FOR_shapelib::dbfcreate "${_IMPORT_PREFIX}/bin/dbfcreate" )

# Import target "shapelib::dbfadd" for configuration "Release"
set_property(TARGET shapelib::dbfadd APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(shapelib::dbfadd PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/dbfadd"
  )

list(APPEND _IMPORT_CHECK_TARGETS shapelib::dbfadd )
list(APPEND _IMPORT_CHECK_FILES_FOR_shapelib::dbfadd "${_IMPORT_PREFIX}/bin/dbfadd" )

# Import target "shapelib::dbfdump" for configuration "Release"
set_property(TARGET shapelib::dbfdump APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(shapelib::dbfdump PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/dbfdump"
  )

list(APPEND _IMPORT_CHECK_TARGETS shapelib::dbfdump )
list(APPEND _IMPORT_CHECK_FILES_FOR_shapelib::dbfdump "${_IMPORT_PREFIX}/bin/dbfdump" )

# Import target "shapelib::shptreedump" for configuration "Release"
set_property(TARGET shapelib::shptreedump APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(shapelib::shptreedump PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/shptreedump"
  )

list(APPEND _IMPORT_CHECK_TARGETS shapelib::shptreedump )
list(APPEND _IMPORT_CHECK_FILES_FOR_shapelib::shptreedump "${_IMPORT_PREFIX}/bin/shptreedump" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
