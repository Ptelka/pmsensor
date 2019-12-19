#!/bin/bash

for i in {1..5000}
do
  pkill -9 fim
  python3 ./get-measures.py || true
  python3 ./refresh.py || true
  echo ${i}
  fim -a current.png & sleep 3s
done


