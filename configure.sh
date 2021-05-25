#!/bin/sh

# sets up iptables firewall on NCSU VCL for TA Testing

sudo iptables -A INPUT -p tcp --dport 65432 -m conntrack --ctstate NEW,ESTABLISHED -j ACCEPT
sudo iptables -A OUTPUT -p tcp --dport 65432 -m conntrack --ctstate NEW,ESTABLISHED -j ACCEPT
