#!/bin/sh
ssh -i ~/Downloads/aws-nova-.pem ubuntu@18.207.166.4  'rmdir log;mkdir logs' &
ssh -i ~/Downloads/aws-syd.pem ubuntu@52.63.5.53 'rmdir log;mkdir logs' &
ssh -i ~/Downloads/aws-sao.pem ubuntu@18.228.22.164 'rmdir log;mkdir logs' &
ssh -i ~/Downloads/aws-paris.pem ubuntu@15.237.43.180 'rmdir log;mkdir logs' &
ssh -i ~/Downloads/aws-can.pem ubuntu@35.182.71.127 'rmdir log;mkdir logs' &
ssh -i ~/Downloads/aws-tok.pem ubuntu@13.231.114.47 'rmdir log;mkdir logs' &

