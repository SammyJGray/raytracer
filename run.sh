#!/bin/bash
clear
cd .build && make
cd .. && bin/main
pkill -f "eog"
xdg-open images/image.ppm
