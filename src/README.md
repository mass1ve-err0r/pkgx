# `pkgx` format specification

In essence a `pkgx` archive is nothing more than 3 _compressed_ files concatenated, including the header of course. The files are compressed with zstd to make it smaller in size.

## Structure
Each `pkgx` must begin with the `pkgx_header_t` to identify and later correctly parse the archive. 
The header is in total 16 bytes and has the following fields:
| Field (uint32_t)  | Value |
| ------------- | ------------- |
| magic  | `pkgx` magic (0xdeadc0de)  |
| control_sz  | size of the compressed control file  |
| layout_sz  | size of the compressed layout file  |
| data_sz  | size of the compressed data* file  |

_(If you'd like more details on the header, please look inside the `/src/format` subdirectory.)_

Following the header are the three compressed files which will be explained in the following subsections:

### 1.1: `control.json.zst`
The `control.json.zst` is a JSON file which describes the details surrounding the `pkgx`. This mimics the debian archive's `control` file but adds a bit more flexibility in terms of defining min/max versions & equally _(if not, more!)_ extensibility since parsing can be adapted to recognize new fields.


### 1.2: `layout.json.zst`
The `layout.json.zst` is also a JSON and maps out the package's contents to the filesystem, including permissions and symlinking. This might seem a bit irritating at first but the reason to go with a sperate file describing the destination, however my goal was to reduce the original `ar` file's overhead because **in my opinion** a simpler concat'ed file is more efficient for this particular usage.

In general, the layout file describes an object's:
- name
	- install name _(optional)_
- install location
- permissions
- symlinking _(optional)_

### 1.3 `data.compactarchive.zst`
This is probably the one of the cooler thing I had to come up with. You might have noticed the asterik on th last entry, that's because `pkgx` uses a different but fairly similar archiving strategy to the standard `ar` archive which I dubbed _"compactarchive"_.

This is my idea of how an `ar` archive could look if it would be simplified to just archiving/ storing object files. 

A `compactarchive` begins with a plain `uint32_t` describing how many objects are inside, followed by N pairs of (object size, object) depending on the user's choice.

Because a `compactarchive` is barebone, the layout file is it's crucial counterpart describing where the objects inside have to go with which permissions etc.

---
An example can be found inside `/example` for you to experiment with.
