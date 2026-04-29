# Changelog

<!-- towncrier release notes start -->

## 1.1.0 (2026-04-29)

### Features

- Add launchpad-build scaffolding via shared-workflows bootstrap script. Patch CMakeLists.txt so ExternalProject_Add pulls launchpad-build/ADS at the commit matching tag 113.0.30-1, giving the org one source of truth for the low-level ADS library. (L3H-129)
