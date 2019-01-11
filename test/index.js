const curl = require("..");

const res = curl("https://example.com");

if (!res.includes("Example Domain")) throw "Test failed";

console.log("passed!");
