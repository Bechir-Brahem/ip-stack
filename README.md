# Presentation

this project is a simple implementation of an ip stack that answers to arp packets
and also responds to icmp ping requests. 

This code implements the ip stack on a virtual tap interface that you can setup by using 
the setup.sh bash file

![arp ](images/arp.png )
![icmp](images/icmp.png)
![ping](images/ping.png)


# setup

```bash
chmod +x setup.sh
./setup.sh
make
sudo ./tuntap
```

# testing

sudo arping -I tap0 10.0.0.1  
sudo ping -I tap1 10.0.0.1


# sources

https://www.youtube.com/watch?v=HHF0xDu4l5E  
http://www.saminiir.com/lets-code-tcp-ip-stack-1-ethernet-arp/  
https://backreference.org/2010/03/26/tuntap-interface-tutorial/  
https://datatracker.ietf.org/doc/html/rfc826  
https://www.kernel.org/doc/html/latest/networking/tuntap.html?highlight=tap  
https://datatracker.ietf.org/doc/html/rfc791  
https://datatracker.ietf.org/doc/html/rfc792  
