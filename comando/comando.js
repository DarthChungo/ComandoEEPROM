const args = require('command-line-parser')();
const fs = require('fs');

const SerialPort = require('serialport');
const ByteLenght = require('@serialport/parser-byte-length');

const version = 1;

const options = {
    help:        args['help']        || args['h'] || false,
    interactive: args['interactive'] || args['i'] || false,
    port:        args['port']        || args['p'] || false,
    send:        args['send']        || args['s'] || false,
    recieve:     args['recieve']     || args['r'] || false
};

if (options['help'] || args.lenght == 0) {
    console.log('Usage: commando [OPTION]...');
    console.log('Connects through the specified serial port to a commando');
    console.log('EEPROM programmer to send or recieve data from it');
    console.log('');
    console.log('Available arguments:');
    console.log(' -h, --help          Show this information');
    console.log(' -i, --interactive   Use interactive mode to send data');
    console.log(' -p, --port PORT     Specify port');
    console.log(' -s, --send FILE     Send a file to the EEPROMs');
    console.log(' -r, --recieve FILE  Recieve to file from EEPROMS');

    process.exit(1);
}

if (options['interactive']) {
    console.log('comando: not implemented');
    process.exit(1);
}

if (!options['port']) {
    console.log('commando: no port specified');
    process.exit(1);
}

console.log('[Comando] Starting COMANDO protocol version ' + version);

const serial = new SerialPort(options['port'], { baudRate: 2000000 });
const parser = serial.pipe(new ByteLenght({ length: 2 }));

var receiving = false;
var sending = false;
var lenght = 0;
var buff = [];

serial.on('open', () => {
    console.log('[Serial] Oppened serial port');

})

serial.on('close', (err) => {
    console.log('[Serial] Closed port')
    process.exit(0);
})

parser.on('data', data => {
    if (receiving) {
        if (lenght >= 255) {
            receiving = false;
            
            fs.writeFile(options['recieve'], buff, err => {});

            console.log('[Comando] Finished recieving');

            serial.write([0x10, 0x00]);
            serial.close();
        
        } else {
            buff.push((data[0]<<8) | data[1]);
            lenght++;
        }

    } else if (sending) {


    } else {
        if (data[0] == 0x01) {
            console.log('[Comando] Recieved READY packet');
            console.log('[Comando] SERVER is using protocol version ' + data[1]);

            if (data[1] != version) {
                console.log('[Comando] PROTOCOL VERSION DOES NOT MATCH, giving up...')
                
                serial.write([0x10, 0x00]);
                serial.close();
            }

            if (options['recieve']) {
                serial.write([0x11, 0x00]);
                console.log('[Comando] Initiated READ handshake');
            }

        } else if (data[0] == 0x02 && data[1] == 0x00) {
            console.log('[Comando] Recieving READ data, please wait')
            receiving = true;
        }
    }
});
