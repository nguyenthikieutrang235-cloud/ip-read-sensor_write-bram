#include "xparameters.h"
#include "xil_io.h"
#include "xil_printf.h"
#include "sleep.h"
#include "pir_axi.h"

#define PIR_BASE_ADDR   XPAR_PIR_AXI_0_S00_AXI_BASEADDR
#define BRAM_BASE_ADDR  XPAR_BRAM_0_BASEADDR

int main()
{
    u32 pir_val;
    u32 prev_pir = 0;
    u32 bram_index = 0;


    xil_printf("\r\n--- WRITE BRAM ---\r\n");
    xil_printf("Move in front of PIR...\r\n");

    while (bram_index < 5)
    {
        pir_val = PIR_AXI_mReadReg(
                    PIR_BASE_ADDR,
                    PIR_AXI_S00_AXI_SLV_REG0_OFFSET
                  ) & 0x01;

        if (pir_val != prev_pir)
        {
            xil_printf("PIR = %d\n" ,pir_val);
        }

        if ((pir_val == 1) && (prev_pir == 0))
        {
            Xil_Out32(BRAM_BASE_ADDR + bram_index * 4, 0xA5A50001);

            xil_printf(
                "phat hien chuyen dong -> WRITE BRAM[%d] = 0xA5A50001\r\n",
                bram_index
            );

            bram_index++;
        }

        prev_pir = pir_val;
        usleep(200000);
    }
sleep (5);
    xil_printf("\r\n--- READ BRAM ---\r\n");

    for (int i = 0; i < bram_index; i++)
    {
        u32 val = Xil_In32(BRAM_BASE_ADDR + i * 4);
        xil_printf("READ: BRAM[%d] = 0x%08X\r\n", i, val);
    }

    xil_printf("\r\n=== TEST DONE ===\r\n");

    while (1);  
    return 0;
}
