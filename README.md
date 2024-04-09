# eMinitel
Windows and Linux Minitel client emulator.
## Description
eMinitel is a Minitel emulator for Windows and Linux.

The Minitel (Médium intéractif par numérisation d'information téléphonique) is an old french terminal connected to Videotex services, prior the World Wide Web.

Historically accessible with a telephone line, Orange (ex-France Telecom) closed the service in 2012.

With this emulator, you can for exemple : 
- connect to fan-made services implementing Videotex-over-TCP
- use it as a nice display for an Arduino/embedded project (Videotex is a lightweight protocol by current standards ...)
## Compatibility
- Currently eMinitel can (mostly) emulate a Minitel 1B on Videotex protocol.
- Modem emulation is currently not planified.
- Mixed mode is not planifid.
- Teleinformatique protocol is not planified
- PAD X-3 protocol is not planified.

eMinitel mainly focuses on DIN emulation.
## Incoming features
- Step by step debug rendering mode
- Minitel's true baudrate emulation for TCP connections
- Serial connections
- Minitel 2 DRCS (Dynamical Redefinable Character Set)
## How to Install
Simply download the latest release for your operating system !
## How to use it
- Launch eMinitel
- Go in "Connection" menu, click on TCP
- Write an an address and port for a TCP Minitel service (for exemple : minitel.xenoth.fr:8083 from https://github.com/Xenoth/pyminitel/)
- Connect
- Enjoy !
## Useful ressources
- Datashet for Minitel 1B (fr) : https://jbellue.github.io/stum1b/
