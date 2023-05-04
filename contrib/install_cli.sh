 #!/usr/bin/env bash

 # Execute this file to install the gamepass cli tools into your path on OS X

 CURRENT_LOC="$( cd "$(dirname "$0")" ; pwd -P )"
 LOCATION=${CURRENT_LOC%Gamepass-Qt.app*}

 # Ensure that the directory to symlink to exists
 sudo mkdir -p /usr/local/bin

 # Create symlinks to the cli tools
 sudo ln -s ${LOCATION}/Gamepass-Qt.app/Contents/MacOS/gamepassd /usr/local/bin/gamepassd
 sudo ln -s ${LOCATION}/Gamepass-Qt.app/Contents/MacOS/gamepass-cli /usr/local/bin/gamepass-cli
