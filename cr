#!/bin/bash
clear 
echo "Cleaning..."
make clean
make build_bytecode
make run