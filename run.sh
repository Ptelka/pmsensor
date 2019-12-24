#!/usr/bin/env bash

while true
do
  python3 ./get-measures.py
  python3 ./refresh.py
  ./show-plot.sh &
  sleep 5m
done


