#!/bin/bash

VERSION="v0.1.0"
ARCH=$(uname -m)
OS=$(uname | tr '[:upper:]' '[:lower:]')

curl -Lo wg "https://github.com/AbnerTeng/wg/releases/download/${VERSION}/wg"
chmod +x wg
sudo mv wg $HOME/.local/bin/wg
echo "Installed wg successfully"
