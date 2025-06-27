# NEWTONS Utils

NEWTONS Utils is a header only repository that includes helper structs for the [NEWTONS](https://github.com/Kargisa/newtons) engine.

## Tests

To run the NEWTONS Utils Tests, simply add the folder path as a sub directory and set the `-DNWT_UTILS_TESTS=ON` flag or if you are using CmakePresets.json set it as a `cacheVariables`.

<b>CMakeLitst.txt</b>

```cmake
add_subdirectory("path/to/newtons-utils")
```

<b>CMakePresets.json</b>

```json
{
    "cacheVariables": {
        ...
        "NWT_UTILS_TESTS": "ON"
    },
}
```
