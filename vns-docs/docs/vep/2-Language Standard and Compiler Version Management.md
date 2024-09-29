# VEP 2 - Language Standard and Compiler Version Management
![Static Badge](https://img.shields.io/badge/vns-2.3-blue?style=for-the-badge) ![Static Badge](https://img.shields.io/badge/Status-Final-green?style=for-the-badge "Accepted and implementation complete, or no longer active")

[TOC]

## Abstract
This VEP proposes a new tag, `[vns]`, for specifying the version of the VNS language standard to be used for the current script file. 

## Motivation
VNS languages have maintained backward compatibility across revisions, but this may not always be the case. Specifically, the removal of the `[br]` tag in VNS 2 can cause scripts using this tag to malfunction in later versions. Additionally, minor differences in behavior and outcomes due to bugs or other factors can cause games to fail. To mitigate these issues, a mechanism for specifying the language version is necessary. This version specification will also help developers perform version checks, thus preventing conflicts and incompatibility. Moreover, with the introduction of VNS-server, this feature will enable the server to select the most appropriate compiler for the compiling process.

## Specification

### Syntax
By default, specifying the language standard for a script file is optional, assuming the script should work regardless of the VNS compiler version. 

The `[vns]` tag specifies the language standard version:
```
[vns] {>=, <=, !<=, !>=} <version.revision>
```

### Examples
- `[vns]2.3`: This script is written according to VNS 2.3 standard and should only be compiled using the VNS 2.3 compiler.
- `[vns]<=2.3`: If the script works between versions 2.0 and 2.3, this tag can be used. Note that compilers released before VNS 2.3 will not support this tag, unless using VNS-server.
- `[vns]>=2.3`: If the script uses features introduced in VNS 2.3 and is not compatible with earlier versions.
- `[vns]!<=2.3`: This script works with versions 1.0 through 2.3.

Forward compatibility can be assumed between revisions, but forward and backward compatibility cannot be assumed between major version changes. Although not recommended, forced compatibility can be indicated using an exclamation mark.

### Backward Compatibility
The introduction of the [vns] tag will not affect existing scripts as it introduces new functionality without altering the behavior of current tags.

### Rejected Ideas
#### Allow Specifying Patch
Specifying patch numbers is unnecessary due to the relatively short release cycle of patches, which only include bug fixes and guarantee forward and backward compatibility.

#### Using `==` for Enforcing a Specific Version
Using `==` is redundant and may cause confusion. To avoid this, equality is assumed when no comparison operator is used, simplifying the syntax for non-programmers and improving readability.

#### Support for `>`, `<`, or `!=`
These operators can create unclear compatibility and introduce scripts that cannot run, such as `[vns]<2.0` when 2.0 is the first release of the VNS 2 standard. Additionally, the unlikely event of a tag being removed and reintroduced makes `!=` redundant. Higher version enforcement is preferable for maintaining script integrity.

## Conclusion
The introduction of the `[vns]` tag for specifying the version of the VNS language standard addresses the need for clearer compatibility and version management in VNS scripts. By allowing script writers to specify the exact version or range of versions their scripts are compatible with, we can prevent issues arising from changes in language features and behavior between versions. This proposal not only helps maintain backward compatibility but also supports forward compatibility, facilitating smoother transitions and integrations with tools like VNS-server. By adopting this proposal, the VNS language will enhance its robustness and reliability, ensuring that scripts perform consistently across different versions of the compiler.

## VEP Development Process
A live draft for this VEP lives on GitHub. There is also an issue tracker, where much of the technical discussion takes place.
The draft on GitHub is updated regularly in small increments. The official VEPS repo is (usually) only updated when a new draft is posted to the Linpg Foundation.

## Copyright
This document has been placed in the public domain.