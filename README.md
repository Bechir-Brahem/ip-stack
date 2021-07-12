# setup

sudo ip tuntap add name tap1 mode tap  
sudo ip link set tap1 up  
sudo ip link set tap1 promisc on  
sudo ip addr add 10.0.0.2/24 dev tap1  
sudo ip route add dev tap1 10.0.0.2/24  

# testing

sudo arping -I tap0 10.0.0.2  



# sources

https://www.youtube.com/watch?v=HHF0xDu4l5E  
http://www.saminiir.com/lets-code-tcp-ip-stack-1-ethernet-arp/  
https://backreference.org/2010/03/26/tuntap-interface-tutorial/  
https://datatracker.ietf.org/doc/html/rfc826  
