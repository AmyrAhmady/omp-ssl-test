#!/bin/bash

# Available configs: Debug, [RelWithDebInfo], Release
[[ -z "$CONFIG" ]] \
&& config=RelWithDebInfo \
|| config="$CONFIG"
# Available versions: 18.04, [20.04], 22.04
[[ -z "$UBUNTU_VERSION" ]] \
&& ubuntu_version=18.04 \
|| ubuntu_version="$UBUNTU_VERSION"
# Available options: [true], false
[[ -z "$BUILD_SHARED" ]] \
&& build_shared=1 \
|| build_shared="$BUILD_SHARED"

docker build \
    -t open.mp/build:ubuntu-${ubuntu_version} \
    build_ubuntu-${ubuntu_version}/ \
|| exit 1

folders=('build' 'conan')
for folder in "${folders[@]}"; do
    if [[ ! -d "./${folder}" ]]; then
        mkdir ${folder} &&
        chown 1000:1000 ${folder} || exit 1
    fi
done

docker run \
    --rm \
    -t \
    -w /code \
    -v $PWD/..:/code \
    -v $PWD/build:/code/build \
    -v $PWD/conan:/home/user/.conan \
    -e CONFIG=${config} \
    -e BUILD_SHARED=${build_shared} \
    open.mp/build:ubuntu-${ubuntu_version}
