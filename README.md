# SeasonOS
##  简介 ##
SeasonOS是一个简单的学习用途操作系内核。它简单有趣、人畜无害、环境友好。

## 编译和调试方法 ##
### 1. 安装 `build-essential`

### 2. 安装 `nasm` 汇编器

### 3. 安装 `bochs` 仿真器

使用源代码编译的方法。

bochs官网：[http://bochs.sourceforge.net/](http://bochs.sourceforge.net/)

最新源代码下载：[http://sourceforge.net/projects/bochs/files/bochs/2.6.7/bochs-2.6.7.tar.gz/download](http://sourceforge.net/projects/bochs/files/bochs/2.6.7/bochs-2.6.7.tar.gz/download)

用户手册(编译安装指南):[http://bochs.sourceforge.net/cgi-bin/topper.pl?name=New+Bochs+Documentation&url=http://bochs.sourceforge.net/doc/docbook/user/index.html](http://bochs.sourceforge.net/cgi-bin/topper.pl?name=New+Bochs+Documentation&url=http://bochs.sourceforge.net/doc/docbook/user/index.html)

编译之前，安装相关库：

    xorg-dev
    gtk-2.0-dev

未安装前者会报错：

    ERROR: X windows gui was selected, but X windows
    
未安装后者会报错：

    gtk/gtk.h: No such file or directory

解压源代码：

    tar -xf bochs-2.6.7.tar.gz

配置：    

    ./configure --enable-debugger --enable-disasm
   
可选配置：
    
    --with-nogui
    
不使用GUI，即能在终端中运行bochs但是无法显示视频输出。其他配置内容参考上面的用户手册链接。

修改Makefile的86、87行,在后面加上`-pthread`参数，即：

    CC = gcc -pthread
    CXX = g++ -pthread
    
否则编译报错：

        g++ -o bochs -g -O2 -D_FILE_OFFSET_BITS=64 -D_LARGE_FILES -DBX_SHARE_PATH=\"/usr/local/share/bochs\" logio.o main.o config.o load32bitOShack.o pc_system.o osdep.o plugin.o crc.o -Wl,--export-dynamic  iodev/libiodev.a iodev/display/libdisplay.a iodev/hdimage/libhdimage.a bx_debug/libdebug.a cpu/libcpu.a cpu/cpudb/libcpudb.a memory/libmemory.a gui/libgui.a disasm/libdisasm.a cpu/fpu/libfpu.a -lX11 -lXpm -lXrandr -lgtk-x11-2.0 -lgdk-x11-2.0 -latk-1.0 -lgio-2.0 -lpangoft2-1.0 -lpangocairo-1.0 -lgdk_pixbuf-2.0 /usr/lib/x86_64-linux-gnu/libcairo.so -lpango-1.0 -lfontconfig -lgobject-2.0 -lglib-2.0 /usr/lib/x86_64-linux-gnu/libfreetype.so -Wl,--rpath -Wl,/usr/lib/x86_64-linux-gnu -Wl,--rpath -Wl,/usr/lib/x86_64-linux-gnu
    /usr/bin/ld: gui/libgui.a(gtk_enh_dbg_osdep.o): undefined reference to symbol 'pthread_create@@GLIBC_2.2.5'
    //lib/x86_64-linux-gnu/libpthread.so.0: error adding symbols: DSO missing from command line
    collect2: error: ld returned 1 exit status
    make: *** [bochs] Error 1

安装：

    sudo make install
    
这样：

    /usr/local/bin                  存放 binary executables (bochs, bximage)
    /usr/local/lib/bochs/plugins    存放 plugins (if present)
    /usr/local/man/man1             存放 manpages for installed binaries
    /usr/local/man/man5             存放 manpage for the config file (bochsrc)
    /usr/local/share/bochs          存放 BIOS images, VGABIOS images, keymaps
    /usr/local/share/doc/bochs      存放 HTML docs, license, readme, changes, bochsrc sample
    
### 4. 安装vgabios

### 5. 编译、调试、运行season
只需一个命令：

    make 

因为启用了调试模式，需要键入`c`以继续运行系统。

