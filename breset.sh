#!/bin/bash
sudo rm ./Downloads/Downloads_A/*
sudo rm ./Downloads/Downloads_B/*
sudo rm ./Downloads/Downloads_C/*
sudo rm ./Downloads/Downloads_D/*
sudo rm ./Downloads/Downloads_E/*
sudo rm ./Downloads/Downloads_F/*
cp ./Downloads/experiment_B/Downloads_A/* ./Downloads/Downloads_A/
cp ./Downloads/experiment_B/Downloads_B/* ./Downloads/Downloads_B/
cp ./Downloads/experiment_B/Downloads_C/* ./Downloads/Downloads_C/
cp ./Downloads/experiment_B/Downloads_D/* ./Downloads/Downloads_D/
cp ./Downloads/experiment_B/Downloads_E/* ./Downloads/Downloads_E/
cp ./Downloads/experiment_B/Downloads_F/* ./Downloads/Downloads_F/
sudo rm conf/*.conf
cp conf/experiment_b/* conf/
