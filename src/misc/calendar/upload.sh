#!/bin/bash

set -e

CURRENT_DIR=$( cd "$(dirname "${BASH_SOURCE[0]}")" ; pwd -P )
cd ${CURRENT_DIR}

if [ $# -ne 1 ]; then
    DST_HOST='192.168.50.107'
else
    DST_HOST=$1
fi

PROJECT='calendar'
DST_DIR="/root/projects/${PROJECT}"
VERSION=`date +%Y%m%d%H%M%S`

rm -rf dist/${VERSION}/
mkdir -p dist/${VERSION}/
cp ${CURRENT_DIR}/*.h dist/${VERSION}/
cp ${CURRENT_DIR}/*.c dist/${VERSION}/
cp ${CURRENT_DIR}/Makefile dist/${VERSION}/
cd dist
rm -f ${VERSION}.zip
zip -r ${VERSION}.zip ${VERSION}/

# Upload and make
ssh -t root@${DST_HOST} "mkdir -p ${DST_DIR}/releases"
scp ${VERSION}.zip root@${DST_HOST}:${DST_DIR}/releases
ssh -t root@${DST_HOST} <<EOF
    set -e
    cd ${DST_DIR}/releases
    rm -rf ${VERSION}/
    unzip ${VERSION}.zip
    rm -f ${VERSION}.zip
    cd ${VERSION}/
    make -j4
    cd ../../
    rm -rf latest
    ln -s ${DST_DIR}/releases/${VERSION} latest
    # remove old releases
    echo "remove old releases"
    cd ${DST_DIR}/releases
    ls -t | tail -n +4 | xargs rm -rf
    echo "done"
    cd ${DST_DIR}/latest
    ./calendar
EOF

rm -rf cd ${CURRENT_DIR}/dist
