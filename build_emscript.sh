#!/bin/bash
#mkdir cmake-build-emrelease
rm -fr cmake-build-emrelease
emcmake cmake -DCMAKE_BUILD_TYPE=Release -DUSE_EXTRAS=ON -S ./ -B ./cmake-build-emrelease
cd cmake-build-emrelease
emmake make -j 16

cp FinalProject.js /var/www/html/emscript/
cp FinalProject.data /var/www/html/emscript/
cp FinalProject.wasm /var/www/html/emscript/
cp FinalProject.worker.js /var/www/html/emscript/
