while [ 1 ]
do 
    echo -n blablub > /dev/tzm
    sleep 1
    echo blab > /dev/tzm
    sleep 2
done
