#!/bin/sh
NOVA=54.198.62.225
SYD=54.252.252.222
SAO=18.231.39.50
PAR=35.181.1.140
CAN=3.98.58.52
TOK=3.112.33.64

#ssh -i ~/Downloads/aws-nova-.pem ubuntu@$NOVA SSH_CMD='md5sum Downloads/Downloads_A/* > logs/md5A'
#ssh -i ~/Downloads/aws-syd.pem ubuntu@$SYD SSH_CMD='md5sum Downloads/Downloads_B/* > logs/md5B'
#ssh -i ~/Downloads/aws-sao.pem ubuntu@$SAO SSH_CMD='md5sum Downloads/Downloads_C/* > logs/md5C'
#ssh -i ~/Downloads/aws-paris.pem ubuntu@$PAR  SSH_CMD='md5sum Downloads/Downloads_D/* > logs/md5D'
#ssh -i ~/Downloads/aws-can.pem ubuntu@$CAN SSH_CMD='md5sum Downloads/Downloads_E/* > logs/md5E'
#ssh -i ~/Downloads/aws-tok.pem ubuntu@$TOK SSH_CMD='md5sum Downloads/Downloads_F/* > logs/md5F'

# Nova
#ssh -i ~/Downloads/aws-nova-.pem ubuntu@18.207.166.4 'mv P2PStreamer/* ~/'
scp -r -i ~/Downloads/aws-nova-.pem ubuntu@$NOVA:~/logs/* ./logs/aws_b2/3/

#Sydney
#ssh -i ~/Downloads/aws-syd.pem ubuntu@52.63.5.53 'mkdir log'
#scp -r -i ~/Downloads/aws-syd.pem ../P2PStreamer/bin/ ubuntu@52.63.5.53:~/
#scp -r -i ~/Downloads/aws-syd.pem ../P2PStreamer/Downloads/ ubuntu@52.63.5.53:~/
scp -r -i ~/Downloads/aws-syd.pem ubuntu@$SYD:~/logs/* ./logs/aws_b2/3/

#Sao
#ssh -i ~/Downloads/aws-sao.pem ubuntu@18.228.22.164 'mkdir log'
#scp -r -i ~/Downloads/aws-sao.pem ../P2PStreamer/bin/ ubuntu@18.228.22.164:~/
#scp -r -i ~/Downloads/aws-sao.pem ../P2PStreamer/Downloads/ ubuntu@18.228.22.164:~/
scp -r -i ~/Downloads/aws-sao.pem  ubuntu@$SAO:~/logs/* ./logs/aws_b2/3/

#Paris
#ssh -i ~/Downloads/aws-paris.pem ubuntu@15.237.43.180 'mkdir log'
#scp -r -i ~/Downloads/aws-paris.pem ../P2PStreamer/bin/ ubuntu@15.237.43.180:~/
#scp -r -i ~/Downloads/aws-paris.pem ../P2PStreamer/Downloads/ ubuntu@15.237.43.180:~/
scp -r -i ~/Downloads/aws-paris.pem ubuntu@$PAR:~/logs/* ./logs/aws_b2/3/

#Canada
#ssh -i ~/Downloads/aws-can.pem ubuntu@35.182.71.127 'mkdir log'
#scp -r -i ~/Downloads/aws-can.pem ../P2PStreamer/bin/ ubuntu@35.182.71.127:~/
#scp -r -i ~/Downloads/aws-can.pem ../P2PStreamer/Downloads/ ubuntu@35.182.71.127:~/
scp -r -i ~/Downloads/aws-can.pem  ubuntu@$CAN:~/logs/* ./logs/aws_b2/3/

#Tokyo
#ssh -i ~/Downloads/aws-tok.pem ubuntu@13.231.114.47 'mkdir log'
#scp -r -i ~/Downloads/aws-tok.pem ../P2PStreamer/bin/ ubuntu@13.231.114.47:~/
#scp -r -i ~/Downloads/aws-tok.pem ../P2PStreamer/Downloads/ ubuntu@13.231.114.47:~/
scp -r -i ~/Downloads/aws-tok.pem  ubuntu@$TOK:~/logs/* ./logs/aws_b2/3/

#ssh -i ~/Downloads/aws-nova-.pem ubuntu@18.207.166.4 'rm conf/*.conf; cp conf/experiment_a/* conf/' &
#ssh -i ~/Downloads/aws-syd.pem ubuntu@52.63.5.53 'rm conf/*.conf; cp conf/experiment_a/* conf/' &
#ssh -i ~/Downloads/aws-sao.pem ubuntu@18.228.22.164 'rm conf/*.conf; cp conf/experiment_a/* conf/' &
#ssh -i ~/Downloads/aws-paris.pem ubuntu@15.237.43.180  'rm conf/*.conf; cp conf/experiment_a/* conf/' &
#ssh -i ~/Downloads/aws-can.pem ubuntu@35.182.71.127 'rm conf/*.conf; cp conf/experiment_a/* conf/' &
#ssh -i ~/Downloads/aws-tok.pem ubuntu@13.231.114.47 'rm conf/*.conf; cp conf/experiment_a/* conf/' &