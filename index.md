## Current Projects

### [Package Availability](/Repository)

When I create packages for my projects they will be available in
my repository.

### [PicoGrubKeyBoard](/PicoGrubKeyBoard)

Pi Pico - CircuitPython Program to act as Keyboard for GRUB boot

A small Python script for Raspberry Pi Pico using CircuitPython and the adafruit_hid library to provide keyboard input to the Linux GRUB boot loader.
This is uesful in my case because I'm using Logi Mx Keys Bluetooth connected keyboard which is not active at that point in the boot sequence.

### [Time-Series Data Daemons](/APRS_WX)

I use [infuxDB](www.influxdata/com) and [Grafana](grafana.com) quite
at bit for various projects. I have written some stand-alone data
collection daemons that also interface to influxDB for data storage.
These are kept in the [APRS_WX](/APRS_WX) project.

### [Rose - *Rendering On Small Environments*](/Rose)

Rose is intended, primarily as a Graphic User Interface library for
Raspberry Pi computers with a small touch screen to support application
that can run directly on the screen frame buffer `/dev/fb0` in an
*Application* or *Kiosk* mode. Rose uses [SDL2](https://www.libsdl.org/)
as the underlying graphics engine which makes it possible to run under 
desktop graphics environments (X11) and multiple OSs and distributions.

The project in the reposition builds under [Cmake](https://cmake.org/)
on Debian Linux derived distributions and produces the following 
artifacts:
* The Rose library.
* Sample programs and applications implements on Rose.
* Debian packages with:
    * The library, sample programs and application with supporting
    resources;
    * A package for developers.

### [hamclock-systemd](/hamclock-systemd) Updated.

This project creates a Debian package to install a compiled version of
[HamClock](https://www.clearskyinstitute.com/ham/HamClock/) and enable
launch on boot using
[systemd](https://www.freedesktop.org/wiki/Software/systemd/).

[Packages](/Repository) available for Debian derived Linux distribution on Intel/AMD
and ARM:
* `hamclock` for systems running an X11 based desktop UI.
* `hamclock-systemd` for systems running `/dev/fb0` graphics.

### [DSP](https://pa28.github.io/DSP/)

C++ Header only (so far) Digital Signal Processing objects including:
* Linear Time Invariant signal abstractions;
* Fast Fourier Transform;
* Demodulation loops.

## Forked Repositories

### [C++ IIR Filters](https://github.com/pa28/iir1)

Forked from [https://github.com/berndporr/iir1](https://github.com/berndporr/iir1).
Release 1.8.1 included Debian packages for amd64 and armhf architecture.
