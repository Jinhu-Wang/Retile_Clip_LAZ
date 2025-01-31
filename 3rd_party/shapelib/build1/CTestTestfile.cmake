# CMake generated Testfile for 
# Source directory: /home/jinhu/Work/Projects/Jinhu-Wang/3rdParty/shapelib
# Build directory: /home/jinhu/Work/Projects/Jinhu-Wang/3rdParty/shapelib/build1
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(test2 "/usr/bin/bash" "/home/jinhu/Work/Projects/Jinhu-Wang/3rdParty/shapelib/build1/sed_scripted_test2.sh")
set_tests_properties(test2 PROPERTIES  _BACKTRACE_TRIPLES "/home/jinhu/Work/Projects/Jinhu-Wang/3rdParty/shapelib/CMakeLists.txt;271;add_test;/home/jinhu/Work/Projects/Jinhu-Wang/3rdParty/shapelib/CMakeLists.txt;0;")
add_test(test3 "/usr/bin/bash" "/home/jinhu/Work/Projects/Jinhu-Wang/3rdParty/shapelib/build1/sed_scripted_test3.sh")
set_tests_properties(test3 PROPERTIES  _BACKTRACE_TRIPLES "/home/jinhu/Work/Projects/Jinhu-Wang/3rdParty/shapelib/CMakeLists.txt;277;add_test;/home/jinhu/Work/Projects/Jinhu-Wang/3rdParty/shapelib/CMakeLists.txt;0;")
subdirs("cmake")
