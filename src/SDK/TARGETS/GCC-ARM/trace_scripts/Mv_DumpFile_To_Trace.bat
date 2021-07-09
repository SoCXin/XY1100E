echo off

mkdir C:\T32\DumpFile

set src_dir=\\10.0.10.2\public\Platform_Docs\压力测试\LOG\2020年03月\0324\ZYWL死机
set dst_dir=C:\T32\DumpFile

cd /d %dst_dir%

del xinyiNBSoc_M3.axf 
del Xinyi_NBIoT_Master_Branch 
del sram_M3_20000000.bin  
del sram_dsp_50042000.bin 
del dram_3FFC0000.bin
del backmem_50100000.bin


cd /d %src_dir%


copy %src_dir%\xinyiNBSoc_M3.axf      %dst_dir%\xinyiNBSoc_M3.axf

copy %src_dir%\Xinyi_NBIoT_Master_Branch*       %dst_dir%\Xinyi_NBIoT_Master_Branch

copy %src_dir%\sram_M3_20000000*.bin       %dst_dir%\sram_M3_20000000.bin

copy %src_dir%\sram_dsp_50042000*.bin       %dst_dir%\sram_dsp_50042000.bin

copy %src_dir%\dram_3FFC0000*.bin       %dst_dir%\dram_3FFC0000.bin

copy %src_dir%\bakupmem_50100000*.bin       %dst_dir%\backmem_50100000.bin


