#!/bin/sh

ps -eaf | grep 'nc -l' > pross.txt

x=1
val=$(awk -F ' ' 'NR=='$x' {print $2}' pross.txt)
echo $val

until [ -z $val ]
do
	kill -9 $val
	x=$((x+1))
	val=$(awk -F ' ' 'NR=='$x' {print $2}' pross.txt)
done


