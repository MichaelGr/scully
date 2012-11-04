Hİ, this is a "Hello, world!" type driver code. 

It creates a char driver, which prints "Hello, world!" when read. It doesn't have write or seek functionality. 

I wrote it while reading LDD3. I think printk'ing doesn't represent a "Hello, world!" example for writing device drivers. This code however comes closer. For people who want to see a working driver code with minimal kernel bureacracy.

You need a few things to do to make it work though. 

1. It registers the device with major 100 and minor 0. You need to make sure these major number is available by reading /proc/devices. If 100 is free no problem, else change the number in MKDEV and recompile (or if you can unplug the module occupying 100)

2. Run the makeconfile script, it will first try to remove /dev/chtest file if there is one and then add a driver file to /dev/chtest. 

After that you can read the /dev/chtest file. If you use "cat /dev/chtest" it will print indefinitely. You can also try "dd if=/dev/chtest of=output bs=13 count=1" and print the contents of output.

Finally this distribution of code is licensed under GNU General Public License version 3. Fake ARP driver is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details. You should have received a copy of it in a file named LICENSE, if not see (http://www.gnu.org/licenses/).
