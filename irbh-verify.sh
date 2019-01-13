#!/bin/bash

bSize=$1
fName=$2
hName=$3

./irb-hash $bSize "$(cat $fName)" tmp.irbh

echo "Comparing hashes..."
if cmp --silent $hName tmp.irbh
then
  clear
  echo "Successfully verified file."
else
  clear
  echo "Failed to verify file."
fi

rm tmp.irbh