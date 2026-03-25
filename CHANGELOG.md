# Changelog

All notable changes to this repository will be documented in this file.

This changelog follows the spirit of [Keep a Changelog](https://keepachangelog.com/en/1.1.0/) and uses [Semantic Versioning](https://semver.org/).

## [1.2.0] - 2026-03-25

### Added

- Added a GitHub-friendly `README.md`
- Added this `CHANGELOG.md`

### Changed

- Updated the host project targets to UE `5.7` build defaults with `BuildSettingsVersion.V6`
- Set the project include order explicitly to `EngineIncludeOrderVersion.Unreal5_7`
- Removed the old hard engine-version pin from the plugin descriptor
- Aligned the plugin metadata and repository documentation for public distribution

### Fixed

- Kept the plugin on its own custom Chaos callback path instead of wiring it into Unreal's built-in Async Physics Tick registration flow
- Switched the custom callback to `RunOnFrozenGameThread` to stabilize Blueprint-facing async tick execution
- Fixed manager lifecycle issues around duplicate registration, stale callback pointers, and world cleanup
- Fixed stale weak-object accumulation and registration duplication in the pawn manager
- Fixed transform helper fallback behavior when a physics handle is unavailable
- Fixed `ATP_GetLinearVelocityAtPoint` so it no longer rejects valid rigid-body cases
- Disabled normal actor tick by default on `AAsyncTickPawn` to reduce unnecessary runtime overhead

### Compatibility

- Verified plugin packaging builds on UE `5.5`
- Verified plugin packaging builds on UE `5.7`
- Kept UE `5.6` compatibility as an expected intermediate version based on the unchanged APIs touched by this release

## Earlier History

Earlier repository changes were not maintained in a public changelog.
