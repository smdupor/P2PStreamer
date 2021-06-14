#!/bin/sh
NOVA=3.80.238.204
SYD=13.211.79.128
SAO=18.229.118.66
PAR=35.180.15.95
CAN=3.96.135.102
TOK=52.194.241.98
# Nova
#ssh -i ~/Downloads/aws-nova-.pem ubuntu@18.207.166.4 'mv P2PStreamer/* ~/'
scp -r -i ~/Downloads/aws-nova-.pem ubuntu@$NOVA:~/logs/a.csv ./logs/aws_b/

#Sydney
#ssh -i ~/Downloads/aws-syd.pem ubuntu@52.63.5.53 'mkdir log'
#scp -r -i ~/Downloads/aws-syd.pem ../P2PStreamer/bin/ ubuntu@52.63.5.53:~/
#scp -r -i ~/Downloads/aws-syd.pem ../P2PStreamer/Downloads/ ubuntu@52.63.5.53:~/
scp -r -i ~/Downloads/aws-syd.pem ubuntu@$SYD:~/logs/b.csv ./logs/aws_b/

#Sao
#ssh -i ~/Downloads/aws-sao.pem ubuntu@18.228.22.164 'mkdir log'
#scp -r -i ~/Downloads/aws-sao.pem ../P2PStreamer/bin/ ubuntu@18.228.22.164:~/
#scp -r -i ~/Downloads/aws-sao.pem ../P2PStreamer/Downloads/ ubuntu@18.228.22.164:~/
scp -r -i ~/Downloads/aws-sao.pem  ubuntu@$SAO:~/logs/c.csv ./logs/aws_b/

#Paris
#ssh -i ~/Downloads/aws-paris.pem ubuntu@15.237.43.180 'mkdir log'
#scp -r -i ~/Downloads/aws-paris.pem ../P2PStreamer/bin/ ubuntu@15.237.43.180:~/
#scp -r -i ~/Downloads/aws-paris.pem ../P2PStreamer/Downloads/ ubuntu@15.237.43.180:~/
scp -r -i ~/Downloads/aws-paris.pem ubuntu@$PAR:~/logs/d.csv ./logs/aws_b/

#Canada
#ssh -i ~/Downloads/aws-can.pem ubuntu@35.182.71.127 'mkdir log'
#scp -r -i ~/Downloads/aws-can.pem ../P2PStreamer/bin/ ubuntu@35.182.71.127:~/
#scp -r -i ~/Downloads/aws-can.pem ../P2PStreamer/Downloads/ ubuntu@35.182.71.127:~/
scp -r -i ~/Downloads/aws-can.pem  ubuntu@$CAN:~/logs/e.csv ./logs/aws_b/

#Tokyo
#ssh -i ~/Downloads/aws-tok.pem ubuntu@13.231.114.47 'mkdir log'
#scp -r -i ~/Downloads/aws-tok.pem ../P2PStreamer/bin/ ubuntu@13.231.114.47:~/
#scp -r -i ~/Downloads/aws-tok.pem ../P2PStreamer/Downloads/ ubuntu@13.231.114.47:~/
scp -r -i ~/Downloads/aws-tok.pem  ubuntu@$TOK:~/logs/f.csv ./logs/aws_b/

#ssh -i ~/Downloads/aws-nova-.pem ubuntu@18.207.166.4 'rm conf/*.conf; cp conf/experiment_a/* conf/' &
#ssh -i ~/Downloads/aws-syd.pem ubuntu@52.63.5.53 'rm conf/*.conf; cp conf/experiment_a/* conf/' &
#ssh -i ~/Downloads/aws-sao.pem ubuntu@18.228.22.164 'rm conf/*.conf; cp conf/experiment_a/* conf/' &
#ssh -i ~/Downloads/aws-paris.pem ubuntu@15.237.43.180  'rm conf/*.conf; cp conf/experiment_a/* conf/' &
#ssh -i ~/Downloads/aws-can.pem ubuntu@35.182.71.127 'rm conf/*.conf; cp conf/experiment_a/* conf/' &
#ssh -i ~/Downloads/aws-tok.pem ubuntu@13.231.114.47 'rm conf/*.conf; cp conf/experiment_a/* conf/' &