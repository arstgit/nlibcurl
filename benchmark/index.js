const { spawn } = require("child_process");
serverPath = __dirname + "/server.js";
const server = spawn("node", [serverPath]);

server.on("exit", code => {
  console.log("server exit");
  process.exit(code);
});
server.stdout.on("data", function(data) {
  if (data.toString().indexOf("start") >= 0) {
    return run();
  }
});

process.on("exit", code => {
  console.log("process exit");
  server.kill();
});

const libcurl = require("./curl");
const node = require("./node");

const urlStr = "http://127.0.0.1";
const cnt = 10;
function run() {
  libcurl(cnt, urlStr);
  node(cnt, urlStr);
}
