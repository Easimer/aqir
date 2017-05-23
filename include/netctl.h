/*
 * netctl.h - NetCtl header
 * Author: Daniel Meszaros <easimer@gmail.com>
 * EasimerNet-Confidental
 */
#pragma once

#include <stdint.h>

#define NETCTL_MAGIC (0xEA51)
#define NETCTL_UPDREQ (1)
#define NETCTL_UPDRES (2)
#define NETCTL_CTLREQ (3)
#define NETCTL_CTLRES (4)

typedef struct {
	uint16_t magic;
	uint16_t version;
	uint32_t type;
	uint64_t timestamp;
} __attribute__((packed)) netctl_pkthdr_t;

typedef struct {
	uint16_t type;
	uint16_t id;
	uint16_t parameter;
	uint8_t reserved[58];
} __attribute__((packed)) netctl_updreq_t;

typedef struct {
	uint16_t type;
	uint16_t id;
	uint16_t parameter;
	uint8_t reserved[58];
} __attribute__((packed)) netctl_ctlreq_t;

typedef struct {
	uint16_t type;
	uint16_t id;
	uint8_t reserved[60];
} __attribute__((packed)) netctl_updres_t;

typedef struct {
	uint16_t type;
	uint16_t id;
	uint8_t reserved[60];
} __attribute__((packed)) netctl_ctlres_t;
