# What is gmonitor?
This program monitors the gpu usage (gpu core usage & temperature, video memory usage and PCI-E & 
memory bus usage).
I write CUDA programs and always needed to monitor how my they  behave in real time, searched for 
an MSI afterburner like programs on GNU/Linux systems but couldn't find any, so I decided to put 
this together and have been using it for quite some time now. Though this is quite minimal, it's 
good enough for what I needed it to do (monitor how my program is using resources and looking for 
bottlenecks in real time).


# Running environment: 
This has been developed and used in Ubuntu 14.04.1 operating system, I have not tested it in any 
other GNU/Linux systems.

# Requirements:
At least one NVIDIA gpu, this only supports NVIDIA gpus for now as I only needed this to monitor 
my CUDA programs. Feel free to add support for AMD cards.
This program probes the NVIDIA drivers for statistics through nvidia-settings, make sure the NVIDA 
drivers are properly installed on your system.
NVIDIA CUDA install guide: 
http://docs.nvidia.com/cuda/cuda-getting-started-guide-for-linux/index.html#axzz3JBNT4RBl
If you face issues with the nouveau driver, try: 
http://askubuntu.com/questions/451221/ubuntu-14-04-install-nvidia-driver

# Usage:
Download source code and compile, run the executable:

-d [displayMode]  displayMode: 0 (default) Monitor both current and previous gpu states.
		                       1 Monitor most recent gpu state only.
		                       2 Monitor previous gpu states only.
		                       3 Same as 0, print current states for all gpus then print history.

-g [gpuNumber]      gpuNumber: Gpu numbers to monitor, primary gpu's number is 0...

-r [refreshRate]  refreshRate: How often does the program refreshes the screen with new states (default         
                               is 2 seconds). 

-h                       Help: Display this menu.

-d 1 -g 02     Monitor the most recent state only for the first and third gpu.
-d 0 -g 0123   Monitor current and history states for all 4 gpus in a quad SLI set up.
-d 0 -r 3      Monitor both current and previous states for all gpus, refresh every 3 seconds.

# Limitations:
This does not support AMD cards, might add that in the future.
Multi gpu support have not been tested, this should scale and work with any number of available but I have 
not tested it myself as I only have one card.
