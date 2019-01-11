const curl = require("../index");

module.exports = function(cnt, urlStr) {
  let now = Date.now();
  let cur;

  // Warm up server.
  for (let i = 0; i < 5; i++) {
    curl(urlStr);
  }

  for (let i = 0; i < cnt; i++) {
    cur = Date.now();

    console.log(curl(urlStr));

    console.log("Count: " + i);
    console.log("Spend: " + (Date.now() - cur));
  }
  let total = Date.now() - now;
  console.log("libcurl Average: " + total / cnt);
};
