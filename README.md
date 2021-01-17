# EEPROM Manager

<p style='text-align: justify;'>

This package contains both the code for an arduino based serial command dispatcher that writes to EEPROMs and a manager for reading and uploading data to them. It is desiged for use **exclusively** with my custom circuit.

</p>

<p style='text-align: justify;'>

## Explanation of the COMMANDO protocol v0.1:

### Considerations:

CLIENT represents the comando program.<br>
SERVER represents the arduino controller.<br>
All data transmitted as little endian.<br>
BYTE represents 8 bits, WORD represents 16 bits.<br>
The serial port will operate with a baud rate of 2000000<br>

### Start

CLIENT opens the serial port<br>
SERVER opens the serial port
SERVER sends 0x01 BYTE when ready<br>
SERVER sends the version BYTE (0x01 for v0.1)

CLIENT may now... (1)<br>

### Terminate the connection:<br>
CLIENT sends 0x1000 WORD
SERVER closes the serial port
CLIENT closes the serial port
Go back to (1)

### Read data from EEPROMs:<br>
CLIENT sends 0x1100 WORD
SERVER sends 0x0200 WORD
SERVER sends exactly 256 WORDS with data
Go back to (1)

### Write data to EEPROMs:<br>
CLIENT sends 0x1200 WORD
CLIENT sends exactly 256 WORDS with data
SERVER sends 0x0300 WORD
Go back to (1)

 </p>
