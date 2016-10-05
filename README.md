## tcp-check

### install

```bash
npm install tcp-check
```

### usage

```js
var promise = tcpCheck(ip, port);
```

### demo

```js
const tcpCheck = require('tcp-check');

tcpCheck('127.0.0.1', 80).then(() => {
  console.log('ok, your 80 port is listening.');
}, error => {
  console.log(':joy:, your 80 port is not listening.');
});
```
