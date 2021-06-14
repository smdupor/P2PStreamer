#!/bin/sh
cd ~/CLionProjects/P2PStreamer;./bin/Client a 192.168.1.109 &./bin/Client b 192.168.1.109 &
ssh 192.168.1.145 'cd ~/P2PStreamer;./bin/Client c 192.168.1.109' &
ssh 192.168.1.145 'cd ~/P2PStreamer;./bin/Client d 192.168.1.109' &
ssh 192.168.1.31 'cd ~/P2PStreamer;./bin/Client e 192.168.1.109' &
ssh 192.168.1.31 'cd ~/P2PStreamer;./bin/Client f 192.168.1.109' &
