const args = require('command-line-parser')();

function PrintHelp() {
    console.log('Usage: commando [OPTION]...');
    console.log('Connects through the specified serial port to a commando');
    console.log('EEPROM programmer to send or recieve data from it.');
    console.log('');
    console.log('Available arguments:');
    console.log(' -h, --help          Show this information.');
    console.log(' -i, --interactive   Use interactive mode to send data.');
    console.log(' -p, --port=PORT     Specify port.');
    console.log(' -s, --send=FILE     Send a file to the EEPROMs.');
    console.log(' -r, --recieve=FILE  Recieve to file from EEPROMS.');
    console.log(' -a, --ascii         Use ascii format instead of binary.');
    console.log(' -n, --no-check      Avoid check of written data.')
}

console.log(args);

if (args['help'] || args['h'] || args.lenght == 0) {
    PrintHelp();
    process.exit(1);
}
