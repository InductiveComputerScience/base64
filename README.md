# base64
This project contains a library for base64 encoding and decoding in many programming languages. The library works exactly the same in all implementations:

 * Java
 * C
 * C++
 * JavaScript
 * C#
 * PHP
 * Python
 * Visual Basic

The library has been made with [progsbase](https://www.progsbase.com), a tool for creating rot-resistant programs that are translatable to many programming languages.

The library can also be browsed and tested on the [progsbase repository's base64 page](https://repo.progsbase.com/repoviewer/no.inductive.idea10.programs/base64/0.1.0).

![progsbase logo](/docs/images/progsbase-logo.png)

For more information about progsbase, check out: [progsbase.com](https://www.progsbase.com).

## Usage

The library has the same interface in all programming languages.

### Encode

`Encoded encode(double [] data)`

The encode function takes an array of numbers, each holding a byte value, a value from 0 to 255, inclusive.

The output is a structure with three fields:

 * success: A boolean which is true iff encoding succeeded.
 * errorMessage: If encoding failed, this contains the human readable reason.
 * data: If encoding succeeded, the base64 encoded string of the input array.

### Decode
`Decoded decode(char [] data)`

The decode function takes a base64 string.

The output is a structure with three fields:

 * success: A boolean which is true iff decoding succeeded.
 * errorMessage: If decoding failed, this contains the human readable reason.
 * data: If decoding succeeded, an array of numbers, each holding a byte value, each between 0 and 255 inclusive.

## Examples

#### Java
`Encoded encoded = base64.encode(byteData);`

`Decoded decoded = base64.decode(base64CharacterArray);`

#### C
`Encoded *encoded = encode(byteData, byteDataLength);`

`Decoded *decoded = decode(base64String, base64StringLength);`

#### C++
`Encoded *encoded = encode(byteData);`

`Decoded *decoded = decode(base64CharacterArray);`

### JavaScript
`var encoded = encode(byteData)`

`var decoded = decode(base64CharacterArray)`

### C#
`Encoded encoded = base64.encode(byteData);`

`Decoded decoded = base64.decode(base64CharacterArray);`

### PHP
`$encoded = encode($byteData)`

`$decoded = decode($base64CharacterArray)`

### Python
`encoded = encode(byteData)`

`decoded = decode(base64CharacterArray)`

### Visual Basic .NET
`encoded = encode(byteData)`

`decoded = decode(base64CharacterArray)`