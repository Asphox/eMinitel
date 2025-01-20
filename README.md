# eMinitel
Windows and Linux Minitel client emulator.

## Some images
![image](https://github.com/user-attachments/assets/dad750a4-56db-4f7e-8d7b-5572b6dda1b9)
![image](https://github.com/user-attachments/assets/d59de56f-e14f-41c2-b095-a64fef589c42)


## Description
eMinitel is a Minitel emulator for Windows and Linux.

The Minitel (Médium intéractif par numérisation d'information téléphonique) is an old french terminal connected to Videotex services, prior the World Wide Web.

Historically accessible with a telephone line, Orange (ex-France Telecom) closed the service in 2012.

With this emulator, you can for exemple : 
- connect to fan-made services implementing Videotex-over-TCP
- use it as a nice display for an Arduino/embedded project (Videotex is a lightweight protocol by current standards ...)
## What can I do ?
- Currently eMinitel can (mostly) emulate a Minitel 1B on Videotex protocol.
- Minitel's baudrate emulation for TCP connections : 300 bauds, 1200 bauds, 4800 bauds, 9600 bauds (from Minitel 2)
- ON/OFF color mode
- Debug mode with step by step execution
- Can load a .vdt file (vidéotext binary file)
- Modem emulation is NOT planified.
- Mixed mode is NOT planifid.
- Teleinformatique/VT100 protocol is NOT planified
- PAD X-3 protocol is NOT planified.

eMinitel mainly focuses on DIN emulation.
## Planned features
- More debugging options
- Serial connection
- Bluetooth connection
- Minitel 2's DRCS (Dynamical Redefinable Character Set)
- Minitel Photo's JPEG support (maybe ... note sure for this one) 
- Some non-standard Videotext extensions (mouse support for exemple)
## How to Install
- Windows: You can download and execute the latest release build !
- Linux and MacOS: No builds provided for now, but eMinitel Win64 build works really fine on Wine.
You can also build from source (needs SFML 2.6)

  You can also build from source.
## How to use it
- Launch eMinitel
- Go in "Connection" menu, click on TCP
- Write an an address and port for a TCP Minitel service (for exemple : minitel.xenoth.fr:8083 from https://github.com/Xenoth/pyminitel/)
- Connect
- Enjoy !
## Useful ressources
- Datashet for Minitel 1B (fr) : https://jbellue.github.io/stum1b/

## Minitel-core submobule
Minitel-core is an independant subset of eMinitel that can be compiled independently and used in other projects.
It implements all the Videotext/Télétel protocol and can manage backends via a C-style callback system (and so, you'll need to provide customs keyboard backend, screen backend, com backend and audio backend).
