#!/bin/bash
rm ./Downloads/Downloads_A/*
rm ./Downloads/Downloads_B/*
rm ./Downloads/Downloads_C/*
rm ./Downloads/Downloads_D/*
rm ./Downloads/Downloads_E/*
rm ./Downloads/Downloads_F/*
cp ./Downloads/experiment_A/Downloads_A ./Downloads/Downloads_A/
cp ./Downloads/experiment_A/Downloads_B ./Downloads/Downloads_B/
cp ./Downloads/experiment_A/Downloads_C/* ./Downloads/Downloads_C/
cp ./Downloads/experiment_A/Downloads_D/* ./Downloads/Downloads_D/
cp ./Downloads/experiment_A/Downloads_E/* ./Downloads/Downloads_E/
sudo chmod -R ug+rw ./Downloads/Downloads_A/*
sudo chmod -R ug+rw ./Downloads/Downloads_B/*
sudo chmod -R ug+rw ./Downloads/Downloads_C/*
sudo chmod -R ug+rw ./Downloads/Downloads_D/*
sudo chmod -R ug+rw ./Downloads/Downloads_E/*
rm conf/*.conf
cp conf/experiment_a/* conf/
