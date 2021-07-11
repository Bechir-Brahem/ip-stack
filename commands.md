sudo ip tuntap add name tapint mode tap
sudo ip link set tapint promisc on
sudo ip link set wlan0 promisc on
