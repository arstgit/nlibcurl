const http = require("http");
const url = require("url");

const addon = require("../build/Release/module");
module.exports = function(cnt, urlStr) {
  const count = cnt;
  const options = new url.URL(urlStr);

  now = Date.now();
  next(cnt);
  let i = 0;
  function next(cnt) {
    cur = Date.now();
    const req = http.request(options, res => {
      res.setEncoding("utf8");
      res.on("data", chunk => {
        console.log(chunk);
      });
      res.on("end", () => {
        console.log("Count: " + i++);
        console.log("Spend: " + (Date.now() - cur));
        if (--cnt > 0) {
          setTimeout(next, 0, cnt);
        } else {
          total = Date.now() - now;
          console.log("node Average: " + total / count);
          process.exit(0);
        }
      });
    });

    req.on("error", e => {
      console.error(`problem with request: ${e.message}`);
    });

    req.end();
  }
};
