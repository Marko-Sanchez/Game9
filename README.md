![game nine banner](resources/images/readme-title.png "Game9 Banner")
[![wakatime]([wakaimage])]([wakahours])

<p align="center">
Game9 the 9th game in the series.
</p>

## Build
To build the project:
First, clone the repository and intialize the submodules.
```
git clone --recursive <repository-url>
```
if you forget to add `--recursive` or wish to only intialize one submodule.
```
git submodule update --init --recursive
# or
git submodule update --init <path-to-submodule>
```
Second, build the project either in 'Debug' or 'Release' mode.
```
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```
Finally, the executable should be `build/src/game9`

[wakaimage]: https://wakatime.com/badge/user/f6244e02-6622-43fe-a0a9-cdbfd95dfe72/project/089cea2c-f28c-4232-9d39-3455e27ee1e7.svg
[wakahours]: https://wakatime.com/badge/user/f6244e02-6622-43fe-a0a9-cdbfd95dfe72/project/089cea2c-f28c-4232-9d39-3455e27ee1e7
