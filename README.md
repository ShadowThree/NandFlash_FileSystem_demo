## 参考
1. [ARM File System Component Overview](https://www.keil.com/pack/doc/mw/FileSystem/html/index.html)
2. [ARM API reference](https://www.keil.com/pack/doc/mw/FileSystem/html/fs_function_reference.html)
3. [ARM File System Examples](https://www.keil.com/pack/doc/mw/FileSystem/html/fs_examples.html)

## 说明
1. 由于通过`ARM Run-Time Environment`配置的`FreRTOS`产生的工程中，只能引用`ARM\Packs\ARM\CMSIS-FreeRTOS\10.5.1\Source\portable\RVDS`中相应架构的文件，这个文件只能在`ARM compiler V5`中使用。`ARM compiler V5`的编译速度太太太慢了。。。使用`ARM compiler V6`的编译速度快了非常多！但是`ARM compiler V6`需要引用的文件为`ARM\Packs\ARM\CMSIS-FreeRTOS\10.5.1\Source\portable\GCC`中相应架构的文件。我们只需要将`RVDS`中相应的文件备份一下，把`GCC`中相应文件复制到`RVDS`下即可。