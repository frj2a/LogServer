#! /bin/bash
qmake "CONFIG+=debug_and_release" && make -j clean
