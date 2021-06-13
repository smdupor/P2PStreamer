#!/bin/sh

# Nova
#ssh -i ~/Downloads/aws-nova-.pem ubuntu@18.207.166.4 'mv P2PStreamer/* ~/'

#Sydney
#ssh -i ~/Downloads/aws-syd.pem ubuntu@52.63.5.53 'mkdir log'
#scp -r -i ~/Downloads/aws-syd.pem ../P2PStreamer/bin/ ubuntu@52.63.5.53:~/
#scp -r -i ~/Downloads/aws-syd.pem ../P2PStreamer/Downloads/ ubuntu@52.63.5.53:~/
#scp -r -i ~/Downloads/aws-syd.pem ../P2PStreamer/conf/ ubuntu@52.63.5.53:~/

#Sao
#ssh -i ~/Downloads/aws-sao.pem ubuntu@18.228.22.164 'mkdir log'
#scp -r -i ~/Downloads/aws-sao.pem ../P2PStreamer/bin/ ubuntu@18.228.22.164:~/
#scp -r -i ~/Downloads/aws-sao.pem ../P2PStreamer/Downloads/ ubuntu@18.228.22.164:~/
#scp -r -i ~/Downloads/aws-sao.pem ../P2PStreamer/conf/ ubuntu@18.228.22.164:~/

#Paris
#ssh -i ~/Downloads/aws-paris.pem ubuntu@15.237.43.180 'mkdir log'
#scp -r -i ~/Downloads/aws-paris.pem ../P2PStreamer/bin/ ubuntu@15.237.43.180:~/
#scp -r -i ~/Downloads/aws-paris.pem ../P2PStreamer/Downloads/ ubuntu@15.237.43.180:~/
#scp -r -i ~/Downloads/aws-paris.pem ../P2PStreamer/conf/ ubuntu@15.237.43.180:~/

#Canada
#ssh -i ~/Downloads/aws-can.pem ubuntu@35.182.71.127 'mkdir log'
#scp -r -i ~/Downloads/aws-can.pem ../P2PStreamer/bin/ ubuntu@35.182.71.127:~/
#scp -r -i ~/Downloads/aws-can.pem ../P2PStreamer/Downloads/ ubuntu@35.182.71.127:~/
#scp -r -i ~/Downloads/aws-can.pem ../P2PStreamer/conf/ ubuntu@35.182.71.127:~/

#Tokyo
#ssh -i ~/Downloads/aws-tok.pem ubuntu@13.231.114.47 'mkdir log'
#scp -r -i ~/Downloads/aws-tok.pem ../P2PStreamer/bin/ ubuntu@13.231.114.47:~/
#scp -r -i ~/Downloads/aws-tok.pem ../P2PStreamer/Downloads/ ubuntu@13.231.114.47:~/
#scp -r -i ~/Downloads/aws-tok.pem ../P2PStreamer/conf/ ubuntu@13.231.114.47:~/

ssh -i ~/Downloads/aws-nova-.pem ubuntu@18.207.166.4 'rm conf/*.conf; cp conf/experiment_a/* conf/' &
ssh -i ~/Downloads/aws-syd.pem ubuntu@52.63.5.53 'rm conf/*.conf; cp conf/experiment_a/* conf/' &
ssh -i ~/Downloads/aws-sao.pem ubuntu@18.228.22.164 'rm conf/*.conf; cp conf/experiment_a/* conf/' &
ssh -i ~/Downloads/aws-paris.pem ubuntu@15.237.43.180  'rm conf/*.conf; cp conf/experiment_a/* conf/' &
ssh -i ~/Downloads/aws-can.pem ubuntu@35.182.71.127 'rm conf/*.conf; cp conf/experiment_a/* conf/' &
ssh -i ~/Downloads/aws-tok.pem ubuntu@13.231.114.47 'rm conf/*.conf; cp conf/experiment_a/* conf/' &