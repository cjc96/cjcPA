#include "common.h"

uint32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t, size_t, uint32_t);

#ifdef CACHE
#include "cpu/reg.h"
#endif

#ifdef SEGMENT
#include "cpu/reg.h"
extern CPU_state cpu;
#endif

/* Memory accessing interfaces */

#ifdef CACHE
static uint32_t rand_temp()
{
	static uint32_t hahahaha;
	return ++hahahaha;
}
#endif

uint32_t hwaddr_read(hwaddr_t addr, size_t len) {
#ifdef CACHE	
	uint32_t musk = ~0u >> ((4 - len) << 3);
	uint32_t tag = addr & 0xfffffe00, offset = addr & 0x0000003f, start_sp = ((addr + len - 1) & 0x000001c0) << 1, tag_sp = (addr + len -1) & 0xfffffe00;
	
	uint32_t i, start =(addr & 0x000001c0) << 1, end = start + 128;
	for (i = start; i < end; i++)
	{
		if (l1_cache[i].valid && l1_cache[i].tag == tag)
		{
			if (start_sp != start)
			{
				uint8_t ans[4], *temp = ans, loc = 0;
				while ((((addr + loc) & 0x000001c0) << 1) == start)
				{
					ans[loc] = l1_cache[i].data_b[offset + loc];
					loc++;
				}
				
				uint32_t j, end_sp = start_sp + 128, loc_sp = 0;
				for (j = start_sp; j < end_sp; j++)
				{
					if (l1_cache[j].valid && l1_cache[j].tag == tag_sp)
					{
						assert(loc < len);
						while (loc < len)
						{
							ans[loc] = l1_cache[j].data_b[loc_sp];
							loc++;
							loc_sp++;
						}
						return *(uint32_t *)temp & musk;
					}
				}
				break;
			}
			else
			{
				uint8_t *temp = &l1_cache[i].data_b[offset];
				return *(uint32_t *)temp & musk;
			}
		}
	}
#ifndef L2_CACHE
	uint32_t temp = rand_temp() % 128 + start, temp_set = addr - offset;
	
	for (i = 0; i < 16; i++)
	{
		l1_cache[temp].data_d[i] = dram_read(temp_set + i * 4, 4);
	}
	l1_cache[temp].tag = tag;
	l1_cache[temp].valid = 1;
	if (start != start_sp)
	{
		uint32_t start_sp_temp = start_sp + rand_temp() % 128, temp_set_sp = (addr + len -1) & 0xffffffc0;
		for (i = 0; i < 16; i++)
		{	
			l1_cache[start_sp_temp].data_d[i] = dram_read(temp_set_sp + i * 4, 4);
		}
		l1_cache[start_sp_temp].tag = tag_sp;
		l1_cache[start_sp_temp].valid = 1;
	}
	
	return dram_read(addr, len) & (~0u >> ((4 - len) << 3));
#endif

#ifdef L2_CACHE

#endif

#endif

#ifndef CACHE
	return dram_read(addr, len) & (~0u >> ((4 - len) << 3));
#endif
}

void hwaddr_write(hwaddr_t addr, size_t len, uint32_t data) {
#ifdef CACHE
	dram_write(addr, len, data);
	
	uint32_t tag = addr & 0xfffffe00, offset = addr & 0x0000003f, tag_sp = (addr + len - 1) & 0xfffffe00, start_sp = ((addr + len - 1) & 0x000001c0) << 1;
	
	uint32_t i, start = (addr & 0x000001c0) << 1, end = start + 128;
	for (i = start; i < end; i++)
	{
		if (l1_cache[i].valid && l1_cache[i].tag == tag)
		{
			if (start_sp != start)
			{
				uint8_t loc = 0;
				while ((((addr + loc) & 0x000001c0) << 1) == start)
				{
					l1_cache[i].data_b[offset + loc] = data & 0xff;
					data >>= 8;
					loc++;
				}
				
				uint32_t j, end_sp = start_sp + 128, loc_sp = 0;
				for (j = start_sp; j < end_sp; j++)
				{
					if (l1_cache[j].valid && l1_cache[j].tag == tag_sp)
					{
						assert(loc < len);
						while (loc < len)
						{
							l1_cache[j].data_b[loc_sp] = data & 0xff;
							data >>= 8;
							loc++;
							loc_sp++;
						}
					}
				}
				break;
			}
			else
			{
				uint32_t j, temp_end = offset + len;
				for (j = offset; j < temp_end; j++)
				{
					l1_cache[i].data_b[j] = data & 0xff;
					data >>= 8;
				}
			}
		}
	}
#endif

#ifndef CACHE
	dram_write(addr, len, data);
#endif
}

#ifndef PAGE
uint32_t lnaddr_read(lnaddr_t addr, size_t len) {
	return hwaddr_read(addr, len);
}

void lnaddr_write(lnaddr_t addr, size_t len, uint32_t data) {
	hwaddr_write(addr, len, data);
}
#endif

#ifdef PAGE

hwaddr_t page_translate(lnaddr_t addr)
{
	return (hwaddr_read(hwaddr_read(cpu.cr3.val + (addr >> 22) * 4, 4) + ((addr >> 12) & 0x3ff) * 4, 4) & 0xffc00000) + (addr & 0xfff);
}

uint32_t lnaddr_read(lnaddr_t addr, size_t len) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
	if ((addr+len) >> 12 != addr >> 12) {
		/* this is a special case, you can handle it later. */
		assert(0);
	}
	else {
		hwaddr_t hwaddr = page_translate(addr);
		return hwaddr_read(hwaddr, len);
	}
}

void lnaddr_write(lnaddr_t addr, size_t len, uint32_t data) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
	if ((addr+len) >> 12 != addr >> 12) {
		/* this is a special case, you can handle it later. */
		assert(0);
	}
	else {
		hwaddr_t hwaddr = page_translate(addr);
		hwaddr_write(hwaddr, len, data);
	}
}
#endif

#ifndef SEGMENT
uint32_t swaddr_read(swaddr_t addr, size_t len) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
	return lnaddr_read(addr, len);
}
#endif

#ifdef SEGMENT
uint32_t swaddr_read(swaddr_t addr, size_t len, uint32_t seg) {
#ifdef DEBUG
    assert(len == 1 || len == 2 || len ==4);
#endif
	uint32_t temp;
	if (seg == 0)
		temp = cpu.DS.cache.base;
	else if (seg == 1)
		temp = cpu.SS.cache.base;
	else if (seg == 2)
		temp = cpu.ES.cache.base;
	else if (seg == 3)
		temp = cpu.CS.cache.base;
#ifdef DEBUG
	else
		assert(0);
#endif	 
	return lnaddr_read(addr + temp, len);
}
#endif

#ifndef SEGMENT
void swaddr_write(swaddr_t addr, size_t len, uint32_t data) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
	lnaddr_write(addr, len, data);
}
#endif

#ifdef SEGMENT
void swaddr_write(swaddr_t addr, size_t len, uint32_t data, uint32_t seg) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
	uint32_t temp;
	if (seg == 0)
		temp = cpu.DS.cache.base;
	else if (seg == 1)
		temp = cpu.SS.cache.base;
	else if (seg == 2)
		temp = cpu.ES.cache.base;
	else if (seg == 3)
		temp = cpu.CS.cache.base;
#ifdef DEBUG
	else
		assert(0);
#endif
	lnaddr_write(addr + temp, len, data);
}
#endif
