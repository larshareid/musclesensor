#!/bin/sh

num=1
#mysql -uroot -proot -b test -e "SELECT * FROM sensor$num;"
mysql -uroot -proot -b test -e "LOAD DATA INFILE '/var/lib/mysql/sensor$num.csv' replace INTO TABLE sensor$num FIELDS TERMINATED BY ','"
mysql -uroot -proot -b test -e "SELECT * FROM sensor$num"
