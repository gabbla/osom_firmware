# Speedometer firmware

## Note

It's not possible to debug the project with `XC32 v2.10`. `XC32 v1.43` still works. It seems that there is a missing library.

## Packets

Every packet, both received or transmitted, is made as follow:

Pos|Len|Meaning|Value
--|--|--|--
0|2|Preamble|0x534D
2|2|Message ID|
4|1|Packet length|
5|1|Command ID|
6|n|Payload|

### Messages

The messages types are enumerated in `firmware/src/somparser.h` in the enumeration `BLECommand`. 

> Not the best name

There are 3 type groups:
- `0x00` to `0x0F` are commands reserved for the `master` only, managed by `BLEApp`;
- `0x10` to `0x4F` are generic commands to control the system;
- `0x50` to `0x??` are response messages sent by the system to the received commands;
- `0xF0` to `0xFF` are special meaning messages sent by the system

There are also some common definitions along all the exchanged messages:

TODO laser

#### `0x00` to `0x0F`

##### `0x00` Ping

- Payload length 0

Simply check if the system is up and running. TODO response

TODO other commands

#### `0x10` to `0x4F`

##### `0x10` Mode setting

- Payload length 1
Payload data:

Field|Bit|Meaning
--|--|--
Mode|7-4|Mode selection
Laser|3-0|Laser selection as defined
