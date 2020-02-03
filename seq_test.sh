#!/bin/bash

#cd /home/spm18-castiglione/Project/
echo "--------------Sequential Test -------------------"
echo "-----------------400x300-------------------------"
./imagewatermarking source/400x300/ source/watermark400x300.jpg result/ -seq
echo "-----------------800x600-------------------------"
./imagewatermarking source/800x600/ source/watermark800x600.jpg result/ -seq
echo "-----------------1600x1200-----------------------"
./imagewatermarking source/1600x1200/ source/watermark1600x1200.jpg result/ -seq
echo "-----------------2000x1500-----------------------"
./imagewatermarking source/2000x1500/ source/watermark2000x1500.jpg result/ -seq
