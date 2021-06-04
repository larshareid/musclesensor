#!/bin/sh

num=0

while [ $num -lt $1 ]
do
	num=$((num+1))
	nc -l -u 400$num > sens$num.csv &
done
