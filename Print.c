#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>

EFI_STATUS EFIAPI UefiEntry(IN EFI_HANDLE imgHandle, IN EFI_SYSTEM_TABLE* sysTable)
{
    	gST = sysTable;
    	gBS = sysTable->BootServices;
    	gImageHandle = imgHandle;
    	// UEFI apps automatically exit after 5 minutes. Stop that here
    	gBS->SetWatchdogTimer(0, 0, 0, NULL);
    	Print(L"Hello, world!\r\n");
	while (1);
}
