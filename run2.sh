#!/bin/bash
# Run the game without Aqir Loader
export LD_LIBRARY_PATH=`pwd | sed 's/ /\\ /g'`:${LD_LIBRARY_PATH}
export LD_PRELOAD="aqir.so"
wine Wow-64 -opengl
