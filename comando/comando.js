const args = require('command-line-parser')();
const promt = require('prompt-sync')();
const fs = require('fs');

const SerialPort = require('serialport');
const ByteLenght = require('@serialport/parser-byte-length');

const version = 1;

const options = {
    help:      args['help']     ||  args['h']  ||  false,
    port:      args['port']     ||  args['p']  ||  false,
    send:      args['send']     ||  args['s']  ||  false,
    recieve:   args['recieve']  ||  args['r']  ||  false,
    single:    args['single']   ||  args['s']  ||  false
};

if (options['help']) {
    console.log('Usage: commando [OPTION]...');
    console.log('Connects through the specified serial port to a commando');
    console.log('EEPROM programmer to send or recieve data from it');
    console.log('');
    console.log('Available arguments:');
    console.log(' -h, --help          Show this information');
    console.log(' -p, --port    PORT  Specify port');
    console.log(' -s, --send    FILE  Send a file to the EEPROMs');
    console.log(' -r, --recieve FILE  Recieve to file from EEPROMS');
    console.log(' -s, --single        Use lowest byte EEPROM only');

    process.exit(0);
}

if (!options['port']) {
    console.log('[Comando] No port specified');
    process.exit(1);
}

if (!(options['send'] || options['recieve'])) {
    console.log('[Comando] Warning, neither sending nor receiving data');
}

if (fs.existsSync(options['recieve'])) {
    if (promt('[Comando] Warning, file ' + options['recieve'] + ' exists, overwrite? (y/N) ') != 'y') {
        process.exit(1);
    }
}

console.log('[Comando] Starting COMANDO protocol version ' + version);

const serial = new SerialPort(options['port'], { baudRate: 2000000 });
const parser = serial.pipe(new ByteLenght({ length: 2 }));

var buff = Buffer.alloc(options['single'] ? 256 : 256*2);
var receiving = false;
var pos = 0;

serial.on('open', () => {
    console.log('[Serial] Oppened serial port');
})

serial.on('close', (err) => {
    console.log('[Serial] Closed port')
    process.exit(0);
})

parser.on('data', data => {
    if (receiving) {
        if (options['single']) {
            buff.writeUInt8(data[1], pos);

        } else {
            buff.writeUInt8(data[0], (pos*2)  );
            buff.writeUInt8(data[1], (pos*2)+1);
        }

        pos++;

        if (pos == 256) {
            receiving = false;
            
            fs.writeFile(options['recieve'], buff, 'binary', err => {});
            console.log('[Comando] Finished recieving');

            console.log('[Comando] Terminating connection');
            serial.write([0x10, 0x00]);
            serial.close();
        } 

    } else {
        if (data[0] == 0x01) {
            console.log('[Comando] SERVER is ready');
            console.log('[Comando] SERVER is using protocol version ' + data[1]);

            if (data[1] != version) {
                console.log('[Comando] Error, protoco versions do not match, giving up...')
                
                console.log('[Comando] Terminating connection');
                serial.write([0x10, 0x00]);
                serial.close();
            }

            if (options['recieve']) {
                serial.write([0x11, 0x00]);
                console.log('[Comando] Initiated READ handshake');

            } else if (options['send']) {
                serial.write([0x12, 0x00]);
                console.log('[Comando] Initiated WRITE handshake');
                
                console.log('[Comando] Sending WRITE data')

                for (let i = 0; i != 256; i++) {
                    serial.write([0x00, 0x00]); // TODO: Send correct data                 
                }

            } else {
                console.log('[Comando] Terminating connection');
                serial.write([0x10, 0x00]);
                serial.close();
            }

        } else if (data[0] == 0x02 && data[1] == 0x00) {
            console.log('[Comando] Receiving READ data, please wait')
            receiving = true;
        } else if (data[0] == 0x03 && data[1] == 0x00) {
            console.log('[Comando] Done sending data');
            console.log('[Comando] Terminating connection');
            serial.write([0x10, 0x00]);
            serial.close();
        }
    }
});
