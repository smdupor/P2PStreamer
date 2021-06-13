#!/bin/sh
ssh -i ~/Downloads/aws-nova-.pem ubuntu@18.207.166.4  '~/bin/Client a 152.7.98.255' &
ssh -i ~/Downloads/aws-syd.pem ubuntu@52.63.5.53 '~/bin/Client b 152.7.98.255' &
ssh -i ~/Downloads/aws-sao.pem ubuntu@18.228.22.164 '~/bin/Client c 152.7.98.255' &
ssh -i ~/Downloads/aws-paris.pem ubuntu@15.237.43.180 '~/bin/Client d 152.7.98.255' &
ssh -i ~/Downloads/aws-can.pem ubuntu@35.182.71.127 '~/bin/Client e 152.7.98.255' &
ssh -i ~/Downloads/aws-tok.pem ubuntu@13.231.114.47 '~/bin/Client f 152.7.98.255' &

