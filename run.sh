#!/bin/bash
# Run the game with Aqir Loader
export LD_LIBRARY_PATH=`pwd | sed 's/ /\\ /g'`:${LD_LIBRARY_PATH}
export LD_PRELOAD="aqirl.so"
wine Wow-64 -opengl
