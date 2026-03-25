# AsyncTickPhysics

AsyncTickPhysics is an Unreal Engine plugin for building custom Chaos physics-step workflows without attaching your gameplay code to Unreal's built-in Async Physics Tick registration path.

This repository contains:

- the plugin itself in `Plugins/AsyncTickPhysics`
- a host project used for local development, examples, and validation

## Highlights

- Custom Chaos sim callback pipeline for per-step async physics workflows
- Blueprint-friendly base pawn: `AAsyncTickPawn`
- Blueprint function library for physics-thread-facing operations: `UAsyncTickFunctions`
- Example content and a sample test map included in the plugin

## Supported Versions

- Unreal Engine `5.5`
- Unreal Engine `5.6`
- Unreal Engine `5.7`

Compatibility note:

- `5.5` and `5.7` were verified with `RunUAT BuildPlugin`
- `5.6` compatibility is inferred from the unchanged API surface between the verified endpoints

Validated platform:

- `Win64`

## Design Notes

AsyncTickPhysics keeps its own custom Chaos callback.

It does **not** register actors or components through Unreal's built-in Async Physics Tick system.

The plugin callback is configured to run on the frozen game thread before the physics step, which keeps the custom workflow separate while avoiding the unsafe lifecycle issues caused by running Blueprint-facing logic directly on the raw physics thread.

## Repository Layout

```text
.
|-- Plugins/AsyncTickPhysics    # Plugin source, content, config, packaged binaries
|-- Source/asynctick            # Host project module for local testing
|-- Content/                    # Host project content
|-- Config/                     # Host project config
`-- asynctick.uproject          # Host project descriptor
```

## Installation

### Option 1: Use the plugin from this repository

1. Clone or copy this repository.
2. Open `asynctick.uproject` with Unreal Engine `5.5` to `5.7`.
3. Build the project if Unreal asks for a recompile.
4. Enable the `AsyncTickPhysics` plugin if it is not already enabled.

### Option 2: Copy the plugin into another project

1. Copy `Plugins/AsyncTickPhysics` into your target project's `Plugins` folder.
2. Regenerate project files if your project uses C++.
3. Build the project in your target Unreal Engine version.

## Quick Start

1. Create a Blueprint or C++ class derived from `AAsyncTickPawn`.
2. Implement `AsyncTick(float DeltaTime)` in Blueprint, or override `NativeAsyncTick(float DeltaTime)` in C++.
3. Call `ATP_*` helper functions from `UAsyncTickFunctions` inside that async tick path.
4. Use the included sample content as a reference if needed.

## Blueprint Usage

`AAsyncTickPawn` exposes a Blueprint event:

- `AsyncTick(float DeltaTime)`

This event is intended for the plugin's custom async physics-step flow.

## C++ Usage

```cpp
virtual void NativeAsyncTick(float DeltaTime) override
{
	Super::NativeAsyncTick(DeltaTime);

	if (MeshComponent)
	{
		UAsyncTickFunctions::ATP_AddForce(
			MeshComponent,
			FVector(0.0f, 0.0f, 5000.0f),
			false);
	}
}
```

## Public API

### `AAsyncTickPawn`

- Base pawn type for plugin-managed async physics-step logic
- Registers itself with the plugin manager at `BeginPlay`
- Unregisters at `EndPlay`

### `UAsyncTickFunctions`

Main Blueprint-callable helpers include:

- `ATP_AddForce`
- `ATP_AddForceAtPosition`
- `ATP_AddTorque`
- `ATP_AddImpulse`
- `ATP_AddImpulseAtPosition`
- `ATP_AddAngularImpulseInRadians`
- `ATP_AddAngularImpulseInDegrees`
- `ATP_GetTransform`
- `ATP_GetLinearVelocity`
- `ATP_GetAngularVelocity`
- `ATP_SetLinearVelocity`
- `ATP_SetAngularVelocityInRadians`
- `ATP_SetAngularVelocityInDegrees`
- `ATP_SetWorldLocation`
- `ATP_SetWorldRotation`
- `ATP_SetWorldLocationAndRotation`
- `ATP_GetLinearVelocityAtPoint`

## Sample Content

The plugin ships with example assets under `Plugins/AsyncTickPhysics/Content`, including:

- a sample map
- example Blueprint assets for testing the async tick flow


## Development Notes

- The host project targets are aligned to UE `5.7` build defaults (`BuildSettingsVersion.V6`)
- The plugin descriptor no longer hard-pins an old engine version
- Runtime validation in this repository was completed against UE `5.5` and UE `5.7`

## Changelog

See [CHANGELOG.md](./CHANGELOG.md).

## Contributing

Issues and pull requests are welcome.

When contributing:

- keep fixes minimal and version-aware
- preserve compatibility with UE `5.5` to `5.7`
- avoid routing plugin logic through Unreal's built-in Async Physics Tick path
