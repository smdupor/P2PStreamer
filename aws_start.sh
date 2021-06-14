#!/bin/sh
ssh -i ~/Downloads/aws-nova-.pem ubuntu@3.80.238.204  '~/bin/Client a 152.7.98.255' &
ssh -i ~/Downloads/aws-syd.pem ubuntu@13.211.79.128 '~/bin/Client b 152.7.98.255' &
ssh -i ~/Downloads/aws-sao.pem ubuntu@18.229.118.66 '~/bin/Client c 152.7.98.255' &
ssh -i ~/Downloads/aws-paris.pem ubuntu@35.180.15.95 '~/bin/Client d 152.7.98.255' &
ssh -i ~/Downloads/aws-can.pem ubuntu@3.96.135.102 '~/bin/Client e 152.7.98.255' &
ssh -i ~/Downloads/aws-tok.pem ubuntu@52.194.241.98 '~/bin/Client f 152.7.98.255' &

