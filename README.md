# pkgx
> modernized debian packages | C++17 & C11


## Prelude
`pkgx` _(originally xpkg)_ is a new container format based on the debian archive. I tried to mix the better parts from `ar` and went from there.

The full format specification can be found in the `/src` folder to keep this a bit more organized.

As to _why_, the answer is because _why not_. In all seriousness, I just wanted to gain some experience with how file formats are made & structured, debain archives seemed like a perfect target to work on.


## Compiling
I aimed to keep this project mostly cross-platform, POSIX compliancy is lacking but if anyone wants to, feel free to PR it.

Simply clone the repository and compile to have your instance running.

Make sure you fullfill the following dependencies _before_ compiling:
- zstd
- nlohmann's json

Afterwards you'll be left with `pkgx`, the packgaing & management utility for a `.pkgx` archive.


## Inclusion & Usage Guide
Beyond the conditions specified in the LICENSE file, the following terms apply:
- Any project using this spec must retain the original copyright.
- The fork MUST be open-source on GitHub, however proprietary bits may be stripped from the source code if you so choose to BUT the excluded bits must be mentioned in the respective fork's README.
- Be kind to those seeking help about your modifications.


## License
This project is licensed under the _BSD3 (With Attribution)_ (SPDX: `BSD-3-Clause-Attribution`) **WITH** aforementioned clauses in the _"Inclusion & Usage Guide"_ section.
