= Procedure to test the USB bandwidth.

linux-serial-test-host.c implements the host->gadget data_rate only.

Requirements:
* host = PC, gadget = zkit-vf51 board from zilogic systems.
* insert g_serial module in gadget and after insertion, /dev/ttyGS0 node will be seen in zkit board. /dev/ttyACM0 will be seen in PC.

== Inside vybrid  
        stty -F /dev/ttyGS0 -icanon
        cat /dev/ttyGS0

==  Inside PC
        compile linux-serial-test-host.c and run
        It prints the Data rate obtained based on data in data.h header file.


The maximum transmission data rate obtained is 10MBps and average around 3.5MBps.

The code linux-serial-test-host.c is developed based on reference https://github.com/cbrake/linux-serial-test
