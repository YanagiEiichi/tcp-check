## The tcp-check

The **tcp-check** is used to check the status of TCP `LISTENING` port.
It will not change the remote socket to `ESTABLISHED` state.

## Background

In the past, if you want to check a `LISTENING` port, you'll have to:

1. try to connect remote port.
2. close immediately after success.

But this makes connection `ESTABLISHED`.
Even if you close immediately after connect,
system will still go through the [4-way shaking](https://en.wikipedia.org/wiki/Transmission_Control_Protocol#Connection_termination) to close,
and the client socket will also change to `TIME_WAIT` state.

In `TIME_WAIT` state, the socket will wait for 2 [MSL](https://en.wikipedia.org/wiki/Maximum_segment_lifetime) (about 2 minutes).
A huge amount of `TIME_WAIT` sockets may run out of your ports.

## Solution

To avoid remote socket being turned to `ESTABLISHED`, set the `TCP_QUICKACK` to `0`.
The client will not send ACK in [3-way shaking](https://en.wikipedia.org/wiki/Transmission_Control_Protocol#Connection_establishment).

To avoid [4-way shaking](https://en.wikipedia.org/wiki/Transmission_Control_Protocol#Connection_termination) and `TIME_WAIT`,
enable the `SO_LINGER` option and set `l_linger` to `0`.
The client closing packet will use RST to replace FIN, which makes socket return directly to `CLOSED` state.

## Usage

```js
const tcpCheck = require('tcp-check');

tcpCheck('127.0.0.1', 80).then(() => {

  console.log('ok, your 80 port is listening.');

}, error => {

  console.log(':joy:, your 80 port is not listening.');

});
```
