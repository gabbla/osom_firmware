# Speedometer firmware

## Packets

Every packet, both received or transmitted, is made as follow:

Pos|Len|Meaning|Value
--|--|--|--
0|2|Preamble|0x534D
2|2|Message ID|
4|1|Packet length|
5|1|Command ID|
6|n|Payload|

### Commands

#### Ping `1`


