# Command Line Course

This directory contains files associated with my Unix/Linux command line course. The primary audience is
Amateur Radio operators using Raspberry Pi but most of the material will apply to most Unix derived command
lines though applicability will decay as the OS appears further down this list:

1. Raspberry Pi OS
2. Debian derived Linux
3. Red Hat derived Linux
4. BSD/System V Derived Unix
5. Mac OS
6. QNX
7. etc.

This file contains general guidance on how to access the command line on a Raspberry Pi. If you are more
interested in learning the command line on a standard desktop Linux/Unix machine most installations will
have everything needed is built in or easily installed.

## Accessing the Command Line.

### Raspberry Pi OS Variants

Raspberry Pi OS comes in two main variants:
* The standard OS which includes a Graphical User Interface (GUI); and
* Raspberry Pi OS Lite which does not include a GUI.

Both of these variants may be run as a desktop system, or as a headless system.

For some setups networking and the internet won't be required however, both are recommended so we can cover 
commands that access the network and the internet.

### Desktop System

A Desktop system is what most people consider a computer. It has a system unit with the CPU, memory and storage
connected to a display monitory, keyboard and mouse. It may also be connected to a network and other devices.
For the purposes of this course I include laptop computers in the Desktop category.

If your Raspberry Pi (or the Linux/Unix system you want to learn) is a full Desktop System you are all set.

### Headless System

In some situations it is desirable to run a computer without the monitor, keyboard and mouse. These are often 
refereed to as *headless* systems. In these cases the user interacts with the computer software through
specialized controls or from a separate desktop system via the network.

If your Raspberry Pi is headless then you will need access to another desktop or laptop system that can be
networked with the Pi. There are two simple ways to access a Raspberry Pi over a network (and several
complicated ways).

#### Raspberry Pi Connect
[Raspberry Pi Connect](https://www.raspberrypi.com/software/connect/) is a facility provided by the Raspberry
Pi Foundation free of charge. It allows you to connect from any computer with a modern web browser to a Raspberry
Pi as long as both are able to make an outbound connection to the internet and the Connect server. If your Pi
is running the full OS you may connect to the GUI Desktop with screen sharing and launch a terminal emulator, or 
with a remote shell.

#### Terminal Emulator and SSH - Linux / Unix / Windows
The other way is to place your Pi on the same local network as a Desktop or Laptop system that supports a
terminal emulator and the *ssh* command. Most (if not all) Linux/Unix based systems will come with these
installed, or they can be easily installed.

Windows comes with a built-in SSH client installed by default. You can access it through the Command Prompt or
[Windows Terminal](https://learn.microsoft.com/en-us/windows/terminal/install). You could also use the [Windows
Subsystem for Linux (WSL)](https://ubuntu.com/desktop/wsl). These options are beyond the scope of this course.
