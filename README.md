sudo ip tuntap add name tap1 mode tap
sudo ip link set tap1 promisc on
sudo ip route add dev tap1 10.0.0.2/24
