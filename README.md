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

### Start

CLIENT opens the serial port.<br>
SERVER sends 0xa1 BYTE when ready.<br>

CLIENT may now... (1)<br>

### Write data to EEPROMs:<br>
CLIENT sends 0x80 BYTE signifying the intent to send data.<br>
SERVER sends 0xa1 BYTE when ready to recieve.<br>
CLIENT sends exactly 256 WORDS to write to the EEPROMs.<br>
SERVER sends exactly 256 WORDS of data read after writing.<br>
Jump back to (1)<br>

### Read data from EEPROMs:<br>
CLIENT sends 0x90 BYTE signifying the intent to recieve.<br>
SERVER sends exactly 256 WORDS of data read.<br>
Jump back to (1)<br>

### Terminate the connection:<br>
CLIENT sends 0x80 BYTE wanting to close the connection.<br>
SERVER sends 0xa2 BYTE to aknowledge.<br>
CLIENT closes the serial port.<br>
SERVRE ends serial transmition.<br>

In beetween all parts of the protocol SERVER may send<br>
BYTE 0xa0 to signify a CLIENT error when sending.<br>
Then, SERVER ends the serial tranmition.<br>



 </p>