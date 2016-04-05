#include "common.h"

uint32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t, size_t, uint32_t);

#ifdef CACHE
#include "cpu/reg.h"
#endif

/* Memory accessing interfaces */

uint32_t hwaddr_read(hwaddr_t addr, size_t len) {
#ifdef CACHE
	
	uint32_t musk = ~0u >> ((4 - len) << 3);
	uint32_t tag = addr & 0xfffffe00, offset = addr & 0x0000003f, tag_sp = (addr + len - 1) & 0xfffffe00;
	
	uint32_t i, start =addr & 0x000001c0, end = start + 128;
	for (i = start; i < end; i++)
	{
		if (l1_cache[i].sign && l1_cache[i].tag == tag)
		{
			if (tag_sp != tag)
			{
				uint8_t ans[4], *temp = ans, loc = 0;
				while (((addr + loc) & 0xfffffe00) == tag)
				{
					ans[loc] = l1_cache[i].data_b[offset + loc];
					loc++;
				}
				
				uint32_t j, start_sp = (addr + len - 1) & 0x000001c0 << 1, end_sp = start_sp + 128, loc_sp = 0;
				for (j = start_sp; j < end_sp; j++)
				{
					if (l1_cache[j].sign && l1_cache[j].tag == tag_sp)
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
	
	uint32_t temp = rand() % 128 + start, temp_set = addr - offset;
	for (i = 0; i < 16; i++)
	{
		l1_cache[temp].data_d[i] = dram_read(temp_set + i * 4, 4);
	}
	l1_cache[temp].tag = tag;
	l1_cache[temp].sign = 1;
	if (tag != tag_sp)
	{
		for (i = 0; i < 16; i++)
		{	
			l1_cache[temp + 1].data_d[i] = dram_read(tag_sp + i * 4, 4);
		}
		l1_cache[temp + 1].tag = tag_sp;
		l1_cache[temp + 1].sign = 1;
	}
	
	printf("Not hit.\nval = %x\taddr = %x\n",dram_read(addr, len) & musk, addr);
	
	return dram_read(addr, len) & (~0u >> ((4 - len) << 3));
#endif

#ifndef CACHE
	return dram_read(addr, len) & (~0u >> ((4 - len) << 3));
#endif
}

void hwaddr_write(hwaddr_t addr, size_t len, uint32_t data) {
#ifdef CACHE
	dram_write(addr, len, data);
	
	uint32_t tag = addr & 0xfffffe00, offset = addr & 0x0000003f, tag_sp = (addr + len - 1) & 0xfffffe00;
	
	uint32_t i, start = addr & 0x000001c0 << 1, end = start + 128;
	for (i = start; i < end; i++)
	{
		if (l1_cache[i].sign && l1_cache[i].tag == tag)
		{
			if (tag_sp != tag)
			{
				uint8_t loc = 0;
				while (((addr + loc) & 0xfffffe00) == tag)
				{
					l1_cache[i].data_b[offset + loc] = data & 0xff;
					data >>= 8;
					loc++;
				}
				
				uint32_t j, start_sp = (addr + len - 1) & 0x000001c0, end_sp = start_sp + 128, loc_sp = 0;
				for (j = start_sp; j < end_sp; j++)
				{
					if (l1_cache[j].sign && l1_cache[j].tag == tag_sp)
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

uint32_t lnaddr_read(lnaddr_t addr, size_t len) {
	return hwaddr_read(addr, len);
}

void lnaddr_write(lnaddr_t addr, size_t len, uint32_t data) {
	hwaddr_write(addr, len, data);
}

uint32_t swaddr_read(swaddr_t addr, size_t len) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
	return lnaddr_read(addr, len);
}

void swaddr_write(swaddr_t addr, size_t len, uint32_t data) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
	lnaddr_write(addr, len, data);
}

