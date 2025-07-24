#!/bin/bash

VERSION="v1.0.0"
ARCH=$(uname -m)
OS=$(uname | tr '[:upper:]' '[:lower:]')

curl -Lo wg "https://github.com/AbnerTeng/wg/releases/download/${VERSION}/wg"
chmod +x wg
sudo mv wg /usr/local/bin/
echo "Installed wg successfully"
