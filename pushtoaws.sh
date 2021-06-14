#!/bin/sh
NOVA=3.80.238.204
SYD=13.211.79.128
SAO=18.229.118.66
PAR=35.180.15.95
CAN=3.96.135.102
TOK=52.194.241.98

# Nova
#ssh -i ~/Downloads/aws-nova-.pem ubuntu@18.207.166.4 'mv P2PStreamer/* ~/'
scp -r -i ~/Downloads/aws-nova-.pem ../P2PStreamer/bin/ ubuntu@$NOVA:~/
scp -r -i ~/Downloads/aws-nova-.pem ../P2PStreamer/areset.sh ubuntu@$NOVA:~/
scp -r -i ~/Downloads/aws-nova-.pem ../P2PStreamer/breset.sh ubuntu@$NOVA:~/

#Sydney
#ssh -i ~/Downloads/aws-syd.pem ubuntu@52.63.5.53 'mkdir log'
scp -r -i ~/Downloads/aws-syd.pem ../P2PStreamer/bin/ ubuntu@$SYD:~/
scp -r -i ~/Downloads/aws-syd.pem ../P2PStreamer/areset.sh ubuntu@$SYD:~/
scp -r -i ~/Downloads/aws-syd.pem ../P2PStreamer/breset.sh ubuntu@$SYD:~/
#scp -r -i ~/Downloads/aws-syd.pem ../P2PStreamer/Downloads/ ubuntu@52.63.5.53:~/
#scp -r -i ~/Downloads/aws-syd.pem ../P2PStreamer/conf/ ubuntu@52.63.5.53:~/

#Sao
#ssh -i ~/Downloads/aws-sao.pem ubuntu@18.228.22.164 'mkdir log'
scp -r -i ~/Downloads/aws-sao.pem ../P2PStreamer/bin/ ubuntu@$SAO:~/
scp -r -i ~/Downloads/aws-sao.pem ../P2PStreamer/areset.sh ubuntu@$SAO:~/
scp -r -i ~/Downloads/aws-sao.pem ../P2PStreamer/breset.sh ubuntu@$SAO:~/
#scp -r -i ~/Downloads/aws-sao.pem ../P2PStreamer/Downloads/ ubuntu@18.228.22.164:~/
#scp -r -i ~/Downloads/aws-sao.pem ../P2PStreamer/conf/ ubuntu@18.228.22.164:~/

#Paris
#ssh -i ~/Downloads/aws-paris.pem ubuntu@15.237.43.180 'mkdir log'
scp -r -i ~/Downloads/aws-paris.pem ../P2PStreamer/bin/ ubuntu@$PAR:~/
scp -r -i ~/Downloads/aws-paris.pem ../P2PStreamer/areset.sh ubuntu@$PAR:~/
scp -r -i ~/Downloads/aws-paris.pem ../P2PStreamer/breset.sh ubuntu@$PAR:~/
#scp -r -i ~/Downloads/aws-paris.pem ../P2PStreamer/Downloads/ ubuntu@15.237.43.180:~/
#scp -r -i ~/Downloads/aws-paris.pem ../P2PStreamer/conf/ ubuntu@15.237.43.180:~/

#Canada
#ssh -i ~/Downloads/aws-can.pem ubuntu@35.182.71.127 'mkdir log'
scp -r -i ~/Downloads/aws-can.pem ../P2PStreamer/bin/ ubuntu@$CAN:~/
scp -r -i ~/Downloads/aws-can.pem ../P2PStreamer/areset.sh ubuntu@$CAN:~/
scp -r -i ~/Downloads/aws-can.pem ../P2PStreamer/breset.sh ubuntu@$CAN:~/
#scp -r -i ~/Downloads/aws-can.pem ../P2PStreamer/Downloads/ ubuntu@35.182.71.127:~/
#scp -r -i ~/Downloads/aws-can.pem ../P2PStreamer/conf/ ubuntu@35.182.71.127:~/

#Tokyo
#ssh -i ~/Downloads/aws-tok.pem ubuntu@13.231.114.47 'mkdir log'
scp -r -i ~/Downloads/aws-tok.pem ../P2PStreamer/bin/ ubuntu@$TOK:~/
scp -r -i ~/Downloads/aws-tok.pem ../P2PStreamer/areset.sh ubuntu@$TOK:~/
scp -r -i ~/Downloads/aws-tok.pem ../P2PStreamer/breset.sh ubuntu@$TOK:~/
#scp -r -i ~/Downloads/aws-tok.pem ../P2PStreamer/Downloads/ ubuntu@13.231.114.47:~/
#scp -r -i ~/Downloads/aws-tok.pem ../P2PStreamer/conf/ ubuntu@13.231.114.47:~/

ssh -i ~/Downloads/aws-nova-.pem ubuntu@$NOVA './breset.sh' &
ssh -i ~/Downloads/aws-syd.pem ubuntu@$SYD './breset.sh' &
ssh -i ~/Downloads/aws-sao.pem ubuntu@$SAO './breset.sh' &
ssh -i ~/Downloads/aws-paris.pem ubuntu@$PAR  './breset.sh' &
ssh -i ~/Downloads/aws-can.pem ubuntu@$CAN './breset.sh' &
ssh -i ~/Downloads/aws-tok.pem ubuntu@$TOK './breset.sh' &