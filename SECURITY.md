# Security Policy

## Reporting a Vulnerability

Do not open a public GitHub issue for security vulnerabilities.

Send a report to **security@lumencanvas.studio** with:

- Description of the vulnerability
- Steps to reproduce
- Affected component (firmware, SDK, broker)
- Version or commit hash

You will receive a response within 72 hours. We will work with you to understand the issue and coordinate a fix before any public disclosure.

## Supported Versions

| Version | Supported |
|---------|-----------|
| 0.1.x   | Yes       |

## Scope

Security issues in the following components are in scope:

- Firmware library (buffer overflows, memory corruption, packet injection)
- SDK clients (command injection, unsafe deserialization)
- Broker configuration (default credentials, exposed services)
- Wire protocol (CRC bypass, framing attacks)

## Disclosure

We follow coordinated disclosure. Fixes are released before public details are shared.
