#!/bin/sh

#mysql -uroot -proot -b test -e "LOAD DATA INFILE '/var/lib/mysql/sensor.csv' replace INTO TABLE sensor FIELDS TERMINATED BY ','"

num=1
FILE=/var/www/html/nontime/sens1.csv

if [ ! -f $FILE ]
then
	while [ $num -lt 10 ]
	do
		mysql -uroot -proot -b test -e "LOAD DATA INFILE '/var/lib/mysql/sens$num.csv' replace INTO TABLE nonsensor$num FIELDS TERMINATED BY ','"
		num=$((num+1))
	done
fi

while [ -f $FILE ]
do
	rm /var/lib/mysql/sens$num.csv
	cp sens$num.csv /var/lib/mysql/
	mysql -uroot -proot -b test -e "LOAD DATA INFILE '/var/lib/mysql/sens$num.csv' replace INTO TABLE nonsensor$num FIELDS TERMINATED BY ','"
	#echo "Sensor '$num' has values!"
	num=$((num+1))
	FILE=/var/www/html/nontime/sen$num.csv
done
