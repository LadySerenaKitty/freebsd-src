/*-
 * SPDX-License-Identifier: BSD-2-Clause-FreeBSD
 *
 * Copyright (c) 2022 Alexander V. Chernikov <melifaro@FreeBSD.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/*
 * This file contains definitions shared among NETLINK_ROUTE family
 */
#ifndef _NETLINK_ROUTE_ROUTE_VAR_H_
#define _NETLINK_ROUTE_ROUTE_VAR_H_

#include <sys/priv.h> /* values for priv_check */

struct nlmsghdr;
struct nlpcb;
struct nl_pstate;

typedef int rtnl_msg_cb_f(struct nlmsghdr *hdr, struct nlpcb *nlp,
    struct nl_pstate *npt);

struct rtnl_cmd_handler {
	int		cmd;
	const char	*name;
	rtnl_msg_cb_f	*cb;
	int		priv;
	int		flags;
};

#define	RTNL_F_NOEPOCH	0x01

bool rtnl_register_messages(const struct rtnl_cmd_handler *handlers, int count);

/* route.c */
struct rib_cmd_info;
void rtnl_handle_route_event(uint32_t fibnum, const struct rib_cmd_info *rc);
void rtnl_routes_init(void);

/* neigh.c */
void rtnl_neighs_init(void);
void rtnl_neighs_destroy(void);

/* iface.c */
struct nl_parsed_link {
	char		*ifla_group;
	char		*ifla_ifname;
	char		*ifla_cloner;
	struct nlattr	*ifla_idata;
	unsigned short	ifi_type;
	int		ifi_index;
	uint32_t	ifla_mtu;
};

typedef int rtnl_iface_create_f(struct nl_parsed_link *lattrs, struct nlpcb *nlp,
    struct nl_pstate *npt);
typedef int rtnl_iface_modify_f(struct nl_parsed_link *lattrs, struct nlpcb *nlp,
    struct nl_pstate *npt);

struct nl_cloner {
	const char		*name;
	rtnl_iface_create_f	*create_f;
	rtnl_iface_modify_f	*modify_f;
	SLIST_ENTRY(nl_cloner)	next;
};

void rtnl_ifaces_init(void);
void rtnl_ifaces_destroy(void);
void rtnl_iface_add_cloner(struct nl_cloner *cloner);
void rtnl_iface_del_cloner(struct nl_cloner *cloner);

/* iface_drivers.c */
void rtnl_iface_drivers_register(void);

/* nexthop.c */
void rtnl_nexthops_init(void);
struct nhop_object *nl_find_nhop(uint32_t fibnum, int family,
    uint32_t uidx, int nh_flags, int *perror);


#endif
