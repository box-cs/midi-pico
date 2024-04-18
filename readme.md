## Dependencies:

- pico-sdk folder with pico sdk contents
- picotool

See official pico C++ documentation for your platform

## Configuring Serial

### Finding vendor/product ids

```bash
lsusb | grep Pico
# Example Output:
# Bus 003 Device 005: ID 2e8a:000a Raspberry Pi Pico
```

### Setting udev rule

```bash
# In /etc/udev/rules.d/pico.rules
SUBSYSTEM=="usb", ATTRS{idVendor}=="2e8a", ATTRS{idProduct}=="000a", MODE="0666"
```

### Reloading udev rules (reboot required)

```bash
sudo udevadm control --reload-rules && sudo udevadm trigger
sudo reboot
```

### Loading with picotool

```bash
./load.sh # compiles and reboots pico
```

### Listening through serial

```bash
./serial.sh
```
