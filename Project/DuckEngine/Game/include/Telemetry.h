#pragma once

/// Read-only sampler for driving the game from outside it.
///
/// Compiled in only when the CMake option QUACK_KITCHEN_TELEMETRY is ON, which
/// it is not by default, so no release package contains any of this. Even when
/// compiled in it stays inert unless QUACK_KITCHEN_TELEMETRY_PATH names a file
/// to write, so an instrumented build still behaves exactly like a normal one
/// when run normally.
///
/// It only reads. Nothing here may change game state, because the point is to
/// observe the game as a player experiences it.
namespace Telemetry
{
	/// Open the output file if the environment asks for one. Safe to call when
	/// telemetry is compiled out or unconfigured.
	void Initialise();

	/// Write one sample. Called once per frame from the main loop; it rate
	/// limits internally so the file stays readable and the cost stays small.
	void Sample();

	/// Flush and close.
	void Shutdown();
}
