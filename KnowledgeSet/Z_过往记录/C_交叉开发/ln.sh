#########################################################################
# File Name: ln.sh
# Author: HMW
# mail: xxx.xxx.com
# Created Time: Tue 09 Jul 2019 10:17:05 AM CST
#########################################################################
#!/bin/bash

var3=`ls arm-none-*`
#echo $var3

for file in $var3
do
	#echo $file
	var1=${file##*gnueabi-}
	echo $var1
	var2=arm-linux-$var1
	echo $var2
	ln -s $file $var2
done



