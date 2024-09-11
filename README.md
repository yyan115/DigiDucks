# Repo of team DigiDucks.

## Folder structure
```
Project/                # ROOT folder containing all project-related files
├── DuckEngine/             # Main engine project folder
│   ├── Build/                # Build directory for compiled files (Debug/Release executables)
│   ├── Engine/               # Core engine source code
│       ├── src/                # Engine source files (.cpp)
│       ├── include/            # Engine header files (.h/.hpp)
│       ├── lib/                # External libraries required by the engine
│   └── Game/                 # Game-specific code and assets
│       ├── src/                # Game source files (.cpp)
│       ├── include/            # Game header files (.h/.hpp)
│       ├── assets/             # Game assets (images, sounds, etc.)
└── README.md
```

## Build Instructions

Engine gets compiled into a DLL file that the Game files will use. You do not need to rebuild the engine unless changes to engine were made.

Set Game as startup project and press build or run in Visual Studio. (Otherwise you will get error when first running)

If you want to use Engine features, simply add #include "DuckEngine.h" to your game scripts.

Engine documentation is WIP. Feel free to ask if there are any enquiries.

Please communciate if you want to work on something, especially on the engine!!

Milestone/Tasks tracker (Notion) - https://www.notion.so/182bafe0170a4d2286f25a523a90060a?v=daba320aa9fd43a6ab404f660f09695e&pvs=4