#pragma once

// BuildInfo provides information about the running firmware.
//
// Initially these values are placeholders. As the framework evolves,
// BuildInfo will become the single source of truth for build metadata,
// board information, display configuration and firmware identity.
//
// UI pages should query BuildInfo rather than embedding knowledge
// about the firmware themselves.

// BuildInfo intentionally owns framework identity.
// It is the single source of truth for user-visible build metadata.

class BuildInfo
{
public:
    static const char* frameworkName();
    static const char* version();

    static const char* boardName();
    static const char* displayName();
};