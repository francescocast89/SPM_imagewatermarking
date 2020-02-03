#!/bin/bash

#cd /home/spm18-castiglione/Project/

echo "--------Parallel Test with c++ Farm----------------"
echo "----------------400x300---------------------------"
for i in 1 2 4 6 8 10 12 16 20 32 64;
    do
		./imagewatermarking source/400x300/ source/watermark400x300.jpg result/ -par -nw $i
    done
echo "----------------800x600---------------------------"
for i in 1 2 4 6 8 10 12 16 20 32 64;
    do
		./imagewatermarking source/800x600/ source/watermark800x600.jpg result/ -par -nw $i
    done
echo "----------------1200x1600-------------------------"
for i in 1 2 4 6 8 10 12 16 20 32 64;
    do
		./imagewatermarking source/1600x1200/ source/watermark1600x1200.jpg result/ -par -nw $i
    done
echo "----------------2000x1500------------------------"
for i in 1 2 4 6 8 10 12 16 20 32 64;
    do
		./imagewatermarking source/2000x1500/ source/watermark2000x1500.jpg result/ -par -nw $i
    done  
