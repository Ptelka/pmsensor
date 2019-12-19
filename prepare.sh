#!/bin/bash

sudo apt-get update
sudo apt-get install python3-pip python3-scipy python3-pandas python3-seaborn sqlite3 -y

sqlite3 measures.db < create.sql
