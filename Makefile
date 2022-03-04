GCCPARAMS = -m32  -fno-use-cxa-atexit  -fno-leading-underscore -fno-exceptions  -fno-builtin -fno-rtti -nostdlib 
ASPPARAMS = --32
LDPARAMS = -melf_i386

objects = loader.o kernel.o gdt.o

%.o: %.cpp
	g++ ${GCCPARAMS} -o $@ -c $<

%.o: %.s
	as ${ASPPARAMS} -o $@ $<

mykernel.bin: linker.ld ${objects}
	ld ${LDPARAMS} -T $< -o $@ ${objects}

install:
	sudo cp $c /boot/mykernel.bin

mykernel.iso: mykernel.bin
	mkdir iso
	mkdir iso/boot
	mkdir iso/boot/grub
	cp mykernel.bin iso/boot
	echo 'set timeout=0' > iso/boot/grub/grub.cfg
	echo 'set default=0' >> iso/boot/grub/grub.cfg
	echo '' >> iso/boot/grub/grub.cfg
	echo 'menuentry "my os" {' >> iso/boot/grub/grub.cfg
	echo '	multiboot /boot/mykernel.bin' >> iso/boot/grub/grub.cfg
	echo '	boot' >> iso/boot/grub/grub.cfg
	echo '}' >> iso/boot/grub/grub.cfg
	grub-mkrescue -o $@ iso
	rm -rf iso


	