# Repository

When projects are ready I usually make them available in my repositories:
* One hosted by [GemFury](https://gemfury.com/) which is deprecated. 
* One self hosted on a virtual public server [repo.vfrott.ca](https://repo.vfrott.ca).

I have made meta-packages, packages that you can download from git hub and install to set up access to the package repository.

## [repo.vfrott.ca](https://repo.vfrott.ca)

This repository is served by a four meta-packages for the architectures I build for:
<dl>
  <dt>vfrott-bookworm</dt>
  <dd>Stable packages for Raspberry Pi OS Bullseye.</dd>
  <dt>vfrott-bullseye</dt>
  <dd>Stable packages for Raspberry Pi OS Bullseye 32 and 64 bit (armhf, arm64)</dd>
  <dt>vfrott-buster</dt>
  <dd>Stable packages for Raspberry Pi OS Buster 32 bit only (armhf). This OS is no longer receiving updates, expect this part of the repository to stop receiving updates, then be removed in the future.</dd>
  <dt>vfrott-stable</dt>
  <dd>Stable packages for Debian derived distributions, currently Intel/AMD only.</dd>
</dl>

Checksums for the meta packages are listed on the repository webpage and are copied here for added confidence.
This repository is served by a three meta-packages for the architectures I build for:
<dl>
  <dt>vfrott-bullseye</dt>
  <dd>bef3ab3b61d99ce04558b8358d2194384622a8b338685095bcaacc79a5e10df9</dd>
  <dt>vfrott-buster</dt>
  <dd>b33525f1c70b03d335a08461b3db09ebfa597ad59f719ba24ce5436ca9468ad7</dd>
  <dt>vfrott-stable</dt>
  <dd>aabc13b90d250e1c6d94276c9f911faac282e98377283b9519115e960b3118df</dd>
  <dt>vfrott-bookworm</dt>
  <dd>685942d4be43d115e79d2021fef0947a3fb85e37c89e5f5c4a63395675fdb5b0</dd>
</dl>

See the repository web page for instructions and available packages.

## GemFury - Deprecated, no longer receiving updates.
There are two meta-packages (because at present I am only creating packages for two platforms:
* [`ve3ysh-repo_2.59.5_amd64.deb`](https://github.com/pa28/hamclock-systemd/releases/download/v2.59.5/ve3ysh-repo_2.59.5_amd64.deb)
for Intel/AMD architecture running a Debian derived Linxu OS;
* [`ve3ysh-repo_2.59.5_armhf.deb`](https://github.com/pa28/hamclock-systemd/releases/download/v2.59.5/ve3ysh-repo_2.59.5_armhf.deb)
for Arm architecture running a Debian derived Linux OS such as Raspberry Pi OS.

See the installation instructions for your architecture (Intel/AMD or ARM) below.

## Installation

The following commands will install the `/etc/apt/sources.list.d/ve3ysh.list` file. The contents are:
```
# Repository with HAM Radio related software writtent by VE3YSH
# This file is managed by apt package.
# See: https://github.com/pa28/hamclock-systemd
deb [trusted=yes] https://apt.fury.io/ve3ysh/ /
```

### Intel/AMD

Execute the following commands:
```
wget https://github.com/pa28/hamclock-systemd/releases/download/v2.59.5/ve3ysh-repo_2.59.5_amd64.deb
sudo apt install -f ./ve3ysh-repo_2.59.5_amd64.deb
```
### Arm 

Execute these commands:
```
wget https://github.com/pa28/hamclock-systemd/releases/download/v2.59.5/ve3ysh-repo_2.59.5_armhf.deb
sudo apt install -f ./ve3ysh-repo_2.59.5_armhf.deb
```
