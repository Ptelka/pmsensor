#!/bin/bash

while true
do
  if pgrep -x "run.sh" > /dev/null
  then
      echo "Running" > /dev/null
  else
      echo "Stopped rerunning"
      ./run.sh &
  fi
  sleep 5m
done