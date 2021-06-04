#!/bin/sh

num=1
while [ $num -lt 10 ]
do
	mysql -uroot -proot -b test -e "DELETE FROM sensor$num;"
	num=$((num+1))
done
