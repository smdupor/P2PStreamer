#!/bin/sh
NOVA=54.198.62.225
SYD=54.252.252.222
SAO=18.231.39.50
PAR=35.181.1.140
CAN=3.98.58.52
TOK=3.112.33.64

ssh -i ~/Downloads/aws-nova-.pem ubuntu@$NOVA  '~/bin/Client a 152.7.98.255' &
ssh -i ~/Downloads/aws-syd.pem ubuntu@$SYD '~/bin/Client b 152.7.98.255' &
ssh -i ~/Downloads/aws-sao.pem ubuntu@$SAO '~/bin/Client c 152.7.98.255' &
ssh -i ~/Downloads/aws-paris.pem ubuntu@$PAR '~/bin/Client d 152.7.98.255' &
ssh -i ~/Downloads/aws-can.pem ubuntu@$CAN '~/bin/Client e 152.7.98.255' &
ssh -i ~/Downloads/aws-tok.pem ubuntu@$TOK '~/bin/Client f 152.7.98.255' &

