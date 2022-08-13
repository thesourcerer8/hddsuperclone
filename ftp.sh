#!/bin/bash
HOST=your-host.com
USER=user@your-host.com
PASSWORD=your-password

sudo ip link set eth0 mtu 1492

cd license_files/public

ftp -inv $HOST <<EOF
user $USER $PASSWORD
mput *
del 0
bye
EOF