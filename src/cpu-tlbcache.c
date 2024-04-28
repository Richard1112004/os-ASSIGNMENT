/*
 * Copyright (C) 2024 pdnguyen of the HCMC University of Technology
 */
/*
 * Source Code License Grant: Authors hereby grants to Licensee 
 * a personal to use and modify the Licensed Source Code for 
 * the sole purpose of studying during attending the course CO2018.
 */
//#ifdef MM_TLB
/*
 * Memory physical based TLB Cache
 * TLB cache module tlb/tlbcache.c
 *
 * TLB cache is physically memory phy
 * supports random access 
 * and runs at high speed
 */


#include "mm.h"
#include <stdlib.h>

#define init_tlbcache(mp,sz,...) init_memphy(mp, sz, (1, ##__VA_ARGS__))

/*
 *  tlb_cache_read read TLB cache device
 *  @mp: memphy struct
 *  @pid: process id
 *  @pgnum: page number
 *  @value: obtained value
 */

int tlb_cache_read(struct memphy_struct * mp, int pid, int pgnum, BYTE value)
{
   /* TODO: the identify info is mapped to 
    *      cache line by employing:
    *      direct mapped, associated mapping etc.
    */
    // Lặp qua tất cả các mục trong cache TLB
    for (int i = 0; i < TLB_CACHE_SIZE; i++) {
        // Nếu tìm thấy một mục có cùng pid và pgnum
        if (tlb_cache[i].pid == pid && tlb_cache[i].pgnum == pgnum) {
            // Đọc giá trị từ cache TLB
            *value = tlb_cache[i].value;
            return 0; // Đọc thành công
        }
    }
    return -1; // Không tìm thấy mục trong cache TLB
}

/*
 *  tlb_cache_write write TLB cache device
 *  @mp: memphy struct
 *  @pid: process id
 *  @pgnum: page number
 *  @value: obtained value
 */
int tlb_cache_write(struct memphy_struct *mp, int pid, int pgnum, BYTE value)
{
   /* TODO: the identify info is mapped to 
    *      cache line by employing:
    *      direct mapped, associated mapping etc.
    */
    // Lặp qua tất cả các mục trong cache TLB
    for (int i = 0; i < TLB_CACHE_SIZE; i++) {
        // Nếu tìm thấy một mục trống trong cache TLB
        if (tlb_cache[i].pid == -1) {
            // Ghi giá trị vào cache TLB
            tlb_cache[i].pid = pid;
            tlb_cache[i].pgnum = pgnum;
            tlb_cache[i].value = value;
            return 0; // Ghi thành công
        }
    }
    return -1; // Cache TLB đầy, không thể ghi mới vào
}

/*
 *  TLBMEMPHY_read natively supports MEMPHY device interfaces
 *  @mp: memphy struct
 *  @addr: address
 *  @value: obtained value
 */
int TLBMEMPHY_read(struct memphy_struct * mp, int addr, BYTE *value)
{
   if (mp == NULL)
     return -1;

   /* TLB cached is random access by native */
   *value = mp->storage[addr];

   return 0;
}


/*
 *  TLBMEMPHY_write natively supports MEMPHY device interfaces
 *  @mp: memphy struct
 *  @addr: address
 *  @data: written data
 */
int TLBMEMPHY_write(struct memphy_struct * mp, int addr, BYTE data)
{
   if (mp == NULL)
     return -1;

   /* TLB cached is random access by native */
   mp->storage[addr] = data;

   return 0;
}

/*
 *  TLBMEMPHY_format natively supports MEMPHY device interfaces
 *  @mp: memphy struct
 */


int TLBMEMPHY_dump(struct memphy_struct * mp)
{
   /*TODO dump memphy contnt mp->storage 
    *     for tracing the memory content
    */
    if (mp == NULL) {
        printf("Error: Invalid memory physical structure\n");
        return -1;
    }

    printf("memory physical content:\n");
    for (int i = 0; i < mp->maxsz; i++) {
        printf("%d: %d\n", i, mp->storage[i]);
    }

    return 0;
}


/*
 *  Init TLBMEMPHY struct
 */
int init_tlbmemphy(struct memphy_struct *mp, int max_size)
{
   mp->storage = (BYTE *)malloc(max_size*sizeof(BYTE));
   mp->maxsz = max_size;

   mp->rdmflg = 1;

   return 0;
}

//#endif
