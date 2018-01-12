rm /dev/tzm
rmmod tzm
insmod tzm.ko
majorNr=$(grep $*tzm /proc/devices | cut -d ' ' -f 1)
sudo mknod /dev/tzm c 249 0 
