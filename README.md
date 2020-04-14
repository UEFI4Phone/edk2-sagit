## Notice  
I have never posted any photos/videos about this project on public media. All videos, photo posts, news etc, are NOT posted by me.  
我从未在网络上公开发布过任何与此项目有关的任何图片或视频，任何网络上相关视频(Bilibili/youtube)均不是我发布的，发布这些内容的账号均与我本人无关。  
edk2-sagit源码仅在此repo发布。 
**以下视频并非原创**  
>
> 视频：https://www.bilibili.com/video/av497635981（https://www.bilibili.com/video/BV1xK411L74d）  
> 个人主页：https://space.bilibili.com/341690282  
 
This info is only added due to people's requests and the fact that this being overblown, as well as some misleading infos popping up.  


Attempt to create a minimal EDK2 for Xiaomi MI 6.

Based on zhuowei's port for Pixel3XL (https://github.com/Pixel3Dev/edk2-pixel3/).

## Status 

Can load GRUB2 from a fat partition on the UFS.(/firmware is tested,you can also format the useless /cust for it)  
Windows now boots with minimal ACPI tables. (Checkout minimal-acpi branch)  

### Working
Continuous Display  
UFS Storage  
Vibrator  
...

### Not Working
Clocks   
...

## Building
Tested on Ubuntu 18.04.

First, clone EDK2.

```
cd ..
git clone https://github.com/tianocore/edk2.git --recursive
git clone https://github.com/tianocore/edk2-platforms.git
```

You should have all three directories side by side.

Next, install dependencies:

18.04:

```
sudo apt install build-essential uuid-dev iasl git nasm python3-distutils gcc-aarch64-linux-gnu abootimg
```

Also see [EDK2 website](https://github.com/tianocore/tianocore.github.io/wiki/Using-EDK-II-with-Native-GCC#Install_required_software_from_apt)

Finally, ./build.sh.

Then fastboot boot uefi.img.

# Credits

This is based on zhuowei's [edk2-pixel3](https://github.com/Pixel3Dev/edk2-pixel3).  
SimpleFbDxe screen driver is from imbushuo's [Lumia950XLPkg](https://github.com/WOA-Project/Lumia950XLPkg).  
Special thanks to @lemon1ice, @gus33000 and @imbushuo for guidance.  
