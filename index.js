const AwesomeLib = require('./build/Release/awesome-lib');

AwesomeLib.awesomeJob(4, (log) => {
    console.log(log);
})
.then(res => {
    console.log("iş bitti sonuç:", res);
});
