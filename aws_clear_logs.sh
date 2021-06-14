#!/bin/sh
ssh -i ~/Downloads/aws-nova-.pem ubuntu@3.80.238.204  'rm logs/*' &
ssh -i ~/Downloads/aws-syd.pem ubuntu@13.211.79.128 'rm logs/*' &
ssh -i ~/Downloads/aws-sao.pem ubuntu@18.229.118.66 'rm logs/*' &
ssh -i ~/Downloads/aws-paris.pem ubuntu@35.180.15.95 'rm logs/*' &
ssh -i ~/Downloads/aws-can.pem ubuntu@3.96.135.102 'rm logs/*' &
ssh -i ~/Downloads/aws-tok.pem ubuntu@52.194.241.98 'rm logs/*' &

