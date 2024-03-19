# Emscripten

## Build

```
mkdir build && cd build
emcmake cmake ..
emmake make -j$(nproc)
```

## Add configuration file

`smw.ini` and `smw.sfc` must be copied to `build` directory.

## Link

```
emcc -flto -O3 *.o libgl.a -o index.html --shell-file ../shell.html -sUSE_SDL=2 -sENVIRONMENT=web -sINVOKE_RUN=0 --preload-file smw.ini --preload-file smw.sfc --preload-file smw_assets.dat -Wl,-u,fileno -lidbfs.js

emcc -flto -O3 *.o libgl.a -o smb1.html --shell-file ../shell.html -sUSE_SDL=2 -sENVIRONMENT=web -sINVOKE_RUN=0 --preload-file smw.ini --preload-file smb1.sfc --preload-file smw_assets.dat -Wl,-u,fileno -lidbfs.js

emcc -flto -O3 *.o libgl.a -o smbll.html --shell-file ../shell.html -sUSE_SDL=2 -sENVIRONMENT=web -sINVOKE_RUN=0 --preload-file smw.ini --preload-file smbll.sfc -Wl,-u,fileno -lidbfs.js
```

# Run locally

```
emrun index.html

Then open in browser: http://localhost:6931/ 