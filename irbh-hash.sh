#!/bin/bash

bSize=$1
fName=$2
hName=$3

./irb-hash $bSize "$(cat $fName)" $hName