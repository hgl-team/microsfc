#!/bin/bash

rm -rf ~/Arduino/libraries/microsfc
mkdir -p ~/Arduino/libraries/microsfc
cp -r src/* ~/Arduino/libraries/microsfc/
cp -r include/* ~/Arduino/libraries/microsfc/