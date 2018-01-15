PARAM_TIME=2
PARAM_NUMBER=2

DEVICE_NAME="tzm"

if [ $(lsmod | grep $DEVICE_NAME -c) -eq 1 ]; then
	rm /dev/$DEVICE_NAME
	echo Old device-node removed
	rmmod $DEVICE_NAME
	echo Old module removed
fi


insmod tzm.ko ret_val_time=$PARAM_TIME ret_val_number=$PARAM_NUMBER
echo New module inserted...parameters: time=$PARAM_TIME, number=$PARAM_NUMBER
MAJOR_NR=$(grep $*$DEVICE_NAME /proc/devices | cut -d ' ' -f 1)
echo MajorNr: $MAJOR_NR
mknod /dev/$DEVICE_NAME c $MAJOR_NR 0 
echo New node created
chmod a+rw /dev/$DEVICE_NAME
echo give read and write access to all
