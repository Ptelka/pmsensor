#!/bin/bash

for i in {1..5000}
do
  python3 ./get-measures.py || true
  python3 ./refresh.py || true
  echo ${i}
  fim -a current.jpg & sleep 10s
done