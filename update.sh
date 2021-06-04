#!/bin/sh

#mysql -uroot -proot -b test -e "LOAD DATA INFILE '/var/lib/mysql/sensor.csv' replace INTO TABLE sensor FIELDS TERMINATED BY ','"

num=1
FILE=/var/www/html/timesens/test$num.csv

if [ ! -f $FILE ]
then
	while [ $num -lt 10 ]
	do
		mysql -uroot -proot -b test -e "LOAD DATA INFILE '/var/lib/mysql/sensor$num.csv' replace INTO TABLE sensor$num FIELDS TERMINATED BY ','"
		num=$((num+1))
	done
fi

while [ -f $FILE ]
do
	sed 's/[][]//g' test$num.csv > sensor$num.csv
	rm test$num.csv
	rm /var/lib/mysql/sensor$num.csv
	cp sensor$num.csv /var/lib/mysql/
	mysql -uroot -proot -b test -e "LOAD DATA INFILE '/var/lib/mysql/sensor$num.csv' replace INTO TABLE sensor$num FIELDS TERMINATED BY ','"
	#echo "Sensor '$num' has values!"
	num=$((num+1))
	FILE=/var/www/html/timesens/test$num.csv
done
