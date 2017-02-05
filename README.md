# What is gmonitor?
This is a GPU monitoring program, it monitors the core usage, VRAM usage, PCI-E & memory bus usage and the temperature of the GPU.
I write CUDA programs and always needed a way to monitor how they behave in real time, searched for an MSI afterburner like programs on GNU/Linux systems but couldn't find any, so I decided to put this together and have been using it for quite some time now. Though this is quite minimal, it's 
good enough for what I needed it.


# Running environment: 
This has been developed and used in a machine running Ubuntu, I have not tested it in any other systems.

# Requirements:
At least one NVIDIA GPU, this only supports NVIDIA GPUs for now as I only needed this to monitor my CUDA programs. Support for AMD/Intel cards might come in the future if needed, feel free to add them as well.

This program probes the NVIDIA drivers for statistics through nvidia-settings, make sure the NVIDA drivers are properly installed on your system.

###### CUDA install:
Follow [Nvidia CUDA install guide](http://docs.nvidia.com/cuda/index.html) to install CUDA on your machine, If you face issues with the nouveau driver, try this [Nouveau workaround](http://askubuntu.com/questions/451221/ubuntu-14-04-install-nvidia-driver).

# Usage:
Download source code and compile, run the executable:

-d [displayMode]<br />
0 (default) Monitor both current and previous GPU states.<br />
1 Monitor most recent GPU state only.<br />
2 Monitor previous GPU states only.<br />
3 Same as 0, print current states for all GPU then print history.<br />

-g [gpuNumber]<br />
GPU numbers to monitor, primary GPU's number is 0...<br />

-r [refreshRate]<br />
Monitoring refresh rate (default is 2 seconds). <br />

-h<br />
Display this menu.<br />

###### Examples:
-d 1 -g 02<br />
Monitor the most recent state only for the first and third gpu.<br />
-d 0 -g 0123<br />
Monitor current and history states for 4 GPUs.<br />
-d 0 -r 3<br />
Monitor both current and previous states for all GPUs, refresh every 3 seconds.<br />

# Limitations:
This only supports Nvidia cards at the moment and support for other cards might be added in the future. Multi GPU support have not been tested, it should scale and work with any number of available GPUs but I have not tested it myself as I only have one card.
