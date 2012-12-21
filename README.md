Hi, this is a "Hello, world!" type of module code for Linux kernel. It creates a char driver, which prints "Hello, world!" when read. 

I wrote it while reading LDD3. I think printk'ing doesn't represent a "Hello, world!" example for writing device drivers. I wrote a [tutorial] (http://aciliketcap.blogspot.com/2012/12/linux-device-driver-tutorial.html) on writing Linux device drivers which follows this code. You can get more information about this module. It may contain some useful information too if you are new to writing modules for Linux kernel.

You need a few things to do after compiling the module. 

1. It registers the device with major 100 and minor 0. You need to make sure these major number is available by reading /proc/devices. If 100 is free no problem, else change the number in MKDEV and recompile (or if you can unplug the module occupying 100)

2. Run the makeconfile script, it will first try to remove /dev/scully file if there is one and then add a driver file to /dev/scully. 

After that you can read the /dev/scully file. If you use "cat /dev/scully" it will print indefinitely. You can also try "dd if=/dev/scully of=output bs=13 count=1" and print the contents of output.

Finally this distribution of code is licensed under GNU General Public License version 3. scully driver is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details. You should have received a copy of it in a file named LICENSE, if not see (http://www.gnu.org/licenses/).
