#!/usr/bin/env bash

while true
do
  python3 ./get-measures.py || true
  python3 ./refresh.py || true
  ./show-plot.sh & sleep 10m
done


