# prepdisk

The 'prepdisk' tool installs the provided bootloader onto a pre-partitioned disk, saving the current partition information. The tool installs legacy BIOS bootloaders, and supports both the MBR and GPT partition table formats.

## Basic Functionality

```
$ prepdisk btldr disk.img
```
This will install the executable 'btldr' onto the DOS (MBR) partitioned disk.img. The formatted partition information will be recommitted to the image along with the bootloader to be installed.

It is important that the bootloader itself is formatted correctly. In the DOS (MBR) partition table, the bootloader sections are not contiguous: the partition information is intermixed. Therefore, the bootloader *must* have the proper jmp instructions to skip these table entries. In addition, these table entries should be zero'd out in the bootloader.

The tool will perform a simple OR operation on the bootloader and the MBR, performing an integrity check on the partition data (reporting any errors and quitting), and finally committing the transaction to the disk image.

