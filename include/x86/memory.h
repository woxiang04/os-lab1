#ifndef __X86_MEMORY_H__
#define __X86_MEMORY_H__

#define SEG_CODEDATA            1
#define SEG_32BIT               1
#define SEG_4KB_GRANULARITY     1
#define SEG_TSS_32BIT           0x9

#define DPL_KERNEL              0
#define DPL_USER                3

#define SEG_WRITABLE            0x2
#define SEG_READABLE            0x2
#define SEG_EXECUTABLE          0x8

#define NR_SEGMENTS             6
#define SEG_NULL                0
#define SEG_KERNEL_CODE         1
#define SEG_KERNEL_DATA         2
#define SEG_USER_CODE			3
#define SEG_USER_DATA			4
#define SEG_TSS					5

#define SELECTOR_KERNEL(s)		( (s << 3) | DPL_KERNEL )
#define SELECTOR_USER(s)		( (s << 3) | DPL_USER )

/* 32bit x86 uses 4KB page size */
#define PAGE_SIZE					4096
// 2**10 page tables/pdt == NR_PDE
#define NR_PDE						1024
// 2**10 pages/page table == NR_PTE
#define NR_PTE						1024
// the size one page directory entry can access -- 2**22
#define PD_SIZE						((NR_PTE) * (PAGE_SIZE))
// the size one page table contains
#define PT_SIZE PD_SIZE

/* force the data to be aligned with page boundary.
   statically defined page tables uses this feature. */
#define align_to_page              __attribute((aligned(PAGE_SIZE)))

/* this marco will be defined by gcc if the source file is assembly */
#ifndef __ASSEMBLER__

#include "common.h"

/* the 32bit Page Directory(first level page table) data structure */
typedef union PageDirectoryEntry {
	struct {
		uint32_t present             : 1;
		uint32_t read_write          : 1;
		uint32_t user_supervisor     : 1;
		uint32_t page_write_through  : 1;
		uint32_t page_cache_disable  : 1;
		uint32_t accessed            : 1;
		uint32_t pad0                : 6;
		uint32_t page_frame          : 20;
	};
	uint32_t val;
} PDE;

/* the 32bit Page Table Entry(second level page table) data structure */
typedef union PageTableEntry {
	struct {
		uint32_t present             : 1;
		uint32_t read_write          : 1;
		uint32_t user_supervisor     : 1;
		uint32_t page_write_through  : 1;
		uint32_t page_cache_disable  : 1;
		uint32_t accessed            : 1;
		uint32_t dirty               : 1;
		uint32_t pad0                : 1;
		uint32_t global              : 1;
		uint32_t pad1                : 3;
		uint32_t page_frame          : 20;
	};
	uint32_t val;
} PTE;

/* the 64bit segment descriptor */
typedef struct SegmentDescriptor {
	uint32_t limit_15_0          : 16;
	uint32_t base_15_0           : 16;
	uint32_t base_23_16          : 8;
	uint32_t type                : 4;
	uint32_t segment_type        : 1;
	uint32_t privilege_level     : 2;
	uint32_t present             : 1;
	uint32_t limit_19_16         : 4;
	uint32_t soft_use            : 1;
	uint32_t operation_size      : 1;
	uint32_t pad0                : 1;
	uint32_t granularity         : 1;
	uint32_t base_31_24          : 8;
} SegDesc;

/* the 64bit segment descriptor */
typedef struct TSS {
	uint32_t prev;			// don't care
	uint32_t esp0;
	uint32_t ss0;
	uint8_t dont_care[88];
} TSS;

typedef struct GateDescriptor {
	uint32_t offset_15_0      : 16;
	uint32_t segment          : 16;
	uint32_t pad0             : 8;
	uint32_t type             : 4;
	uint32_t system           : 1;
	uint32_t privilege_level  : 2;
	uint32_t present          : 1;
	uint32_t offset_31_16     : 16;
} GateDesc;

/**
 * @see do_irq.S#asm_do_irq to see the order of registers
 */
typedef struct TrapFrame {
	uint32_t edi, esi, ebp, xxx, // <--- xxx is original ESP before `pushal`
            ebx, edx, ecx, eax;	// GPRs
	uint32_t gs, fs, es, ds;							// data segment registers
	int32_t irq;										// #irq
	uint32_t error_code;								// error code
	uint32_t eip, cs, eflags;							// execution state saved by hardware
	uint32_t esp, ss;									// only uses when DPL = 3
} TrapFrame;

#endif

#endif
