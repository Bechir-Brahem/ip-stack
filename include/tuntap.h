#ifndef TUNTAP_H
#define TUNTAP_H

#include <fcntl.h>
#include <linux/if_tun.h>
#include <net/if.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

int tun_alloc(char *dev, int flags);


#endif
