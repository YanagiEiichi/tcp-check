let __tcpCheck = require('./build/Release/tcpCheck');
let net = require('net');
let dns = require('dns');

class InvalidPortError extends Error {
  constructor() {
    super('invalid port, must be in range [1,65535]');
    this.name = 'INVALID_PORT';
  }
}

let tcpCheck = (host, port, timeout) => new Promise((resolve, reject) => {

  let timer = setTimeout(reject, timeout * 1 || 1000, { name: 'TIMEOUT' });

  host += '';
  port |= 0;

  if (port < 1 || port > 65535) return reject(new InvalidPortError());

  const onLookup = (error, ip) => {
    if (error) return reject({ name: error.code });
    actualCheck(ip, port);
  };

  const actualCheck = (ip, port) => {
    __tcpCheck(ip, port, result => {
      clearTimeout(timer);
      if (result.name === 'OK') {
        resolve(result);
      } else {
        reject(result);
      }
    });
  };

  net.isIPv4(host) ? actualCheck(host, port) : dns.lookup(host, { family: 4, timeout: 100 }, onLookup);

});

module.exports = tcpCheck;
