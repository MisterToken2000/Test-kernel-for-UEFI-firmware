#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DevicePathLib.h>
#include <Protocol/LoadedImage.h>
#include <Protocol/SimpleFileSystem.h>
#include <Guid/FileInfo.h>
#include <stdbool.h>
#include <stdint.h>

EFI_STATUS CatFile(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable, CHAR16* FilePath, UINT8 *MachineCode) {
    EFI_STATUS Status;
    EFI_FILE_IO_INTERFACE *FileSystem;
    EFI_FILE_HANDLE Root, File;
    EFI_LOADED_IMAGE *LoadedImage;
    EFI_GUID LoadedImageProtocol = LOADED_IMAGE_PROTOCOL;
    EFI_GUID FileSystemProtocol = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;
    EFI_GRAPHICS_OUTPUT_PROTOCOL *GraphicsOutput;

    // Получаем GraphicsOutputProtocol
    Status  = SystemTable->BootServices->LocateProtocol(&gEfiGraphicsOutputProtocolGuid, NULL, (void**)&GraphicsOutput);
    if (EFI_ERROR(Status)) {
        Print(L"[ERR] Не удалось найти GOP\n");
        return Status;
    }

    // Получаем информацию о загруженном образе
    Status = SystemTable->BootServices->HandleProtocol(ImageHandle, &LoadedImageProtocol, (void **)&LoadedImage);
    if (EFI_ERROR(Status)) {
        Print(L"[ERR] Не удалось получить LOADED_IMAGE_PROTOCOL\n");
        return Status;
    }

    // Получаем файловую систему
    Status = SystemTable->BootServices->HandleProtocol(LoadedImage->DeviceHandle, &FileSystemProtocol, (void **)&FileSystem);
    if (EFI_ERROR(Status)) {
        Print(L"Failed to get file system protocol\n");
        return Status;
    }

    // Открываем корневую директорию
    Status = FileSystem->OpenVolume(FileSystem, &Root);
    if (EFI_ERROR(Status)) {
        Print(L"Failed to open file system root\n");
        return Status;
    }

    // Открываем файл ядра
    Status = Root->Open(Root, &File, FilePath, EFI_FILE_MODE_READ, 0);
    if (EFI_ERROR(Status)) {
        Print(L"Failed to open file: %s\n", FilePath);
        return Status;
    }

    // Получаем информацию о файле
    EFI_FILE_INFO *FileInfo;
    UINTN FileInfoSize = sizeof(EFI_FILE_INFO) + 512;
    FileInfo = AllocatePool(FileInfoSize);
    if (!FileInfo) return EFI_OUT_OF_RESOURCES;

    Status = File->GetInfo(File, &gEfiFileInfoGuid, &FileInfoSize, FileInfo);

    if (EFI_ERROR(Status)) {
        Print(L"Failed to get file information\n");
        FreePool(FileInfo);
        return Status;
    }

    UINTN FileSize = FileInfo->FileSize;
    VOID *buffer = AllocatePool(FileSize);
    if (!buffer) {
        FreePool(FileInfo);
        return EFI_OUT_OF_RESOURCES;
    }

    // Чтение файла в буфер
    Status = File->Read(File, &FileSize, buffer);
    if (EFI_ERROR(Status)) {
        Print(L"Failed to read file\n");
        FreePool(buffer);
        FreePool(FileInfo);
        return Status;
    }

    Print(L"Read %d bytes from %s\n", FileSize, FilePath);

    CHAR8 *TextBuffer = (CHAR8 *)buffer;

    UINTN Index = 0;

    for (UINTN i = 0; i < FileSize; i += 16) {
        
        for (UINTN j = 0; j < 16; j++) {
                if (i + j < FileSize) {
                        UINT8 byte = TextBuffer[i + j] & 0xff;
			MachineCode[Index++] = byte;
			// Print(L"%x ", byte);

		} else {
                        Print(L"");
                }
        }
    }

    // Не забудьте освободить ресурсы
    FreePool(buffer);
    FreePool(FileInfo);
    File->Close(File);
    Root->Close(Root);
    return EFI_SUCCESS;
}

EFI_STATUS Set_Screen(EFI_GRAPHICS_OUTPUT_PROTOCOL *GraphicsOutput, UINTN Width, UINTN Height){
    EFI_STATUS Status;
    UINTN SizeOfInfo;
    EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *Info;
    uint32_t ModeNumber;
    unsigned int i;
    for (i = 0; i < GraphicsOutput->Mode->MaxMode; i++) {
        GraphicsOutput->QueryMode(GraphicsOutput, i, &SizeOfInfo, &Info);
        if (EFI_ERROR(Status)) {
            continue;
        }

        // Ищем нужное разрешение, например 1920x1080
        if (Info->HorizontalResolution == Width && Info->VerticalResolution == Height) {
            ModeNumber = i;
            // Переключаемся на нужный режим
            GraphicsOutput->SetMode(GraphicsOutput, ModeNumber);
            if (!EFI_ERROR(Status)) {
                Print(L"Resolution set to 1920x1080");
            } else {
                Print(L"Failed to set resolution");
            }
            break;
        }
    }
    return EFI_SUCCESS;
}

EFI_STATUS Set_Background_Color(EFI_GRAPHICS_OUTPUT_PROTOCOL GraphicsOutput, UINT32 Color) {
    UINT32 Pitch = GraphicsOutput.Mode->Info->PixelsPerScanLine;
    UINT32 x = 0;
    UINT32 y = 0;
    UINT32 Scale = 1920;

    UINT8 Pix[1] = {0b10000000}; // закрашен самый левый пиксель

    UINT32 *FrameBuffer = (UINT32 *)GraphicsOutput.Mode->FrameBufferBase;

    for (UINTN Row = 0; Row < 1; Row++) {
        for (UINTN Col = 0; Col < 8; Col++) {
            if (Pix[Row] & (1 << (7 - Col))) {
                for (UINTN dy = 0; dy < Scale; dy++) {
                    for (UINTN dx = 0; dx < Scale; dx++) {
                        UINTN screen_y = y + Row * Scale + dy;
                        UINTN screen_x = x + Col * Scale + dx;
                        FrameBuffer[screen_y * Pitch + screen_x] = Color;
                    }
                }
            }
        }
    }

    return EFI_SUCCESS;
}

EFI_STATUS EFIAPI UefiMain(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable) {
    	EFI_STATUS Status;

	gBS = SystemTable->BootServices;
    	// UEFI apps automatically exit after 5 minutes. Stop that here
    	gBS->SetWatchdogTimer(0, 0, 0, NULL);

    	EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *TextOutputProtocol;

    	Status = gBS->LocateProtocol(
    		&gEfiSimpleTextOutProtocolGuid, // GUID нужного протокола
    		NULL,
    		(VOID **)&TextOutputProtocol
    	);

	SystemTable->ConOut->OutputString(SystemTable->ConOut, L"\nText Output Protocol initialized\n");

	Print(L"\nText Output Protocol initialized\nSystemTable->ConOut->OutputString Address: %x\nSystemTable->ConOut Address: %x\n", SystemTable->ConOut->OutputString, SystemTable->ConOut);

	EFI_GRAPHICS_OUTPUT_PROTOCOL *GraphicsOutput;

        Status = gBS->LocateProtocol(
                &gEfiGraphicsOutputProtocolGuid,
                NULL,
                (VOID **)&GraphicsOutput
        );

	UINT32 *FrameBuffer = (UINT32*)GraphicsOutput->Mode->FrameBufferBase;

	UINT32 Pitch = GraphicsOutput->Mode->Info->PixelsPerScanLine;
	// Status = Set_Background_Color(*GraphicsOutput, 0x00000000);
	Status = Set_Screen(GraphicsOutput, 1920, 1080);
	Print(L"\nFrameBuffer Address: 0x%x\nPitch: %x\n", FrameBuffer, Pitch);

        UINT32 ScreenWidth = GraphicsOutput->Mode->Info->HorizontalResolution;
        UINT32 ScreenHeight = GraphicsOutput->Mode->Info->VerticalResolution;

        Print(L"\n%dx%d\n", ScreenWidth, ScreenHeight);

    	EFI_TIME Time;
    	EFI_TIME_CAPABILITIES Capabilities;

    	Status = gRT->GetTime (&Time, &Capabilities);

    	Print(L"%04d-%02d-%02d %02d:%02d:%02d\n", Time.Year, Time.Month, Time.Day, Time.Hour, Time.Minute, Time.Second);

    	Print(L"UEFI started.\n");

	UINT8 MachineCode[4096] = {};

	Status = CatFile(ImageHandle, SystemTable, L"\\System\\Kernel\\kernel", MachineCode);

    	VOID *ExecMem = NULL;

    	// 2. Выделяем исполняемую память (одна страница = 4 KiB)
    	Status = gBS->AllocatePages(
        	AllocateAnyPages,
        	EfiLoaderCode,
        	1,
        	(EFI_PHYSICAL_ADDRESS*)&ExecMem
    	);

    	if (EFI_ERROR(Status)) {
        	Print(L"AllocatePages failed: %r\n", Status);
        	return Status;
    	}

    	// 3. Копируем код в выделенную память
    	CopyMem(ExecMem, MachineCode, sizeof(MachineCode));

    	// 4. Объявляем тип указателя на функцию без аргументов
	UINT64 (*Func)(UINT64) = (UINT64 (*)(UINT64))ExecMem;
	
    	Print(L"Jumping to machine code...\n");

    	Func((UINT64)(UINTN) FrameBuffer); // Переход и возврат

    	// Print(L"Machine code returned: 0x%lx\n", Result);

    	while (1);
}
