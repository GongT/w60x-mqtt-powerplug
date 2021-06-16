# socat tcp-listen:8888,reuseaddr PTY,crtscts=1,raw,echo=0,link=/dev/ttyUSB0

# socat PTY,crtscts=1,raw,echo=0,link=COM3 tcp:work.gongt.me.normal:8888
# socat -d -d -d /dev/ttyS3,crtscts=0,rawer,echo=0 tcp:work.gongt.me.normal:23000,reuseaddr,fork

socat tcp-listen:23000,fork PTY,link=/dev/ttyUSB0,rawer,wait-slave
