/*
Copyright 2018 VMware, Inc.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

/*
 * This file contains all functions and definitions necessary for the user space
 * target C code to compile. It is part of the extended ebpf testing framework
 * and must be included with any file generated by the p4c-ebpf test compiler.
 * This file also depends on the ebpf_registry, which defines a central table
 * repository as well as various ebpf map operations.
 */

#ifndef BACKENDS_EBPF_RUNTIME_EBPF_USER_H_
#define BACKENDS_EBPF_RUNTIME_EBPF_USER_H_

#include "ebpf_registry.h"
#include "ebpf_common.h"

#define bpf_printk(fmt, ...)                                            \
                ({                                                      \
                        char ____fmt[] = fmt;                           \
                        printf(____fmt, sizeof(____fmt),                \
                                     ##__VA_ARGS__);                    \
                })

/** helper macro to place programs, maps, license in
 * different sections in elf_bpf file. Section names
 * are interpreted by elf_bpf loader
 * In the userspace version, this macro does nothing
 */
#define SEC(NAME)

/* simple descriptor which replaces the kernel sk_buff structure */
struct sk_buff {
    void *data;
    u16 len;
    u32 ifindex;
};

/* flags for BPF_MAP_UPDATE_ELEM command, copied from "linux/bpf" */
#define BPF_ANY     0 /* create new element or update existing */
#define BPF_NOEXIST 1 /* create new element if it didn't exist */
#define BPF_EXIST   2 /* update existing element */
#define BPF_F_LOCK  4 /* spin_lock-ed map_lookup/map_update */

/* Support bpf map types */
enum bpf_map_type {
    BPF_MAP_TYPE_HASH,
    BPF_MAP_TYPE_ARRAY,
};


#define SK_BUFF struct sk_buff
#define REGISTER_START() \
struct bpf_table tables[] = {
#define REGISTER_TABLE(NAME, TYPE, KEY_SIZE, VALUE_SIZE, MAX_ENTRIES) \
    { MAP_PATH"/"#NAME, TYPE, KEY_SIZE, VALUE_SIZE, MAX_ENTRIES, NULL },
#define REGISTER_END() \
    { 0, 0, 0, 0, 0 } \
};

#define BPF_MAP_LOOKUP_ELEM(table, key) \
    registry_lookup_table_elem(MAP_PATH"/"#table, key)
#define BPF_MAP_UPDATE_ELEM(table, key, value, flags) \
    registry_update_table(MAP_PATH"/"#table, key, value, flags)


/* These should be automatically generated and included in the generated x.h header file */
extern struct bpf_table tables[];
extern int ebpf_filter(struct sk_buff *skb);

#endif  // BACKENDS_EBPF_RUNTIME_EBPF_USER_H_
