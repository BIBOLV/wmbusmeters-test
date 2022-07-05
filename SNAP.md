Installation of snapd
===============================

Please refer to official documentation for snapd installation - https://snapcraft.io/docs/installing-snapd

:bangbang: | Remeber to follow documentation and install core snap - needed for interfaces to work
:---: | :---

Installation of wmbusmeters Snap
===============================

Installing a snap is straightforward:

    sudo snap install wmbusmeters
    
The stable channel is used by default, but opting to install from a edge channel is easily accomplished:

    sudo snap install --channel=edge wmbusmeters
    
In stable channel latest releases of wmbusmeters will be available, but in edge channel wmbusmeters snap will contain all latest code changes.

Using the wmbusmeters Snap
===============================

After installing the Snap, the privileged access to USB interfaces and system process listing needs to be configured:

    sudo snap connect wmbusmeters:raw-usb core:raw-usb
    sudo snap connect wmbusmeters:system-observe core:system-observe

Using wmbusmeters Daemon
-------------------------------------------

For all actions with wmbusmeters daemon, like enable/disable/start/stop/restart/status
systemctl commands should be used

For example:

    systemctl start snap.wmbusmeters.wmbusmeters.service
    systemctl status snap.wmbusmeters.wmbusmeters.service

Configuration for wmbusmeters
-----------------------------

Configuration of wmbusmeters is located in
   `/var/snap/wmbusmeters/common/etc/`

Logs are in 
   `/var/snap/wmbusmeters/common/logs/`
   
When changes are made to configuration, service must be restarted.

Those directories are persistent across updates - files will not be deleted or changed when updating snap. 
Files and directories are being deleted only when snap is removed.

:information_source: | On arm arch following error might show up - `ERROR: ld.so: object '/usr/lib/arm-linux-gnueabihf/libarmmem-${PLATFORM}.so' from /etc/ld.so.preload cannot be preloaded (cannot open shared object file): ignored.` It is just a warning and can be disabled by commenting out `/usr/lib/arm-linux-gnueabihf/libarmmem-${PLATFORM}.so` from `/etc/ld.so.preload`
:---: | :---
