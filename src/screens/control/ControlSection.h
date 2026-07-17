#pragma once

// Top-level sections available within the Control Panel.
//
// Sections group related pages together.
// They represent areas of responsibility within the framework rather
// than individual screens.

enum class ControlSection
{
    Information,
    Connectivity,
    Storage,
    Diagnostics,
    Display,
    Input,
    Power,
};