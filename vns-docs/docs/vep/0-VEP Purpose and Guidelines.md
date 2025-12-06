# VEP 0 - VEP Purpose and Guidelines
![Static Badge](https://img.shields.io/badge/vns-2.0-blue?style=for-the-badge) ![Static Badge](https://img.shields.io/badge/Status-Final-green?style=for-the-badge "Accepted and implementation complete, or no longer active")

[TOC]

## Abstract
This VEP describes what a VEP (VNS Enhancement Proposal) is, explains the purpose of VEPs, and provides guidelines for writing and submitting VEPs.

## What is a VEP?
VEP stands for VNS Enhancement Proposal. A VEP is a design document providing information to the VNS community, describing a new feature, syntax, or convention for the Visual Novel Script (VNS) language. VEPs are the primary mechanism for proposing significant changes to the VNS language standard and for documenting the design decisions that have gone into VNS.

## VEP Types
VEPs can be categorized into the following types:

- **Feature VEP**: Proposes a new feature or syntax for VNS (e.g., new tags, variable systems, control flow mechanisms).
- **Informational VEP**: Provides general guidelines or information to the VNS community but does not propose a new feature.
- **Process VEP**: Describes a process surrounding VNS or proposes a change to a process (this VEP is an example of a Process VEP).

## VEP Status
Each VEP has a status indicating its current state:

- **Draft**: The VEP is under active discussion and development.
- **Accepted**: The VEP has been approved but implementation is not yet complete.
- **Final**: The VEP has been accepted and implementation is complete, or it is no longer active.
- **Rejected**: The VEP has been rejected and will not be implemented.
- **Withdrawn**: The VEP has been withdrawn by the author(s).
- **Deferred**: The VEP is postponed for future consideration.

## VEP Structure
A VEP should contain the following sections:

### Required Sections

1. **Title**: A concise, descriptive title in the format "VEP X - Title"
2. **Status Badges**: Include version and status badges at the top of the document
3. **Abstract**: A short (~200 word) description of the technical issue being addressed
4. **Motivation/Rationale and Goals**: Clearly explain why the existing VNS specification is inadequate to address the problem that the VEP solves
5. **Specification/Definition**: The technical specification should describe the syntax and semantics of any new feature
6. **Conclusion**: A summary of the proposal and its benefits

### Optional Sections

- **Syntax**: Detailed syntax examples if proposing new language features
- **Optional Syntax**: Additional syntax options for convenience and flexibility
- **Backward Compatibility**: Describe how the proposal affects existing scripts
- **Rejected Idea(s)**: Document alternative approaches that were considered but not chosen, with explanations
- **Examples**: Usage examples demonstrating the proposed feature

### Standard Footer Sections

All VEPs should include these sections at the end:

1. **VEP Development Process**: Information about where the live draft and issue tracker are located
2. **Copyright**: Copyright notice (typically public domain)

## Writing a VEP

### Before You Begin
1. Ensure your idea is within scope for VNS
2. Search existing VEPs to make sure your idea has not already been proposed or rejected
3. Consider discussing your idea informally with the community before writing a formal VEP

### Format Guidelines
- Use Markdown format for all VEPs
- Include a Table of Contents using `[TOC]`
- Use clear, concise language
- Provide concrete examples where applicable
- Use code blocks for syntax examples

### Status Badges
Include the following badges at the top of your VEP:

```markdown
![Static Badge](https://img.shields.io/badge/vns-X.X-blue?style=for-the-badge) ![Static Badge](https://img.shields.io/badge/Status-STATUS-COLOR?style=for-the-badge "Status description")
```

Where:
- `X.X` is the VNS version the proposal targets
- `STATUS` is Draft, Accepted, Final, Rejected, Withdrawn, or Deferred
- `COLOR` is green for Final, yellow for Draft, red for Rejected, etc.

### File Naming Convention
VEP files should be named using the following format:
```
<VEP-number>-<Title>.md
```
For example: `1-Variables.md`, `2-Language Standard and Compiler Version Management.md`

## Submitting a VEP

1. Fork the VNS repository on GitHub
2. Create your VEP as a new Markdown file in the `vns-docs/docs/vep/` directory
3. Add an entry for your VEP in `vns-docs/mkdocs.yml` under the "Visual Novel Script Enhancement Proposals" section
4. Submit a pull request to the main repository
5. The VEP will be reviewed by the VNS maintainers
6. Address any feedback and revise the VEP as needed
7. Once accepted, the VEP will be merged and assigned a number

## VEP Development Process
A live draft for this VEP lives on GitHub. There is also an issue tracker, where much of the technical discussion takes place.
The draft on GitHub is updated regularly in small increments. The official VEPs repo is (usually) only updated when a new draft is posted to the Linpg Foundation.

## Copyright
This document has been placed in the public domain.
