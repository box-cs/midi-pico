if [ ! -d build ]; then
    mkdir build
fi

path=$(pwd)/pico-sdk/
cd build
cmake .. -DPICO_SDK_PATH=$path && make && picotool load -F main.uf2 && picotool reboot