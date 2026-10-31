# Litonpay Snap Packaging

Commands for building and uploading a Litonpay Core Snap to the Snap Store. Anyone on amd64 (x86_64), arm64 (aarch64), or i386 (i686) should be able to build it themselves with these instructions. This would pull the official Litonpay binaries from the releases page, verify them, and install them on a user's machine.

## Building Locally
```
sudo apt install snapd
sudo snap install --classic snapcraft
sudo snapcraft
```

### Installing Locally
```
snap install \*.snap --devmode
```

### To Upload to the Snap Store
```
snapcraft login
snapcraft register litonpay-core
snapcraft upload \*.snap
sudo snap install litonpay-core
```

### Usage
```
litonpay-unofficial.cli # for litonpay-cli
litonpay-unofficial.d # for litonpayd
litonpay-unofficial.qt # for litonpay-qt
litonpay-unofficial.test # for test_litonpay
litonpay-unofficial.tx # for litonpay-tx
```

### Uninstalling
```
sudo snap remove litonpay-unofficial
```