#include <stdint.h>

#include "define.h"
#include "table.h"
// testing of my implementation
int main(void)
{
    page_table_t table = pt_init();
    map_page(0, 0, WRITE | EXECUTE, &table);
    uint32_t virt_addr = 100;
    uint32_t phys_addr = virt_to_phys(virt_addr, &table);
    printf(
        "this is the virtual addr: %u and this the correlating phys_addr: %u\n",
        virt_addr, phys_addr);
    printf("Expected: 0x%x (frame 0 * 4096 + 100)\n", 0 * PAGE_SIZE + 100);
    pt_free(&table);
    return 1;
}
