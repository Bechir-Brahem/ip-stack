#!/bin/bash
sudo ip tuntap add name tap1 mode tap  
echo device created
sudo ip link set tap1 up 
echo device is up
sudo ip link set tap1 promisc on
echo promiscuous mode is on
sudo ip addr add 10.0.0.2/24 dev tap1  
echo ip addr assigned
sudo ip route add dev tap1 10.0.0.0/24  
echo route added

