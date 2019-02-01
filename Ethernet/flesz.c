/* 
zapis flash  
 */
#include "../mcc_generated_files/mcc.h"

unsigned  int  NVMUnlock (unsigned int nvmop)
{
unsigned int  status;
// Suspend or Disable all Interrupts
asm      volatile ("di %0" : "=r" (status));
// Enable Flash Write/Erase Operations and Select
// Flash operation to perform
NVMCON = nvmop;
// Write Keys
NVMKEY = 0xAA996655;
NVMKEY = 0x556699AA;
// Start the operation using the Set Register
NVMCONSET = 0x8000;
// Wait for operation to complete
while (NVMCON & 0x8000);
// Restore Interrupts
if (status & 0x00000001)
   {
       // asm      volatile ("ei");
   // __builtin_enable_interrupts();
    //
    INTERRUPT_GlobalEnable();
   }
else
   {
    //asm      volatile ("di");
    // __builtin_disable_interrupts();
    INTERRUPT_GlobalDisable();
   }
// Disable NVM write enable
NVMCONCLR = 0x0004000;
// Return WRERR and LVDERR Error Status Bits 
return (NVMCON & 0x3000);
}

unsigned int NVMWriteWord (void* address, unsigned int data)
{
unsigned int res;
// Load data into NVMDATA register
NVMDATA0 = data;
// Load address to program into NVMADDR register
NVMADDR = (unsigned int) address;
// Unlock and Write Word
res = NVMUnlock (0x4001);// nie ma dla pic32mm
// Return Result
return   res;
}

unsigned int NVMWriteDoubleWord(void* address, unsigned int data_l, unsigned int data_h)
{
unsigned int res;
// Load data into NVMDATA register
NVMDATA0 = data_l;
NVMDATA1 = data_h;
// Load address to program into NVMADDR register
NVMADDR = (unsigned int) address;
// Unlock and Write Word
res = NVMUnlock (0x4010);
// Return Result
return
res;

}