#!/bin/bash

for i in {1..500}
do
  python3 ./get-measures.py
  echo ${i}
  sleep 20m
done