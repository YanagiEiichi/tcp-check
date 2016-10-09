let __tcpCheck = require('./build/Release/tcpCheck');
let net = require('net');

class InvalidAddressError extends Error {
  constructor() {
    super('invalid ip address, must be a ipv4 string');
    this.name = 'INVALID_IP_ADDRESS';
  }
}

class InvalidPortError extends Error {
  constructor() {
    super('invalid port, must be in range [1,65535]');
    this.name = 'INVALID_PORT';
  }
}

let tcpCheck = (ip, port, timeout) => new Promise((resolve, reject) => {

  // Type converting
  ip += '';
  port |= 0;
  timeout = timeout * 1 || 1000;

  // Value checking
  if (!net.isIPv4(ip)) return reject(new InvalidAddressError());
  if (port < 1 || port > 65535) return reject(new InvalidPortError());

  // Set timer
  let timer = setTimeout(reject, timeout, { name: 'TIMEOUT' });

  // Actual check
  __tcpCheck(ip, port, result => {
    clearTimeout(timer);
    if (result.name === 'OK') {
      resolve(result);
    } else {
      reject(result);
    }
  });

});

module.exports = tcpCheck;
