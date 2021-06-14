#!/bin/sh
NOVA=54.198.62.225
SYD=54.252.252.222
SAO=18.231.39.50
PAR=35.181.1.140
CAN=3.98.58.52
TOK=3.112.33.64

SSH_CMD='rm -rf logs/*'

ssh -i ~/Downloads/aws-nova-.pem ubuntu@$NOVA $SSH_CMD &
ssh -i ~/Downloads/aws-syd.pem ubuntu@$SYD $SSH_CMD &
ssh -i ~/Downloads/aws-sao.pem ubuntu@$SAO $SSH_CMD &
ssh -i ~/Downloads/aws-paris.pem ubuntu@$PAR  $SSH_CMD &
ssh -i ~/Downloads/aws-can.pem ubuntu@$CAN $SSH_CMD &
ssh -i ~/Downloads/aws-tok.pem ubuntu@$TOK $SSH_CMD &
