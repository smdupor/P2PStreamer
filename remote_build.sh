#!/bin/sh
ssh 152.7.98.255 "rm -rf ~/rmtbld && mkdir ~/rmtbld && exit"
scp -r ./* 152.7.98.255:~/rmtbld/
ssh 152.7.98.255 "cd rmtbld;make"
#ssh 152.7.98.115 "rm -rf ~/rmtbld && mkdir ~/rmtbld && exit"
#scp -r ./* 152.7.98.115:~/rmtbld/
#ssh 152.7.98.115 "cd rmtbld && make clean;make"
