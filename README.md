# BattleCity_v2.0

### Install packages
```
sudo apt install cmake build-essential ccache python3-pip libgl-dev xorg-dev libx11-xcb-dev libxcb-render0-dev libxcb-render-util0-dev libxcb-xkb-dev libxcb-icccm4-dev libxcb-image0-dev libxcb-keysyms1-dev libxcb-randr0-dev libxcb-shape0-dev libxcb-sync-dev libxcb-xfixes0-dev libxcb-xinerama0-dev libxcb-dri3-dev libxcb-util-dev libudev-dev
```
```
pip install conan
```

### Using Sanitizers
Using GCC
1. Turn on ENABLE_COVERAGE on cmake/Sanitizers.cmake on choose needed options.
2. Build and run binary.
3. Check for *.gcno && *.gcna next to object files are created. Go to that directory.
4. lcov --capture --directory . --output-file=coverage.info
5. genhtml coverage.info --output-directory=result
See html file in result folder.