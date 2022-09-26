#!/bin/bash

install_packages(){
    PKG_ARR=("$@")
    for REQUIRED_PKG in "${PKG_ARR[@]}"; do
        PKG_OK=$(dpkg-query -W --showformat='${Status}\n' $REQUIRED_PKG|grep "install ok installed")
        if [ "$PKG_OK" = "install ok installed" ]; then
            PKG_OK="$REQUIRED_PKG is already installed."
        fi
        echo Checking for $REQUIRED_PKG: $PKG_OK
        if [ "" = "$PKG_OK" ]; then
            echo "No $REQUIRED_PKG. Setting up $REQUIRED_PKG."
            sudo apt-get install $REQUIRED_PKG
        fi
    done
}

install_carol_fi(){
    installation_dir="$PWD"
    cp -a . /tmp/smith-waterman-fault-injection
    cd /tmp/smith-waterman-fault-injection
    git checkout fc5c66b62 >/dev/null 2>/dev/null
    cp -a carol-fi $installation_dir/carol-fi/
    rm -rf /tmp/smith-waterman-fault-injection/
    cd $installation_dir
}

install_all_dependencies(){
    declare -a REQUIRED_PKGS=("gcc" "gdb" "make")
    install_packages "${REQUIRED_PKGS[@]}"
    install_carol_fi
}

while getopts "c" opt
	do
		case "$opt" in
			c ) parameterC=true ;;
			? ) install_all_dependencies ;;
		esac
	done

if [ "$parameterC" ]; then
	install_carol_fi
fi