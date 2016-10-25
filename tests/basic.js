const mocha = require('mocha');
const should = require('should');
const net = require('http');
const tcpCheck = require('..');

const { port } = net.createServer((socket) => {
  throw new Error(':joy:');
}).listen().address();

describe('functional tests', function() {

  it(`tcpCheck('127.0.0.1', ${port}) OK`, () => {
    return tcpCheck('127.0.0.1', port);
  });

  it(`tcpCheck('github.com', 80) OK`, () => {
    return tcpCheck('github.com', 80);
  });

  it(`tcpCheck('127.0.0.1', 1) ECONNREFUSED`, () => {
    return tcpCheck('127.0.0.1', 1).then(() => {
      throw new Error('must throw');
    }, error => {
      error.name.should.equal('ECONNREFUSED');
    });
  });

  it(`tcpCheck('127.0.0.1', 0) INVALID_PORT`, () => {
    return tcpCheck('127.0.0.1', 0).then(() => {
      throw new Error('must throw');
    }, error => {
      error.name.should.equal('INVALID_PORT');
    });
  });

  it(`tcpCheck('0.0.0.1', 80) EHOSTUNREACH`, () => {
    return tcpCheck('0.0.0.1', 80).then(() => {
      throw new Error('must throw');
    }, error => {
      error.name.should.equal('EHOSTUNREACH');
    });
  });

  it(`tcpCheck('x.x', 80) ENOTFOUND`, () => {
    return tcpCheck('x.x', 80).then(() => {
      throw new Error('must throw');
    }, error => {
      error.name.should.equal('ENOTFOUND');
    });
  });

  it(`tcpCheck('x.com', 80) TIMEOUT`, () => {
    return tcpCheck('x.com', 80).then(() => {
      throw new Error('must throw');
    }, error => {
      error.name.should.equal('TIMEOUT');
    });
  });

});
