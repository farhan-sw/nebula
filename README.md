# NEBULA PROJECT
![Nebula](https://github.com/farhan-sw/nebula/blob/main/images/imagesspitzer20170510nebula-16.width-1280.gif)

Project ini merupakan project yang dibuat untuk mempelajari dan riset fokus pada ROS2 dan micro-ROS. Project ini dibuat oleh tim KRAI 2024.

## Cloning Project
Untuk melakukan cloning project ini, gunakan perintah berikut:
```bash
git clone https://github.com/farhan-sw/nebula.git
```
Update submodule dengan perintah berikut:
```bash
git submodule update --init --recursive
```

## SETUP PROJECT NEBULA
Untuk melakukan setup project nebula, ikuti langkah pada nebula-setup/README.md

## DIRECTORY STRUCTURE
```
nebula
├── mbed-microros
├── nebula-mbed
│   ├── mbed-os
│   ├── KRAI-Library
├── nebula-ws
```

Explanation:
- mbed-microros: Folder yang berisi project micro-ROS pada platform arduino dan PlatformIO
- nebula-mbed: Folder yang berisi project mbed-os dan KRAI-Library
- nebula-ws: Folder yang berisi project ROS2
- nebul-setup: Folder yang berisi setup project nebula
