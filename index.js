import { createRequire } from "node:module";
import { readFileSync } from "node:fs";

const functions = {};
const batchFunctions = {};

{
  const require = createRequire(import.meta.url);
  const addon = require("./build/Release/rsqrt.node");

  functions.addon = addon.Q_rsqrt;
  batchFunctions["addon"] = (array) => {
    const data = new Float32Array(array);
    addon.Q_rsqrt_vec(data);

    return data;
  };
}

{
  const content = readFileSync("./build/Release/rsqrt.wasm");
  const wasm = await WebAssembly.instantiate(content);
  functions["llvm wasm"] = wasm.instance.exports.Q_rsqrt;
}

{
  const content = readFileSync("./src/rsqrt.wasm");
  const wasm = await WebAssembly.instantiate(content);
  functions["wat wasm"] = wasm.instance.exports.Q_rsqrt;
  functions["wat simple"] = wasm.instance.exports.rsqrt;

  /** @type {WebAssembly.Memory} */
  const memory = wasm.instance.exports.memory;

  const vecFn = wasm.instance.exports.Q_rsqrt_vec;

  /**
   *
   * @param {Float32Array} buffer
   */
  batchFunctions["wat"] = (buffer) => {
    const length = buffer.length * 4;
    const diff = length - memory.buffer.byteLength;
    if (diff > 0) memory.grow(diff);

    const clone = new Float32Array(memory.buffer);

    for (let i = 0; i < buffer.length; i++) {
      clone[i] = buffer[i];
    }

    vecFn(0, length);

    return clone;
  };
}

{
  functions.simple = (number) => 1 / Math.sqrt(number);

  functions.js = (number) => {
    let i;
    let x2, y;
    const threehalfs = 1.5;

    const f32 = new Float32Array(1);
    const i32 = new Int32Array(f32.buffer);

    x2 = number * 0.5;
    y = number;
    //evil floating bit level hacking
    f32[0] = number;
    i = i32[0];
    i = 0x5f3759df - (i >> 1); //What the fuck?
    i32[0] = i;
    y = f32[0];
    y = y * (threehalfs - x2 * y * y); // 1st iteration
    //  y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed

    return y;
  };

  const f32 = new Float32Array(1);
  const i32 = new Int32Array(f32.buffer);

  functions["js external"] = (number) => {
    let i;
    let x2, y;
    const threehalfs = 1.5;

    x2 = number * 0.5;
    y = number;
    //evil floating bit level hacking
    f32[0] = number;
    i = i32[0];
    i = 0x5f3759df - (i >> 1); //What the fuck?
    i32[0] = i;
    y = f32[0];
    y = y * (threehalfs - x2 * y * y); // 1st iteration
    //  y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed

    return y;
  };

  batchFunctions["simple"] = (inputs) => {
    const outputs = new Float32Array(inputs);

    for (let i = 0; i < inputs.length; i++) {
      outputs[i] = 1 / Math.sqrt(outputs[i]);
    }

    return outputs;
  };
}

function benchmark({
  name = method.name,
  method,
  inputs = [],
  count = 50_000,
}) {
  console.time(name);

  for (let i = 0; i < count; i++) {
    method(inputs[i % inputs.length]);
  }

  console.timeEnd(name);
}

const inputs = Array.from({ length: 1000 }, () => Math.random());

console.log(inputs);

const sampleSizes = [10_000, 100_000, 1_000_000, 10_000_000];

console.log("\n\n\nSingle functions:");

for (const count of sampleSizes) {
  console.log("=".repeat(10));

  for (const [name, method] of Object.entries(functions)) {
    benchmark({
      method,
      name: `${name}/${count.toPrecision(1).split("+")[1]}`,
      inputs,
      count,
    });
  }
}

console.log("\n\n\nVector functions:");

for (const count of sampleSizes.map((count) =>
  Math.floor(count / inputs.length)
)) {
  console.log("=".repeat(10));

  for (const [name, method] of Object.entries(batchFunctions)) {
    benchmark({
      method,
      name: `${name}/${inputs.length}x${count}`,
      inputs: [inputs],
      count,
    });
  }
}
