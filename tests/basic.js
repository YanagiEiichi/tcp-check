const mocha = require('mocha');
const should = require('should');
const net = require('http');
const tcpCheck = require('..');

const { port } = net.createServer((socket) => {
  throw new Error(':joy:');
}).listen().address();

describe('functional tests', function() {

  it(`tcpCheck('127.0.0.1', ${port})`, () => {
    return tcpCheck('127.0.0.1', port);
  });

  it(`tcpCheck('127.0.0.1', 1)`, () => {
    return tcpCheck('127.0.0.1', 1).then(() => {
      throw new Error('must throw');
    }, error => {
      error.name.should.equal('ECONNREFUSED');
    });
  });

  it(`tcpCheck('10.0.0.0', 80)`, () => {
    return tcpCheck('10.0.0.0', 80).then(() => {
      throw new Error('must throw');
    }, error => {
      error.name.should.equal('TIMEOUT');
    });
  });

});
