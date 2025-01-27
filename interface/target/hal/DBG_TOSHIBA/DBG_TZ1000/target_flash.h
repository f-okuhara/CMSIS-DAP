/* CMSIS-DAP Interface Firmware
 * Copyright (c) 2009-2013 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef TARGET_FLASH_H
#define TARGET_FLASH_H

#include "target_struct.h"
#include "swd_host.h"
#include <stdint.h>

#define FLASH_SECTOR_SIZE           (1024)

#define TARGET_AUTO_INCREMENT_PAGE_SIZE    0x1000

static uint8_t target_flash_init(uint32_t clk);
static uint8_t target_flash_uninit(void);
static uint8_t target_flash_erase_chip(void);
static uint8_t target_flash_erase_sector(uint32_t adr);
static uint8_t target_flash_program_page(uint32_t adr, uint8_t * buf, uint32_t size);

static const uint32_t TZ1000_FLM[] = {
    /*0x000*/ 0xE00ABE00, 0x062D780D, 0x24084068, 0xD3000040, 0x1E644058, 0x1C49D1FA, 0x2A001E52, 0x4770D1F2,
    /*0x020*/ 0x2340f04f, 0x7180f44f, 0x00896299, 0xf44f62d9, 0x6319710c, 0xf8c32105, 0x21011100, 0xf44f6359, 
    /*0x040*/ 0xf8d331fa, 0x07d220a0, 0xb149d008, 0xf8c3210f, 0xf8d310a0, 0x60011200, 0x47702000, 0xd1f01e49, 
    /*0x060*/ 0x47702001, 0x2340f04f, 0x15186298, 0x631962d8, 0x2100f8c3, 0x63582001, 0x30faf44f, 0x10a0f8d3, 
    /*0x080*/ 0xd4020789, 0xd1f91e40, 0xb120e005, 0xf8c3200f, 0x200000a0, 0x20014770, 0xb5384770, 0xf44f2206, 
    /*0x0a0*/ 0xf44f7144, 0xf7ff7080, 0xb928ffdd, 0x34faf44f, 0xf7ff4668, 0xb108ffb5, 0xbd382001, 0x07809800, 
    /*0x0c0*/ 0x1e64d401, 0x2000d1f4, 0x4984bd38, 0xf8c12000, 0x2a030154, 0xf04fd103, 0x20012140, 0x20006508, 
    /*0x0e0*/ 0x20004770, 0xb5104770, 0xffd7f7ff, 0x2008b970, 0xf852f000, 0xf44f22c7, 0xf44f7144, 0xf7ff7080, 
    /*0x100*/ 0xb918ffb1, 0xf84ff000, 0xd0002800, 0xbd102001, 0x4604b510, 0xffc1f7ff, 0x2008b978, 0xf83cf000, 
    /*0x120*/ 0xf040ba20, 0x496e0220, 0x7080f44f, 0xff9af7ff, 0xf000b918, 0x2800f838, 0x2001d000, 0xb4f0bd10, 
    /*0x140*/ 0x2640f04f, 0x7380f44f, 0x62b32500, 0x62f3009b, 0x730cf44f, 0x23356333, 0x3100f8c6, 0x63772701, 
    /*0x160*/ 0x33faf44f, 0x40a0f8d6, 0xd00a07e4, 0x230fb12b, 0x30a0f8c6, 0x5200f8d6, 0xb12f2700, 0x2001bcf0, 
    /*0x180*/ 0x1e5b4770, 0xe7f8d1ee, 0xd50207ab, 0xf000bcf0, 0xbcf0b862, 0xb81bf000, 0x0040eb00, 0xbf000080, 
    /*0x1a0*/ 0xd1fc1e40, 0xb5384770, 0x34faf44f, 0xf7ff4668, 0xb108ff37, 0xbd382001, 0x07c09800, 0x2000d005, 
    /*0x1c0*/ 0x28641c40, 0x1e64dbfc, 0x2000d1f0, 0xe92dbd38, 0x461641f0, 0x4605460c, 0xff5ff7ff, 0x4630bbc0, 
    /*0x1e0*/ 0x0103f024, 0xe0032300, 0xc8804a3e, 0x1d1b50d7, 0xd3f9428b, 0xd013428c, 0x18702300, 0xf004461a, 
    /*0x200*/ 0xe0070703, 0x6b01f810, 0x0cc2ea4f, 0xf60cfa06, 0x1c524333, 0xd8f54297, 0x2140f101, 0x3200f8c1, 
    /*0x220*/ 0xd2042ce0, 0xeba02008, 0xf7ff1054, 0xba28ffb5, 0x0202f040, 0x1e64482a, 0xea403020, 0xf44f6104, 
    /*0x240*/ 0xf7ff7080, 0xb918ff0f, 0xffadf7ff, 0xd0002800, 0xe8bd2001, 0xe92d81f0, 0x461641f0, 0x4605460c, 
    /*0x260*/ 0xff1bf7ff, 0x4630bbc0, 0x0103f024, 0xe0032300, 0xc8804a1c, 0x1d1b50d7, 0xd3f9428b, 0xd013428c, 
    /*0x280*/ 0x18702300, 0xf004461a, 0xe0070703, 0x6b01f810, 0x0cc2ea4f, 0xf60cfa06, 0x1c524333, 0xd8f54297, 
    /*0x2a0*/ 0x2140f101, 0x3200f8c1, 0xd2042ce0, 0xeba02008, 0xf7ff1054, 0xba28ff71, 0x0232f040, 0x1e644808, 
    /*0x2c0*/ 0xea403020, 0xf44f6104, 0xf7ff7081, 0xb918fecb, 0xff69f7ff, 0xd0bc2800, 0xe7ba2001, 0x4004a000, 
    /*0x2e0*/ 0x00030310, 0x40004200, 0x00000000, 
};

static const TARGET_FLASH flash = {
    0x200000CB, // Init
    0x200000E3, // UnInit
    0x200000E7, // EraseChip
    0x20000111, // EraseSector
    0x2000013F, // ProgramPage

    // RSB : base adreess is address of Execution Region PrgData in map file
    //       to access global/static data
    // RSP : Initial stack pointer
    { 0x20000001, 0x20000300, 0x20008000 - 0x20 }, // {breakpoint, RSB, RSP}

    0x20000400, // program_buffer
    0x20000000, // algo_start
    0x000002ec, // algo_size
    TZ1000_FLM, // image
    256         // ram_to_flash_bytes_to_be_written
};

static uint8_t target_flash_init(uint32_t clk) {
    // Download flash programming algorithm to target and initialise.
    if (!swd_write_memory(flash.algo_start, (uint8_t *)flash.image, flash.algo_size)) {
        return 0;
    }

    if (!swd_flash_syscall_exec(&flash.sys_call_param, flash.init, 0, 0 /* clk value is not used */, 0, 0)) {
        return 0;
    }

    return 1;
}

static uint8_t target_flash_erase_sector(unsigned int sector) {    
    if (!swd_flash_syscall_exec(&flash.sys_call_param, flash.erase_sector, sector*0x1000, 0, 0, 0)) {
        return 0;
    }

    return 1;
}
//#include <RTL.h>
static uint8_t target_flash_erase_chip(void) {
    if (!swd_flash_syscall_exec(&flash.sys_call_param, flash.erase_chip, 0, 0, 0, 0)) {
        return 0;
    }
//os_dly_wait(200);
    return 1;
}

static uint8_t target_flash_program_page(uint32_t addr, uint8_t * buf, uint32_t size)
{
    uint32_t bytes_written = 0;

    // Program a page in target flash.
    if (!swd_write_memory(flash.program_buffer, buf, size)) {
        return 0;
    }

    while(bytes_written < size) {
        if (!swd_flash_syscall_exec(&flash.sys_call_param,
                                    flash.program_page,
                                    addr,
                                    flash.ram_to_flash_bytes_to_be_written,
                                    flash.program_buffer + bytes_written, 0)) {
            return 0;
        }

        bytes_written += flash.ram_to_flash_bytes_to_be_written;
        addr += flash.ram_to_flash_bytes_to_be_written;
    }

    return 1;
}


#endif
