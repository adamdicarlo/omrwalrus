#!/bin/bash
cd ..
zip -9 -r AjdSpeedHack2007 `find omrwalrus/  -name \*.cpp -o -name \*.hpp -o -name \*.h -o -name Makefile -o -name \*.txt -o -name \*.bmp` `find omrwalrus/data/ -name \*.png -o -name \*.ttf -o -name \*.wav`
cd speedhack2007

