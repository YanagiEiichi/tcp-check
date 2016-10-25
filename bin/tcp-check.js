#!/usr/bin/env node

const tcpCheck = require('..');

tcpCheck(...process.argv.slice(2)).then(result => {
  console.log(result.name); // eslint-disable-line
}, error => {
  console.error(error.name); // eslint-disable-line
  process.exit(1);
});
