
#include <stdint.h>

#include "define.h"
#include "table.h"
// testing of my implementation
int main(void)
{
    page_table_t table = pt_init();
    uint8_t physical_memory[MEM_SIZE];  // simulated ram
    int frame_number = allocate_frame(&table);
    uint32_t virt_addr = 100;
    map_page(virt_addr, frame_number, WRITE | EXECUTE, &table);
    uint32_t phys_addr = virt_to_phys(virt_addr, &table);
    printf(
        "this is the virtual addr: %u and this the correlating phys_addr: "
        "0x%x\n",
        virt_addr, phys_addr);
    printf("Expected: 0x%x (frame 0 * 4096 + %u)\n", 0 * PAGE_SIZE + virt_addr,
           virt_addr);
    // next test:
    int frame_number1 = allocate_frame(&table);
    uint32_t virt_addr2 = 10000;
    map_page(virt_addr2, frame_number1, WRITE | EXECUTE, &table);
    uint32_t phys_addr2 = virt_to_phys(virt_addr2, &table);
    printf(
        "this is the virtual addr: %u and this the correlating phys_addr: "
        "0x%x\n",
        virt_addr2, phys_addr2);
    // next test write memory and read memory:
    write_mem(2000, 3, &table, physical_memory);
    printf("this is your value on  the virtual adress: %d\n",
           read_mem(2000, &table, physical_memory));
    return 1;
}
