# `Q_rsqrt` NodeJS benchmark

Benchmark of fast inverse square root implementations for NodeJS.

## Run

```bash
node index.js
```


## Output

```
Single value inputs:
==========
addon/6: 51.186ms
llvm wasm/6: 13.694ms
wat wasm/6: 10.459ms
simple/6: 9.976ms
js/6: 311.792ms
js external/6: 11.276ms
==========


Array inputs:
==========
addon/1000x10: 0.12ms
wat/1000x10: 0.297ms
simple/1000x10: 0.56ms
==========
addon/1000x100: 0.429ms
wat/1000x100: 1.135ms
simple/1000x100: 0.502ms
==========
addon/1000x1000: 5.03ms
wat/1000x1000: 1.222ms
simple/1000x1000: 5.075ms
==========
addon/1000x10000: 42.844ms
wat/1000x10000: 7.926ms
simple/1000x10000: 48.996ms
```


`/6` - means 1e6 iterations = 1 million

## Variants

- `addon` - node-gyp compiled C addon that uses [N-API](https://nodejs.org/api/n-api.html) 
  ```bash
  node-gyp build
  ```
- `llvm wasm` - CLang compiled [src/rsqrt.c](src/rsqrt.c) to WASM.
  Command:
  ```bash
  clang --target=wasm32 -nostdlib -Wl,--no-entry -Wl,--export-all -o build/Release/rsqrt.wasm src/rsqrt.c
  ```
- `wat wasm` - `wat2wasm` compiled WebAssembly Text Format version of this alg. See [src/rsqrt.wat](src/rsqrt.wat)
  ```bash
  npx wat2wasm src/rsqrt.wat
  ```
- `simple`
  ```javascript
  const simple = (x) => 1 / Math.sqrt(x);
  ```
- `js` JS version from [this gist](https://gist.github.com/starfys/aaaee80838d0e013c27d)
- `js external` JS version with [buffers outside of function scope](https://gist.github.com/starfys/aaaee80838d0e013c27d?permalink_comment_id=3875468#gistcomment-3875468)


## Conclusion 

- NodeJS optimizing jit-compiler is amazing
- WASM is the best way to perform computationally-hard operations over arrays for JS
- I suck at C
