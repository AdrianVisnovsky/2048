# 2️⃣0️⃣4️⃣8️⃣

This is my implementation of 2048 in C++ with ncurses library. UI is adapting to your terminal window, but does not react to resize events. Some minimum terminal height and width is required...

## Features
- choose your board size
  - 3x3 board - small
  - 4x4 board - original / medium
  - 5x5 board - big
- high score table - save your biggest score

## Screenshot
<p align="center">
  
  <img src="https://raw.githubusercontent.com/AdrianKokula/2048/main/images/menu.png", alt="2048 game screenshot - menu" />
  <img src="https://raw.githubusercontent.com/AdrianKokula/2048/main/images/gameplay.png", alt="2048 game screenshot - gameplay" />
  
</p>

## Requirements
- `ncursesw library v6.2`
- `cmake`
- `make`

## Compilation
```
mkdir build && cd build
cmake ..
make
cd 2048
./2048
```

## Contributing
Feel free to make changes, create pull request or submit an issue.

## License
This implementation is licensed under the MIT license, see LICENSE.md
