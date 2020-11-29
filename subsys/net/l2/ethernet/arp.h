/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __ARP_H
#define __ARP_H

#if defined(CONFIG_NET_ARP) && defined(CONFIG_NET_NATIVE)

#include <sys/slist.h>
#include <net/ethernet.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Address resolution (ARP) library
 * @defgroup arp ARP Library
 * @ingroup networking
 * @{
 */

#define NET_ARP_HDR(pkt) ((struct net_arp_hdr *)net_pkt_data(pkt))

struct net_arp_hdr {
	uint16_t hwtype;		/* HTYPE */
	uint16_t protocol;		/* PTYPE */
	uint8_t hwlen;			/* HLEN */
	uint8_t protolen;		/* PLEN */
	uint16_t opcode;
	struct net_eth_addr src_hwaddr;	/* SHA */
	struct in_addr src_ipaddr;	/* SPA */
	struct net_eth_addr dst_hwaddr;	/* THA */
	struct in_addr dst_ipaddr;	/* TPA */
}  __packed;

#define NET_ARP_HTYPE_ETH 1
#define NET_ARP_IPV4_PTYPE_SIZE 4

#define NET_ARP_REQUEST 1
#define NET_ARP_REPLY   2

struct net_pkt *net_arp_prepare(struct net_pkt *pkt,
				struct in_addr *request_ip,
				struct in_addr *current_ip);
enum net_verdict net_arp_input(struct net_pkt *pkt,
			       struct net_eth_hdr *eth_hdr);

struct arp_entry {
	sys_snode_t node;
	uint32_t req_start;
	struct net_if *iface;
	struct in_addr ip;
	union {
		struct net_pkt *pending;
		struct net_eth_addr eth;
	};
};

typedef void (*net_arp_cb_t)(struct arp_entry *entry,
			     void *user_data);
int net_arp_foreach(net_arp_cb_t cb, void *user_data);

void net_arp_clear_cache(struct net_if *iface);
void net_arp_init(void);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#else /* CONFIG_NET_ARP */
#define net_arp_prepare(_kt, _u1, _u2) _kt
#define net_arp_input(...) NET_OK
#define net_arp_clear_cache(...)
#define net_arp_foreach(...) 0
#define net_arp_init(...)

#endif /* CONFIG_NET_ARP */

#endif /* __ARP_H */
