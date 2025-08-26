MyEfiApp:
	cd ..
	build -a X64 -t GCC5 -p MdeModulePkg/MdeModulePkg.dsc
	rm ~/edk2tutor/iso/EFI/Boot/bootx64.efi
	cp ~/edk2tutor/edk2/Build/MdeModule/DEBUG_GCC5/X64/MyEfiApp.efi ~/edk2tutor/iso/EFI/Boot/bootx64.efi
	xorriso -as mkisofs \
                -R -J -l -D \
                -partition_offset 16 \
                -eltorito-alt-boot ~/edk2tutor/iso/EFI/Boot/bootx64.efi \
                -no-emul-boot -isohybrid-gpt-basdat \
                -o edk2-app-amd64.iso ~/edk2tutor/iso/	
kernel:
	gcc -m64 -ffreestanding -fno-pic -fno-stack-protector -nostdlib -c Kernel.c -o Kernel.o
	ld  -melf_x86_64 -o ~/edk2tutor/iso/System/Kernel/kernel -Ttext 0x1000 Kernel.o --oformat binary -T kernel_linker.ld
