#!/bin/sh

appDir="$(cd "$(dirname "$0")"; pwd)"

${appDir}/scripts/genericmake Release $*
