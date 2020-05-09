#!/bin/bash

pushd ../
vendor/premake/premake5 gmake2
popd
read -p "Press any key to continue" x
