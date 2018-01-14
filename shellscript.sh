if [ $(lsmod | grep "tzm" -c) -eq 1 ]; then
	rm /dev/tzm
	rmmod tzm
fi


insmod tzm.ko
majorNr=$(grep $*tzm /proc/devices | cut -d ' ' -f 1)
mknod /dev/tzm c $majorNr 0 

chmod a+rw /dev/tzm