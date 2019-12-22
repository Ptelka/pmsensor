#!/usr/bin/env bash

pkill -9 fim
convert -resize current.png 480x320 current.png
fim -a current.png & sleep 1s